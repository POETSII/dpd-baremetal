#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <sys/time.h>
#include <map>
#include <math.h>
#include <random>
#include <iomanip>
#include <fstream>
#include <sstream>

#include <boost/algorithm/string.hpp>

#ifndef SERIAL
#include "POLiteSimulator.hpp"
#include "POLiteVolume.hpp"
#endif

#ifdef GALS
#include "gals.h"
#elif defined(SERIAL)
#include "SerialSimulator.hpp"
#else
#include "sync.h"
#endif

void getParameters(std::string *bead_file, std::string *expected, float *volume_length, int *cells_per_dimension, uint32_t *test_length) {
  #ifdef GALS
    printf("Testing the GALS DPD application\n");
  #else
    printf("Testing the Synchronous DPD application\n");
  #endif

#ifdef BONDS
    printf("Testing a system which includes bonded beads\n");
    *bead_file += "bonds_in_25";
    *expected += "bonds_out_25";
    *volume_length = 25;
    *cells_per_dimension = 25;
    *test_length = 10;

#elif defined(LARGE_TEST)
    printf("Testing a system with a larger volume (no bonds)\n");
    *bead_file += "in_40";
    *expected += "out_40";
    *volume_length = 40;
    *cells_per_dimension = 40;
    *test_length = 10;

#else
    printf("Testing a smaller system without bonds\n");
    *bead_file += "in_18";
    *expected += "out_18";
    *volume_length = 18;
    *cells_per_dimension = 18;
    *test_length = 10;
#endif

  #ifdef BETTER_VERLET
    printf("Using improved velocity Verlet\n");
    *expected += "_new_verlet";
  #else
    printf("Using a less accurate velocity Verlet\n");
    *expected += "_old_verlet";
  #endif

  #ifdef SMALL_DT_EARLY
    printf("Using a smaller dt\n");
    *expected += "_small_dt_early";
  #endif

    *bead_file += ".csv";
    *expected += ".csv";
}

int main() {

    std::string bead_file = "../tests/beads_";
    std::string expected  = "../tests/beads_";
    float volume_length = 0;
    int cells_per_dimension = 0;
    uint32_t test_length = 0;

    getParameters(&bead_file, &expected, &volume_length, &cells_per_dimension, &test_length);

    printf("Volume dimensions: %f, %f, %f\n", volume_length, volume_length, volume_length);

    // Start at timestep 0, run until test_length timestep is reached
  #ifndef SERIAL
    POLiteSimulator simulator(volume_length, cells_per_dimension, 0, test_length, "");
    POLiteVolume *volume = simulator.get_volume();
  #else
    SerialSimulator simulator(volume_length, cells_per_dimension, 0, test_length, "");
    SerialVolume *volume = simulator.get_volume();
  #endif

    std::cerr << "Volume setup -- loading beads from " << bead_file << "\n";

    // Get the input beads from the file
    // File holding beads
    std::ifstream input_beads(bead_file);
    // Holds current line
    std::string line;
    // Loop through and add the beads to the universe
    uint32_t num_beads_in = 0;
    while(std::getline(input_beads, line)) {
        // Used for splitting line into separate parts of bead info
        std::stringstream ss(line);
        // Holds individual peices of information
        std::string s;
        // Vector holding all information used to create the bead
        std::vector<std::string> lines;
        // Loop through the line, split it into separate parts
        while (std::getline(ss, s, ',')) {
            // Remove whitespaces
            boost::trim(s);
            // Add to vector
            lines.push_back(s);
        }
        // Create the bead
        bead_t* b1 = new bead_t;
        b1->id = std::stol(lines.at(0));
        b1->type = std::stoi(lines.at(1));
        b1->pos.set(std::stof(lines.at(2)), std::stof(lines.at(3)), std::stof(lines.at(4)));
        b1->velo.set(0.0, 0.0, 0.0);
    #ifdef BETTER_VERLET
        b1->acc.set(0.0, 0.0, 0.0);
    #endif
        // Cell for bead to go in
        cell_t cell;
        cell.x = std::stoi(lines.at(5));
        cell.y = std::stoi(lines.at(6));
        cell.z = std::stoi(lines.at(7));
        // Add it to the universe
        volume->add_bead_to_cell(b1, cell);
        // Increment the number of beads
        num_beads_in++;
    }

    // Get the expected bead positionings from the expected output file
    // Store the expected bead positions in a map of bead ID to bead information
    std::map<uint32_t, bead_t> expected_beads_map;
    std::map<uint32_t, bead_t> actual_out;

    std::ifstream expected_out(expected);
    bool expected_exists = !expected_out.fail();

    if (expected_exists) {
        // Reuse line from above
        // Loop through and add the beads to the expected output map
        while(std::getline(expected_out, line)) {
            // Used for splitting line into separate parts of bead info
            std::stringstream ss(line);
            // Holds individual peices of information
            std::string s;
            // Vector holding all information used to create the bead
            std::vector<std::string> lines;
            // Loop through the line, split it into separate parts
            while (std::getline(ss, s, ',')) {
                // Remove whitespaces
                boost::trim(s);
                // Add to vector
                lines.push_back(s);
            }
            // Cell that bead ends up in
            cell_t cell;
            cell.x = std::stoi(lines.at(5));
            cell.y = std::stoi(lines.at(6));
            cell.z = std::stoi(lines.at(7));

            // Create the bead
            bead_t b1;
            b1.id = std::stol(lines.at(0));
            b1.type = std::stoi(lines.at(1));
            b1.pos.set(std::stof(lines.at(2)) + cell.x, std::stof(lines.at(3)) + cell.y, std::stof(lines.at(4)) + cell.z);
            b1.velo.set(0.0, 0.0, 0.0);
        #ifdef BETTER_VERLET
            b1.acc.set(0.0, 0.0, 0.0);
        #endif
            // Add it to the map
            expected_beads_map[b1.id] = b1;
        }
    }

    simulator.write(); // write the universe into the POETS memory


    struct timeval start, finish, elapsedTime; // Time the test

    // Time it for interest
    gettimeofday(&start, NULL);

    printf("Running...\n");

    // Run the test and get the result
    simulator.test(&actual_out);

    // Get the finish time
    gettimeofday(&finish, NULL);
    // Calculate the elapsed time
    timersub(&finish, &start, &elapsedTime);
    double duration = (double) elapsedTime.tv_sec + (double) elapsedTime.tv_usec / 1000000.0;

    bool fail = false;

    FILE* newFile;

    if (!expected_exists) {
        newFile = fopen(expected.c_str(), "w+");
    }

    // Ensure the number of input beads is the same as the number of output beads
    if (actual_out.size() != num_beads_in) {
        std::cerr << "Number of beads input does not equal to number of beads output\n";
        std::cerr << "In: " << num_beads_in << ". Out: " << actual_out.size() << "\n";
        fail = true;
    } else {
        for (std::map<uint32_t, bead_t>::iterator i = actual_out.begin(); i!=actual_out.end(); ++i) {
            // Actual values
            bead_id_t actual_id = i->second.id;
            bead_class_t actual_type = i->second.type;
            Vector3D<float> actual_pos;
            actual_pos.set(i->second.pos.x(), i->second.pos.y(), i->second.pos.z());

            // Expected values
            bead_id_t expected_id = expected_beads_map[i->first].id;
            bead_class_t expected_type = expected_beads_map[i->first].type;
            Vector3D<float> expected_pos = expected_beads_map[i->first].pos;

            bool this_failed = false;

            if (!expected_exists) {
                cell_t actual_cell;
                actual_cell.x = floor(actual_pos.x());
                actual_cell.y = floor(actual_pos.y());
                actual_cell.z = floor(actual_pos.z());
                actual_pos.set(actual_pos.x() - actual_cell.x, actual_pos.y() - actual_cell.y, actual_pos.z() - actual_cell.z);
                fprintf(newFile, "%u, %u, %1.20f, %1.20f, %1.20f, %u, %u, %u\n", actual_id, actual_type, actual_pos.x(), actual_pos.y(), actual_pos.z(), actual_cell.x, actual_cell.y, actual_cell.z);
            } else {

                if (expected_type != actual_type) {
                    fail = true;
                    this_failed = true;
                }

                if (expected_pos.x() != actual_pos.x() || expected_pos.y() != actual_pos.y() || expected_pos.z() != actual_pos.z()) {
                    fail = true;
                    this_failed = true;
                }

                if (this_failed) {
                    std::cerr << "ID: " << actual_id << "\n";
                    std::cerr << "Type: Expected " << (uint32_t) expected_type << " Actual " << (uint32_t) actual_type << "\n";
                    printf("Position: Expected (%1.20f, %1.20f, %1.20f)\n", expected_pos.x(), expected_pos.y(), expected_pos.z());
                    printf("          Actual   (%1.20f, %1.20f, %1.20f)\n", actual_pos.x(), actual_pos.y() , actual_pos.z());
                }
            }

        }
    }
    if (!expected_exists) {
        fclose(newFile);
    }

    uint8_t exit_code = 0;

    if (expected_exists) {
        printf("TESTING HAS ");
        if (fail) {
            printf("FAILED\n");
            exit_code = 55;
        } else {
            printf("PASSED\n");
            exit_code = 0;
        }
    } else {
        printf("Expected output file did not exist, it has been created for next time\n");
    }

    printf("Runtime = %1.10f\n", duration);
    return exit_code;
}
