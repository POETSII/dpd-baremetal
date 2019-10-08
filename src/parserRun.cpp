#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <stdint.h>
#include <string>
#include <assert.h>
#include "parser.cpp"
#include "DPDSimulation.hpp"
#include "parseUniverse.hpp"

// #include <sys/time.h>
// #include <HostLink.h>
// #include "dpd.h"
// #include "universe.hpp"
// #include <map>
// #include <math.h>
// #include <random>
// #ifdef TIMER
//     #include "timer.cpp"
// #endif



void print_help() {
    std::cerr << "POETS DPD simulator - POLite version\n";
    std::cerr << "Usage:\n";
    std::cerr << "./run <DMPCI filepath> [--help]\n";
    std::cerr << "\n";
    std::cerr << "DMPCI filepath - Path to an input DMPCI file specifying a DPD simulation.\n";
    std::cerr << "\n";
    std::cerr << "help           - Optional. Print this help information and quit.\n";
}

int main(int argc, char *argv[]) {

    if (argc < 2) {
        std::cerr << "Not enough arguments. Please provide an input filepath\n";
        print_help();
        return(1);
    }

    std::string filepath;

    for (int i = 1; i < argc; i++) {
        std::string arg(argv[i]);
        if (argv[i][0] == '-' && argv[i][1] == '-') {
            std::string arg(argv[i]);
            if (arg == "--help") {
                print_help();
                return(0);
            }
        } else {
                filepath = argv[i];
        }
    }

    DPDSimulation sim = parse(filepath);
    std::cout << "\nFile parsed. Building universe.\n\n";

    Universe<ptype> uni(sim);

    // int beads_added = 0;
    // printf("starting the DPD application\n");
    // printf("Volume dimensions: %f, %f, %f\n", problem_size, problem_size, problem_size);

    // Universe<ptype> uni(problem_size, N);

    // printf("Universe setup -- adding beads\n");

    // int total_beads = N*N*N * 4;
    // int w = 0.6 * total_beads;
    // int r = 0.3 * total_beads;
    // int o = 0.1 * total_beads;

    // auto default_3D_world = [&]() {

    //     uint32_t b_uid = 0;
    //     for(int i=0; i<w; i++) {
    //         bool added = false;
    //         while(!added) {
    //             bead_t *b1 = new bead_t;
    //             b1->id = b_uid++;
    //             b1->type = 0;
    //             b1->pos.set((rand() / (float)RAND_MAX * problem_size), (rand() / (float)RAND_MAX * problem_size), (rand() / (float)RAND_MAX * problem_size));
    //             // b1->pos.set((rand() / (float)RAND_MAX * problem_size), (rand() / (float)RAND_MAX * problem_size), 0.0);
    //             b1->velo.set(0.0,0.0,0.0);
    //             if(uni.space(b1)) {
    //                 uni.add(b1);
    //                 added = true;
    //                 beads_added++;
    //             }
    //         }
    //     }

    //     for(int i=0; i<r; i++) {
    //         bool added = false;
    //         while(!added) {
    //             bead_t *b1 = new bead_t;
    //             b1->id = b_uid++;
    //             b1->type = 1;
    //             b1->pos.set((rand() / (float)RAND_MAX * problem_size), (rand() / (float)RAND_MAX * problem_size), (rand() / (float)RAND_MAX * problem_size));
    //             // b1->pos.set((rand() / (float)RAND_MAX * problem_size), (rand() / (float)RAND_MAX * problem_size), 0.0);
    //             b1->velo.set(0.0,0.0,0.0);
    //             if(uni.space(b1)) {
    //                 uni.add(b1);
    //                 added = true;
    //                 beads_added++;
    //             }
    //         }
    //     }

    //     for(int i=0; i<o; i++) {
    //         bool added = false;
    //         while(!added) {
    //             bead_t *b1 = new bead_t;
    //             b1->id = b_uid++;
    //             b1->type = 2;
    //             b1->pos.set((rand() / (float)RAND_MAX * problem_size), (rand() / (float)RAND_MAX * problem_size), (rand() / (float)RAND_MAX * problem_size));
    //             // b1->pos.set((rand() / (float)RAND_MAX * problem_size), (rand() / (float)RAND_MAX * problem_size), 0.0);
    //             b1->velo.set(0.0,0.0,0.0);
    //             if(uni.space(b1)) {
    //                 uni.add(b1);
    //                 added = true;
    //                 beads_added++;
    //             }
    //         }
    //     }
    // };

    // uint32_t bonds = 0;
    // auto bonded_world_3D=[&]()
    // {
    //     uint32_t b_uid_bonded=0x80000000ul;
    //     unsigned failures=0;

    //     uint32_t const bead_chain_numbers = 10;

    //     for(int i=0; i<(0.1*4*N*N*N); i+=bead_chain_numbers) {
    //         bool added = false;
    //         auto prev_bead = std::make_shared<bead_t>();
    //         prev_bead->id = b_uid_bonded++;
    //         while(!added) {
    //             prev_bead->type = 1;
    //             prev_bead->velo.set(0.0, 0.0, 0.0);
    //             prev_bead->pos.set((rand() / (float)RAND_MAX * problem_size), (rand() / (float)RAND_MAX * problem_size), (rand() / (float)RAND_MAX * problem_size));
    //             if (uni.space(prev_bead.get())) {
    //                 uni.add(prev_bead.get());
    //                 added = true;
    //                 beads_added++;
    //             } else {
    //             }
    //         }
    //         for (int j = 1; j < bead_chain_numbers; j++) {
    //             bool added = false;
    //             uint32_t bid_a=b_uid_bonded++;
    //             while(!added) {
    //                 auto b1=std::make_shared<bead_t>();
    //                 b1->id = bid_a;
    //                 b1->type = 1;
    //                 b1->velo.set(0.0,0.0,0.0);
    //                 b1->pos.set(((rand() / (float)RAND_MAX) - 0.5) + prev_bead.get()->pos.x(), ((rand() / (float)RAND_MAX) - 0.5) + prev_bead.get()->pos.y(), ((rand() / (float)RAND_MAX) - 0.5) + prev_bead.get()->pos.z());
    //                 if(uni.space(b1.get())) {
    //                     uni.add(b1.get());
    //                     added = true;
    //                     prev_bead = b1;
    //                     beads_added++;
    //                     bonds++;
    //                 }
    //             }
    //         }
    //         b_uid_bonded++; // Create a break;
    //     }


    //     uint32_t b_uid = 0;
    //     for(int i=0; i<(0.9*4*N*N*N); i++) {
    //         bool added = false;
    //         while(!added) {
    //             bead_t *b1 = new bead_t;
    //             b1->id = b_uid++;
    //             b1->type = 0;
    //             b1->pos.set((rand() / (float)RAND_MAX * problem_size), (rand() / (float)RAND_MAX * problem_size), (rand() / (float)RAND_MAX * problem_size));
    //             //b1->pos.set((rand() / (float)RAND_MAX * problem_size), (rand() / (float)RAND_MAX * problem_size), 0.0);
    //             b1->velo.set(0.0,0.0,0.0);
    //             if (uni.space(b1)) {
    //                 uni.add(b1);
    //                 added = true;
    //                 beads_added++;
    //             }
    //         }
    //     }
    // };

    // if (!include_bonds) {
    //     default_3D_world();
    // } else {
    //     bonded_world_3D();
    // }

    // uni.write(); // write the universe into the POETS memory

    // // uni.print_occupancy();

    // printf("running...\n");
    // uni.run(printBeadNum, beads_added); // start the simulation

    return 0;
}
