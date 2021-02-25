// This program will take the JSON files containing the state of DPD Simulation
// And calculate the RDF for this state. It does this by building a Volume
// object from this state, and then passing it into an RDFCalculator thread
// which does all the work.

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h>
#include <fstream>
#include <sstream>
#include <thread>
#include <queue>
#include <tuple>
#include <sys/ioctl.h>
#include <unistd.h>

#include "rapidjson/document.h"

#include "Volume.hpp"
#include "RDFCalculator.hpp"

#define MAX_THREADS 10

enum CoreState { idle, busy };

// ****** Structs *****
struct CalculatorMonitor {
    Progress progress;
    double percent;
    uint32_t core;
};

struct CoreInfo {
    uint8_t core_num;
    CoreState state;
};

// ****** USER DEFINED CONSTANTS ******
const float volume_length = 100.0;
const std::string state_dir = "../100_vesicle_frames/";
const unsigned number_density = 3;
const unsigned number_bead_types = 3;
const bool printBeadNum = false;
const int cells_per_dimension = 100;
std::vector<std::string> typeToName = {"head", "tail", "water"};

#define TIMESTEPS 20
const uint32_t timesteps[TIMESTEPS] = {0, 10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000, 90000 , 100000,
                                 200000, 300000, 400000, 500000, 600000, 700000, 800000, 900000, 1000000};
// #define TIMESTEPS 3
// const uint32_t timesteps[TIMESTEPS] = {0, 10000, 20000};

// ****** Global variables (to make life easier for now) ******
std::queue<RDFCalculator> calculator_queue;
std::map<uint32_t, std::thread> threads;
std::map<uint32_t, std::vector<std::vector<std::vector<double>>>> results;
std::map<uint32_t, CalculatorMonitor> monitor_map;
CoreInfo cores[MAX_THREADS];
std::map<uint32_t, CoreInfo *> core_map;
std::vector<RDFCalculator> calculators;
std::vector<std::vector<std::string>> out_filepaths;
std::map<uint32_t, timeval> start_times;
uint32_t x = 0;
uint32_t y = 0;

std::string getFileAsString(std::string filepath) {
    std::ifstream t(filepath);
    std::stringstream buffer;
    buffer << t.rdbuf();
    return buffer.str();
}

std::vector<std::vector<std::string>> prep_out_filepaths(std::vector<std::string> typeToName, float volume_length) {
    std::vector<std::vector<std::string>> out_filepaths;
    // Prepare filenames
    for (uint8_t i = 0; i < typeToName.size(); i++) {
        std::vector<std::string> v;
        for (uint8_t j = i; j < typeToName.size(); j++) {
            std::string path = "../rdf_data/";
            path += std::to_string((uint32_t) volume_length);
            path += "_rdf_";
            path += typeToName.at(std::min(i, j));
            path += "_";
            path += typeToName.at(std::max(i, j));
            path += ".csv";
            // Open the file to clear it
            FILE* f = fopen(path.c_str(), "w+");
            // A1 of the CSV should be clear
            // Add r values
            double r = 0;
            double rmax = volume_length / 10;
            double dr = rmax / 100;
            while (r < rmax) {
                // Write r value as the top line of files
                fprintf(f, ", %1.3f", r);
                r += dr;
            }
            fprintf(f, "\n");
            fclose(f);
            v.push_back(path);
        }
        out_filepaths.push_back(v);
    }
    // Clear the runtime file
    FILE* f = fopen("../rdf_data/runtimes.csv", "w+");
    fprintf(f, "Timestep, Runtime\n");
    fclose(f);
    return out_filepaths;
}

void prep_results(uint8_t number_bead_types) {
    for (uint8_t t = 0; t < TIMESTEPS; t++) {
        uint32_t timestep = timesteps[t];
        std::vector<std::vector<std::vector<double>>> res;
        for (uint i = 0; i < number_bead_types; i++) {
            std::vector<std::vector<double>> v;
            for (uint j = i; j < number_bead_types; j++) {
                std::vector<double> w;
                v.push_back(w);
            }
            res.push_back(v);
        }
        results[timestep] = res;
    }
}

CoreInfo * get_core() {
    for (uint8_t i = 0; i < MAX_THREADS; i++) {
        if (cores[i].state == idle) {
            return &cores[i];
        }
    }
}

void run_calculator() {
    if (!calculator_queue.empty() && (threads.size() < MAX_THREADS)) {
        // Get the calculator from the front of the queue and place it in the vector for calculators
        // This is so it doesn't get garbage collected
        calculators.push_back(std::move(calculator_queue.front()));
        // Remove this from the queue
        calculator_queue.pop();
        uint32_t timestep = calculators.back().get_timestep();
        // Run the thread pointing to the object in running_calculators (the last element as we used push_back())
        std::thread thread = std::thread(&RDFCalculator::run, &calculators.back());
        struct timeval start;
        gettimeofday(&start, NULL);
        start_times[timestep] = start;
        // Store the thread in a map for use later
        threads[timestep] = std::move(thread);
        // Give this a core to stick to
        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);
        CoreInfo *c = get_core();
        CPU_SET(c->core_num, &cpuset);
        // Store the thread in a map so we can access it later
        core_map[timestep] = c;
        monitor_map[timestep].core = c->core_num;
        int rc = pthread_setaffinity_np(threads[timestep].native_handle(), sizeof(cpu_set_t), &cpuset);
        c->state = busy;
        if (rc != 0) {
          std::cerr << "Error calling pthread_setaffinity_np: " << rc << "\n";
        }
    }
}

void add_calculator_to_queue(RDFCalculator calculator) {
    CalculatorMonitor progress;
    progress.progress = waiting;
    progress.percent = 0.0;
    monitor_map[calculator.get_timestep()] = progress;
    calculator_queue.push(calculator);
    // Check if we can run a new RDFCalculator thread
    run_calculator();
}

RDFMessage receive_message(moodycamel::BlockingConcurrentQueue<RDFMessage> *queue) {
    RDFMessage msg;
    // while (!queue->try_dequeue(msg)) { };
    queue->wait_dequeue(msg);
    return msg;
}

void setCursorPosition(int XPos, int YPos) {
    printf("\033[%d;%dH",YPos+1,XPos+1);
}

void draw_progress() {
    uint8_t on_line = 0;
    for (std::map<uint32_t, CalculatorMonitor>::iterator i = monitor_map.begin(); i != monitor_map.end(); ++i) {
        std::string timestep = std::to_string(i->first);
        CalculatorMonitor mon = i->second;

        std::cout << "Timestep ";
        for (int c = 7; c >= 0; c--) {
            if (c > timestep.length()) {
                std::cout << " ";
            } else {
                std::cout << timestep[timestep.length() - c];
            }
        }
        std::cout << ": ";

        switch (mon.progress) {
            case waiting: {
                std::cout << "  WAIT";
                break;
            }
            case running: {
                char per[8];
                sprintf(per, "%.1f", mon.percent);
                std::string percent(per);
                for (int c = 5; c >= 0; c--) {
                    if (c > percent.length()) {
                        std::cout << " ";
                    } else {
                        std::cout << percent[percent.length() - c];
                    }
                }
                std::cout << "%";
                break;
            }
            case finished: {
                std::cout << "  DONE";
                break;
            }

        }
        std::cout << " ";

        if (mon.progress != finished && (core_map.find(i->first) != core_map.end())) {
            std::cout << "(" << (uint32_t)core_map[i->first]->core_num << ")";
        } else {
            std::cout << "   ";
        }
        std::cout << " ";

        on_line++;
        if (on_line == 5) {
            std::cout << "\n     ";
            on_line = 0;
        }
    }
}

void store_results(uint32_t timestep) {
    std::vector<std::vector<std::vector<double>>> res = results[timestep];
    for (uint8_t i = 0; i < res.size(); i++) {
        for (uint8_t j = 0; j < res.at(i).size(); j++) {
            std::vector<double> values = res.at(i).at(j);
            FILE* f = fopen(out_filepaths.at(i).at(j).c_str(), "a+");
            fprintf(f, "%u, ", timestep);
            for (std::vector<double>::iterator v = values.begin(); v != values.end(); ++v) {
                fprintf(f, "%1.10f", *v);
                if (next(v) != values.end()) {
                    fprintf(f, ", ");
                }
            }
            fprintf(f, "\n");
            fclose(f);
        }
    }
}

void calculator_finished(uint32_t timestep) {
    // Join the thread
    threads[timestep].join();
    // Store the results
    store_results(timestep);
    // Clear the thread from the map
    threads.erase(timestep);
    // Set the core to idle
    core_map[timestep]->state = idle;
    // Calculate the elapsed time
    struct timeval end, elapsedTime;
    gettimeofday(&end, NULL);
    timersub(&end, &start_times[timestep], &elapsedTime);
    double duration = (double) elapsedTime.tv_sec + (double) elapsedTime.tv_usec / 1000000.0;
    FILE* f = fopen("../rdf_data/runtimes.csv", "a+");
    fprintf(f, "%u, %1.10f\n", timestep, duration);
    fclose(f);
    // Run the next calculator in the queue
    run_calculator();
}

void handle_message(RDFMessage msg) {
    uint32_t timestep = msg.timestep;
    uint32_t core = msg.core;
    monitor_map[timestep].progress = msg.progress;
    monitor_map[timestep].percent = msg.percent;
    if (msg.progress == finished) {
        monitor_map[timestep].progress = msg.progress;
        core_map[timestep]->state = idle;
        calculator_finished(timestep);
    }
}

int main(int argc, char *argv[]) {
    // Record start time of RDF
    struct timeval start, finish, elapsedTime;
    gettimeofday(&start, NULL);
    // Set up array of core info structs
    for (uint8_t i = 0; i < MAX_THREADS; i++) {
        CoreInfo c = {i, idle};
        cores[i] = c;
    }
    // Reserve space so the array doesn't move and pointers don't change
    calculators.reserve(TIMESTEPS);
    // Clear the screen
    // Store the XY coordinates of the terminal cursor
    y = system("clear");
    x = 5;
    // Prepare filepaths for the RDF results to be output to
    out_filepaths = prep_out_filepaths(typeToName, volume_length);
    // Prepare the results map
    prep_results(number_bead_types);
    // Queue for receiving messages from threads
    moodycamel::BlockingConcurrentQueue<RDFMessage> message_queue(100);

    std::cout << "Starting RDF calculations\n";
    y++;
    std::cout << "Volume dimensions: " << volume_length << ", " << volume_length << ", " << volume_length << "\n";
    y++;
    std::cout << "JSON state directory: " << state_dir << "\n";
    y++;

    for (int time_index = 0; time_index < TIMESTEPS; time_index++) {
        uint32_t timestep = timesteps[time_index];
        int beads_added = 0;
        std::cout << "Building volume for timestep " << timestep << "                                       \n";
        y++;

        RDFCalculator calculator(volume_length, cells_per_dimension, timestep, number_density, number_bead_types, &results[timestep], &message_queue);
        Volume<double, std::vector<DPDState>> *volume = calculator.get_volume();
        std::cout << "Volume built                                                                          \n";
        y++;

        std::string filepath = state_dir + "state_" + std::to_string(timestep) + ".json";
        std::cout << "Loading JSON file at " << filepath << "                                               \n";
        y++;
        std::string json = getFileAsString(filepath);
        rapidjson::Document state;
        state.Parse(json.c_str());
        // With the parsed JSON, we need the array of beads
        rapidjson::Value beads = state["beads"].GetArray();

        for (rapidjson::Value::ConstValueIterator itr = beads.Begin(); itr != beads.End(); ++itr) {
            const rapidjson::Value& bead = *itr;
            bead_t b;
            b.id = bead["id"].GetUint();

            b.type = bead["type"].GetUint();
            // IF STATEMENT ADDED FOR VESICLE
            if (b.type == 3) {
                b.type = 1; // Ignore tail beads for now
            }
            b.pos.set(bead["x"].GetDouble(), bead["y"].GetDouble(), bead["z"].GetDouble());
            volume->add_bead(&b);
            beads_added++;
        }
        std::cout << beads_added << " beads added.                                                          \n";
        y++;
        // Add the volume to a queue, the thread management is done separately
        add_calculator_to_queue(calculator);
    }

    std::cout << "All volumes built. Now monitoring threads                                                  \n";
    y++;

    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    if (y > w.ws_row) {
        y = w.ws_row;
    }

    while (threads.size() > 0) {
        RDFMessage msg = receive_message(&message_queue);
        handle_message(msg);

        while (y >= (w.ws_row - ceil(TIMESTEPS/5))) {
            std::cout << "\n";
            y--;
        }
        setCursorPosition(x, y);
        draw_progress();
    }

    // Get the finish time
    gettimeofday(&finish, NULL);
    timersub(&finish, &start, &elapsedTime);
    double duration = (double) elapsedTime.tv_sec + (double) elapsedTime.tv_usec / 1000000.0;
    FILE* f = fopen("../rdf_data/runtimes.csv", "a+");
    fprintf(f, "Total runtime, %1.10f\n", duration);
    std::cout << "\n";
    return 0;
}
