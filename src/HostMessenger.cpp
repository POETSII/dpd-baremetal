// Implementation file for Host messenger.
// This is what handles messaging coming from the simulation engine.

#include "HostMessenger.hpp"

#ifndef __HOST_MESSENGER_IMPL
#define __HOST_MESSENGER_IMPL

template<class Q>
HostMessenger<Q>::HostMessenger(Q *queue, std::string state_dir, uint32_t cells_per_dimension, uint32_t max_timestep) {
    this->queue = queue;
    this->state_dir = state_dir;
    this->cells_per_dimension = cells_per_dimension;
    this->number_of_cells = cells_per_dimension * cells_per_dimension * cells_per_dimension;
    this->max_timestep = max_timestep;
}

template<class Q>
void HostMessenger<Q>::check_error(DPDMessage msg) {
    if (msg.type == 0xE0) {
        std::cout << "ERROR: A cell was too full at timestep " << msg.timestep << "\n";
        exit(1);
    }
}

#ifdef TIMER
template<class Q>
unsigned HostMessenger<Q>::timer_message(DPDMessage msg) {
    if (msg.type == 0xDD) {
        if (msg.timestep > timestep) {
            std::cerr << msg.from.x << ", "<< msg.from.y << ", " << msg.from.z;
            std::cerr << " finished early. Timestep " << msg.timestep << "\n";
            timestep = msg.timestep;
            return 2;
        }
    } else if (msg.type != 0xBB) {
        if (msg.timestep >= max_timestep) {
            gettimeofday(&finish, NULL);
            timersub(&finish, &start, &elapsedTime);
            double duration = (double) elapsedTime.tv_sec + (double) elapsedTime.tv_usec / 1000000.0;
            printf("Runtime = %1.10f\n", duration);
            FILE* f = fopen("../mega_results.csv", "a+");
            // FILE* f = fopen("../timing_results.csv", "a+");
            fprintf(f, "%1.10f", duration);
            fclose(f);
            return 0;
        } else {
            std::cerr << "ERROR: Received finish message at early timestep: " << msg.timestep << "\n";
            return 1;
        }
    } else {
        std::cerr << "ERROR: received message when not expected\n";
        return 3;
    }
    return 3;
}
#endif

#ifdef STATS
template<class Q>
unsigned HostMessenger<Q>::stats_collection(DPDMessage msg) {
    if (msg.type == 0xAB) {
        printf("Stat collection complete, run \"make print-stats -C ..\"\n");
        return 0;
    } else {
        std::cerr << "Error, an unexpected host message was received\n";
        return 1;
    }
}
#endif

#ifdef VISUALISE
template<class Q>
void HostMessenger<Q>::emit_message(DPDMessage msg) {
    if (timestep < msg.timestep) {
        timestep = msg.timestep;
        bead_print_map[timestep] = 0;
        // if (timestep > start_timestep + emitperiod) {
        #ifndef VESICLE_SELF_ASSEMBLY
        //     std::string path = "../100_bond_frames/state_" + std::to_string(timestep - emitperiod) + ".json";
        #else
        //     std::string path = "../" + std::to_string(cells_per_dimension) + "_vesicle_frames/state_" + std::to_string(timestep - emitperiod) + ".json";
        #endif
        //     FILE* old_file = fopen(path.c_str(), "a+");
        //     fprintf(old_file, "\n]}\n");
        //     fclose(old_file);
        // }

        std::string fpath = this->state_dir + "state_" + std::to_string(timestep) + ".json";
        FILE* f = fopen(fpath.c_str(), "w+");
        fprintf(f, "{\n\t\"beads\":[\n");
        fclose(f);
        this->first = true;
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
    #ifdef SERIAL
    //     thread.join();
    #endif
    //     return;
    // }

    bead_t b = msg.beads[0];
    b.pos.x(b.pos.x() + msg.from.x);
    b.pos.y(b.pos.y() + msg.from.y);
    b.pos.z(b.pos.z() + msg.from.z);

    std::string path = state_dir + "state_" + std::to_string(msg.timestep) + ".json";
    FILE* f = fopen(path.c_str(), "a+");
    if (first) {
        this->first = false;
    } else {
        fprintf(f, ",\n");
    }
    fprintf(f, "\t\t{\"id\":%u, \"x\":%f, \"y\":%f, \"z\":%f, \"vx\":%f, \"vy\":%f, \"vz\":%f, \"type\":%u}", b.id, b.pos.x(), b.pos.y(), b.pos.z(), b.velo.x(), b.velo.y(), b.velo.z(), b.type);
    fclose(f);
    bead_print_map[msg.timestep]++;
    if (bead_print_map[msg.timestep] >= number_of_beads) {
        std::string path = state_dir + "state_" + std::to_string(msg.timestep) + ".json";
        FILE* old_file = fopen(path.c_str(), "a+");
        fprintf(old_file, "\n]}\n");
        fclose(old_file);

        // Check if we've run for longer than the max time
        gettimeofday(&finish, NULL);
        timersub(&finish, &start, &elapsedTime);
        double duration = (double) elapsedTime.tv_sec + (double) elapsedTime.tv_usec / 1000000.0;
        printf("Timestep %u stored after %1.10f seconds                     \r", timestep,  duration);
        fflush(stdout);
    }
    return;
}
#endif

template<class Q>
void HostMessenger<Q>::run() {

    std::cout << "Entered host messenger\n";
    // Get the start time
    gettimeofday(&start, NULL);
    // This function will be run after the simulation has been started

    // enter the main loop
    while(1) {
        std::cout << "Awaiting message\n";
        DPDMessage msg = await_message();

        // Check if there's an error
        check_error(msg);

    // Performance simulations
    #ifdef TIMER
        // We expect the first message received to be when the simulation has
        // finished. If anything is returned earlier it's an error.
        unsigned val = timer_message(msg);
        if (!val) {
            return;
        } else {
            exit(val);
        }
    #elif defined(STATS)
    // POLite stat collection.
        unsigned val = stats_collection(msg);
        if (!val) {
            return;
        } else {
            exit(val);
        }
    #elif defined(VISUALISE)
        emit_message(msg);
    #endif

    }
}

#endif //__HOST_MESSENGER_IMPL
