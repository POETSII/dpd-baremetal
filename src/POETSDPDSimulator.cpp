// Implementation file for the host simulation volume class

#include "POETSDPDSimulator.hpp"

#ifndef __POETSDPDSIMULATOR_IMPL
#define __POETSDPDSIMULATOR_IMPL

#include "SimVolume.cpp"

POETSDPDSimulator::POETSDPDSimulator(const ptype volume_length, const unsigned cells_per_dimension, uint32_t start_timestep, uint32_t max_timestep) : DPDSimulator(volume_length, cells_per_dimension, start_timestep, max_timestep) {
    uint32_t boxesX = volume.get_boxes_x(); //TinselBoxMeshXLen;
    uint32_t boxesY = volume.get_boxes_y(); //TinselBoxMeshYLen;

    std::cout << "Acquiring Hostlink...\r";
    // Acquire Hostlink so can communicate with POETS hardware
    hostLink = new HostLink(boxesX, boxesY);

    std::cout << "HostLink acquired.     \n";

    std::cout << "Running on " << boxesX * boxesY << " box";
    if ((boxesX * boxesY) != 1) {
        std::cout << "es";
    }
    std::cout << ".\n";

#ifdef VISUALISE
    std::cout << "Preparing server for external connections...\r";
    _extern = new ExternalServer("_external.sock");
    std::cout << "External server ready.\n";
#endif

}

void POETSDPDSimulator::write() {
    ptype dt = ptype(0.0);
    ptype inv_sqrt_dt = ptype(0.0);
    #ifdef SMALL_DT_EARLY
        if (start_timestep < 1000) {
            dt = early_dt;
            inv_sqrt_dt = early_inv_sqrt_dt;
        } else {
            dt = normal_dt;
            inv_sqrt_dt = normal_inv_sqrt_dt;
        }
    #endif

    // Set first and last timestep and dt value in each cell
    unsigned cells_per_dimension = volume.get_cells_per_dimension();
    for (uint16_t x = 0; x < cells_per_dimension; x++) {
        for (uint16_t y = 0; y < cells_per_dimension; y++) {
            for (uint16_t z = 0; z < cells_per_dimension; z++) {
                // Generate location
                cell_t loc = {x, y, z};
                // Get the state of this cell
                DPDState *state = volume.get_state_of_cell(loc);

                // Set the values
                state->timestep = start_timestep;
                state->max_timestep = max_timestep;
            #ifdef SMALL_DT_EARLY
                state->dt = dt;
                state->inv_sqrt_dt = inv_sqrt_dt;
            #endif
            }
        }
    }


// Put the cells in the simulator memory
// #ifdef SERIAL
//     // The x86 serial simulator can move some things out of the DPDState
//     this->cells.setTimestep(start_timestep);
//     this->cells.setMaxTimestep(_max_timestep);
//     this->cells.setCellLength(_cell_length);
//     this->cells.setCellsPerDimension(cells_per_dimension);
// #else
    // Write the volume into the POETS system
    volume.get_cells()->write(hostLink);
}

// Run the simulation
void POETSDPDSimulator::run() {
#ifdef VISUALISE
    // Max runtime - currently only checked when a json file is closed
    runtime_hours = 0;
    runtime_minutes = 5;
    runtime_seconds = 0;

    uint16_t cells_per_dimension = volume.get_cells_per_dimension();
    uint32_t total_cells = volume.get_number_of_cells();
    uint32_t total_beads_in = volume.get_number_of_beads();

    std::cout << "Simulation run for a maximum time of ";
    if (runtime_hours > 0) {
        std::cout << runtime_hours << " hours ";
    }
    if (runtime_minutes > 0) {
        std::cout << runtime_minutes << " minutes ";
    }
    if (runtime_seconds > 0) {
        std::cout << runtime_minutes << " seconds.";
    }
    std::cout << "\n";

    calculate_runtime();

    std::cout << "This totals " << runtime_seconds << " seconds.\n";
#endif

#ifdef SERIAL
    moodycamel::BlockingConcurrentQueue<DPDMessage> queue(100);
    _volume.setQueue(&queue);
    std::thread thread(&SerialSim::run, _volume);
#else // We dont need host link if we're running serial sim on x86
    hostLink->boot("code.v", "data.v");
    hostLink->go();
#endif

    struct timeval start, finish, elapsedTime;
    gettimeofday(&start, NULL);

    std::cout << "Running...\n";

#if defined(STATS)
    politeSaveStats(hostLink, "stats.txt");
#endif

    uint32_t devices = 0;
    uint32_t timestep = start_timestep;
#ifdef BEAD_COUNTER
    uint32_t total_beads_out = 0;
#endif

#ifdef VISUALISE
    std::map<uint32_t, uint32_t> bead_print_map;
#endif

#ifdef MESSAGE_COUNTER
    std::map<cell_t, uint32_t> cell_messages;
#endif
    std::map<uint32_t, std::map<uint32_t, bead_t>> bead_map;
    bool first = true;
    // enter the main loop
    while(1) {
    #ifdef SERIAL
        // Need some way of acquiring messages from the serial x86 simulator
        DPDMessage msg = volume.receiveMessage();
    #else
        PMessage<DPDMessage> pmsg;
        hostLink->recvMsg(&pmsg, sizeof(pmsg));
        DPDMessage msg = pmsg.payload;
    #endif
        if (msg.type == 0xE0) {
            std::cout << "ERROR: A cell was too full at timestep " << msg.timestep << "\n";
            exit(1);
        }
    #ifdef TIMER
      #ifdef BEAD_COUNTER
        if (msg.type == 0xAA) {
            devices++;
            beads_out += msg.timestep;
            if (devices >= total_cells) { // All devices reported
                std::cerr << "Beads in  = " << total_beads_in << "\n";
                std::cerr << "Beads out = " << total_beads_out << "\n";
                FILE *f = fopen("../bead_count.csv", "a+");
                fprintf(f, "%u, %u, %u\n", cells_per_dimension, total_beads_in, total_beads_out);
                fclose(f);
                return;
            }
        }
      #else
        std::cerr << "Msg type = " << (uint32_t) msg.type << "\n";
        if (msg.type == 0xDD) {
            if (msg.timestep > timestep) {
                std::cerr << msg.from.x << ", "<< msg.from.y << ", " << msg.from.z;
                std::cerr << " finished early. Timestep " << msg.timestep << "\n";
                timestep = msg.timestep;
            }
        } else if (msg.type != 0xBB) {
            if (msg.timestep >= _max_timestep) {
                gettimeofday(&finish, NULL);
                timersub(&finish, &start, &elapsedTime);
                double duration = (double) elapsedTime.tv_sec + (double) elapsedTime.tv_usec / 1000000.0;
                printf("Runtime = %1.10f\n", duration);
                FILE* f = fopen("../mega_results.csv", "a+");
                // FILE* f = fopen("../timing_results.csv", "a+");
                fprintf(f, "%1.10f", duration);
                fclose(f);
            #ifdef SERIAL
                thread.join();
            #endif
                return;
            } else {
                std::cerr << "ERROR: Received finish message at early timestep: " << msg.timestep << "\n";
                return;
            }
        } else {
            std::cerr << "ERROR: received message when not expected\n";
            return;
        }
      #endif
    #elif defined(STATS)
        if (msg.type == 0xAA) {
            printf("Stat collection complete, run \"make print-stats -C ..\"\n");
            return;
        }
    #elif defined(MESSAGE_COUNTER)
        if (msg.type != 0xBB) {
            devices++;
            cell_messages[msg.from] = msg.timestep;
            if (devices >= total_cells) {
                calculateMessagesPerLink(cell_messages);
                return;
            }
        }
    #elif defined(VISUALISE)
        if (timestep < msg.timestep) {
            timestep = msg.timestep;
            bead_print_map[timestep] = 0;
            // if (timestep > start_timestep + emitperiod) {
            // #ifndef VESICLE_SELF_ASSEMBLY
            //     std::string path = "../100_bond_frames/state_" + std::to_string(timestep - emitperiod) + ".json";
            // #else
            //     std::string path = "../" + std::to_string(cells_per_dimension) + "_vesicle_frames/state_" + std::to_string(timestep - emitperiod) + ".json";
            // #endif
            //     FILE* old_file = fopen(path.c_str(), "a+");
            //     fprintf(old_file, "\n]}\n");
            //     fclose(old_file);
            // }

        #ifndef VESICLE_SELF_ASSEMBLY
            std::string fpath = "../100_bond_frames/state_" + std::to_string(timestep) + ".json";
        #else
            std::string fpath = "../" + std::to_string(cells_per_dimension) + "_vesicle_frames/state_" + std::to_string(timestep) + ".json";
        #endif
            FILE* f = fopen(fpath.c_str(), "w+");
            fprintf(f, "{\n\t\"beads\":[\n");
            fclose(f);
            first = true;
        }
        // pts_to_extern_t eMsg;
        // eMsg.timestep = msg.timestep;
        // eMsg.from = msg.from;
        // eMsg.bead = msg.beads[0];
        // _extern->send(&eMsg);


        // if (msg.timestep >= _max_timestep + 100) {
        //     std::cout << "\n";
        //     std::cout << "Finished, saving now\n";
        //     for (std::map<uint32_t, std::map<uint32_t, bead_t>>::iterator i = bead_map.begin(); i != bead_map.end(); ++i) {
        //         std::cout << "Timestep " << i->first << "\r";
        //         fflush(stdout);
        //         std::string path = "../100_bond_frames/state_" + std::to_string(i->first) + ".json";
        //         FILE* f = fopen(path.c_str(), "w+");
        //         fprintf(f, "{\n\t\"beads\":[\n");
        //         bool first = true;
        //         for (std::map<uint32_t, bead_t>::iterator j = i->second.begin(); j != i->second.end(); ++j){
        //             if (first) {
        //                 first = false;
        //             } else {
        //                 fprintf(f, ",\n");
        //             }
        //             fprintf(f, "\t\t{\"id\":%u, \"x\":%f, \"y\":%f, \"z\":%f, \"vx\":%f, \"vy\":%f, \"vz\":%f, \"type\":%u}", j->second.id, j->second.pos.x(), j->second.pos.y(), j->second.pos.z(), j->second.velo.x(), j->second.velo.y(), j->second.velo.z(), j->second.type);
        //         }
        //         fprintf(f, "\n]}");
        //         fclose(f);
        //     }
        //     std::cout << "\n";
        // #ifdef SERIAL
        //     thread.join();
        // #endif
        //     return;
        // }

        bead_t b = msg.beads[0];
        b.pos.x(b.pos.x() + msg.from.x);
        b.pos.y(b.pos.y() + msg.from.y);
        b.pos.z(b.pos.z() + msg.from.z);
        // bead_map[msg.timestep][msg.beads[0].id] = b;
    #ifndef VESICLE_SELF_ASSEMBLY
        std::string path = "../100_bond_frames/state_" + std::to_string(msg.timestep) + ".json";
    #else
        std::string path = "../" + std::to_string(cells_per_dimension) + "_vesicle_frames/state_" + std::to_string(msg.timestep) + ".json";
    #endif
        FILE* f = fopen(path.c_str(), "a+");
        if (first) {
            first = false;
        } else {
            fprintf(f, ",\n");
        }
        fprintf(f, "\t\t{\"id\":%u, \"x\":%f, \"y\":%f, \"z\":%f, \"vx\":%f, \"vy\":%f, \"vz\":%f, \"type\":%u}", b.id, b.pos.x(), b.pos.y(), b.pos.z(), b.velo.x(), b.velo.y(), b.velo.z(), b.type);
        fclose(f);
        bead_print_map[msg.timestep]++;
        if (bead_print_map[msg.timestep] >= total_beads_in) {
          #ifndef VESICLE_SELF_ASSEMBLY
            std::string path = "../100_bond_frames/state_" + std::to_string(msg.timestep) + ".json";
          #else
            std::string path = "../" + std::to_string(cells_per_dimension) + "_vesicle_frames/state_" + std::to_string(msg.timestep) + ".json";
          #endif
            FILE* old_file = fopen(path.c_str(), "a+");
            fprintf(old_file, "\n]}\n");
            fclose(old_file);

            // Check if we've run for longer than the max time
            gettimeofday(&finish, NULL);
            timersub(&finish, &start, &elapsedTime);
            double duration = (double) elapsedTime.tv_sec + (double) elapsedTime.tv_usec / 1000000.0;
            printf("Timestep %u stored after %1.10f seconds                     \r", timestep,  duration);
            fflush(stdout);
            if (duration >= runtime_seconds) {
                std::cout << "\nMax runtime reached, exiting\n";
            #ifdef VESICLE_SELF_ASSEMBLY
                FILE* timeFile = fopen("../vesicle_total_run_time.csv", "a+");
            #else
                FILE* timeFile = fopen("../oil_water_total_run_time.csv", "a+");
            #endif
                fprintf(timeFile, "%u, %u, %f\n", start_timestep, msg.timestep, duration);
                return;
            }
        }

    #endif
    }
}

//Runs a test, gets the bead outputs and returns this to the test file
void POETSDPDSimulator::test(void *result) {
    std::map<uint32_t, DPDMessage> *res = (std::map<uint32_t, DPDMessage> *)result;
    uint32_t total_cells = volume.get_number_of_cells();
    uint32_t total_beads_in = volume.get_number_of_beads();
    // Finish counter
    uint32_t finish = 0;
#ifdef SERIAL
    moodycamel::BlockingConcurrentQueue<DPDMessage> queue(100);
    _volume.setQueue(&queue);
    std::thread thread(&SerialSim::run, _volume);
#else // We dont need host link if we're running serial sim on x86
    hostLink->boot("code.v", "data.v");
    hostLink->go();
#endif

    // enter the main loop
    while(1) {
    #ifdef SERIAL
        // Need some way of acquiring messages from the serial x86 simulator
        DPDMessage msg = _volume.receiveMessage();
    #else
        PMessage<DPDMessage> pmsg;
        hostLink->recvMsg(&pmsg, sizeof(pmsg));
        DPDMessage msg = pmsg.payload;
    #endif
        if (msg.type == 0xE0) {
            std::cout << "ERROR: A cell was too full at timestep " << msg.timestep << "\n";
            exit(1);
        }
        (*res)[msg.beads[0].id] = msg;
        if (msg.type == 0xAA) {
            finish++;
            if (finish >= total_cells && res->size() >= total_beads_in) {
            #ifdef SERIAL
                thread.join();
            #endif
                return;
            }
        }
    }

}

#endif /* __POETSDPDSIMULATOR_IMPL */
