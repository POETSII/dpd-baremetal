#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <sys/time.h>
#include <HostLink.h>
#include "dpd.h"
#include "universe.hpp"
#include <map>
#include <math.h>
#include <random>
#ifdef TIMER
    #include "timer.cpp"
#endif

//! generates a random position within a given space (NxN)
Vector3D<ptype> rand2DPos(unsigned N) {
    Vector3D<ptype> t_pos;
    ptype x(rand() / (float)RAND_MAX * N);
    ptype y(rand() / (float)RAND_MAX * N);
    ptype z(0.0);
    t_pos.set(x,y,z);
    return t_pos;
}

Vector3D<ptype> randPos(unsigned N) {
    Vector3D<ptype> t_pos;
    ptype x(rand() / (float)RAND_MAX * N);
    ptype y(rand() / (float)RAND_MAX * N);
    ptype z(rand() / (float)RAND_MAX * N);
    t_pos.set(x,y,z);
    return t_pos;
}

int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("Not enough arguments. Please provide simulation volume length\n");
        return 1;
    }

    if (argc > 3) {
        printf("Too many arguments. Please provide at least: Simulation volume length\n");
        return 1;
    }

    float problem_size = (float) std::stoi(argv[1]);
    int N = std::stoi(argv[1]);
    int beads_added = 0;
    printf("starting the DPD application\n");
    printf("Volume dimensions: %f, %f, %f\n", problem_size, problem_size, problem_size);

    bool printBeadNum = false;
    if (argc == 3) {
        if (std::stoi(argv[2]) > 0) {
            printBeadNum = true;
        }
    }

    Universe<ptype> uni(problem_size, N);

    printf("Universe setup -- adding beads\n");

    int total_beads = N*N*N * 4;
    int w = 0.6 * total_beads;
    int r = 0.3 * total_beads;
    int o = 0.1 * total_beads;

    auto default_world = [&]() {

        uint32_t b_uid = 0;
        for(int i=0; i<w; i++) {
            bool added = false;
            while(!added) {
                bead_t *b1 = new bead_t;
                b1->id = b_uid++;
                b1->type = 0;
                b1->pos.set((rand() / (float)RAND_MAX * problem_size), (rand() / (float)RAND_MAX * problem_size), (rand() / (float)RAND_MAX * problem_size));
                // b1->pos.set((rand() / (float)RAND_MAX * problem_size), (rand() / (float)RAND_MAX * problem_size), 0.0);
                b1->velo.set(0.0,0.0,0.0);
                if(uni.space(b1)) {
                    uni.add(b1);
                    added = true;
                    beads_added++;
                }
            }
        }

        for(int i=0; i<r; i++) {
            bool added = false;
            while(!added) {
                bead_t *b1 = new bead_t;
                b1->id = b_uid++;
                b1->type = 1;
                b1->pos.set((rand() / (float)RAND_MAX * problem_size), (rand() / (float)RAND_MAX * problem_size), (rand() / (float)RAND_MAX * problem_size));
                // b1->pos.set((rand() / (float)RAND_MAX * problem_size), (rand() / (float)RAND_MAX * problem_size), 0.0);
                b1->velo.set(0.0,0.0,0.0);
                if(uni.space(b1)) {
                    uni.add(b1);
                    added = true;
                    beads_added++;
                }
            }
        }

        for(int i=0; i<o; i++) {
            bool added = false;
            while(!added) {
                bead_t *b1 = new bead_t;
                b1->id = b_uid++;
                b1->type = 2;
                b1->pos.set((rand() / (float)RAND_MAX * problem_size), (rand() / (float)RAND_MAX * problem_size), (rand() / (float)RAND_MAX * problem_size));
                // b1->pos.set((rand() / (float)RAND_MAX * problem_size), (rand() / (float)RAND_MAX * problem_size), 0.0);
                b1->velo.set(0.0,0.0,0.0);
                if(uni.space(b1)) {
                    uni.add(b1);
                    added = true;
                    beads_added++;
                }
            }
        }
    };

    auto bonded_world_3d=[&]()
    {
        uint32_t b_uid_bonded=0x80000000ul;
        unsigned failures=0;

        for(int i=0; i<(0.4*4*N*N*N); i+=2) {
            bool added = false;
            uint32_t bid_a=b_uid_bonded++;
            uint32_t bid_b=b_uid_bonded++;
            b_uid_bonded++; // Create a break;
            while(!added) {
                auto b1=std::make_shared<bead_t>();
                b1->id = bid_a;
                b1->type = 1;
                b1->velo.set(0.0,0.0,0.0);
                b1->pos.set((rand() / (float)RAND_MAX * problem_size), (rand() / (float)RAND_MAX * problem_size), (rand() / (float)RAND_MAX * problem_size));
                auto b2=std::make_shared<bead_t>();
                b2->id = bid_b;
                b2->type = 2;
                b2->velo.set(0.0,0.0,0.0);
                b2->pos=b1->pos+Vector3D<ptype>(0.2,0.2,0.2);

                if(uni.space(b1.get(), b2.get())) {
                    uni.add(b1.get());
                    uni.add(b2.get());
                    added = true;
                    beads_added++;
                } else {
                    fprintf(stderr, "Failed to add %u\n", failures++);
                }
            }
        }


        uint32_t b_uid = 0;
        for(int i=0; i<(0.6*4*N*N*N); i++) {
            bool added = false;
            while(!added) {
                bead_t *b1 = new bead_t;
                b1->id = b_uid++;
                b1->type = 0;
                b1->pos.set((rand() / (float)RAND_MAX * problem_size), (rand() / (float)RAND_MAX * problem_size), (rand() / (float)RAND_MAX * problem_size));
                //b1->pos.set((rand() / (float)RAND_MAX * problem_size), (rand() / (float)RAND_MAX * problem_size), 0.0);
                b1->velo.set(0.0,0.0,0.0);
                if (uni.space(b1)) {
                    uni.add(b1);
                    added = true;
                    beads_added++;
                } else {
                    fprintf(stderr, "Failed to add %u\n", failures++);
                }
            }
        }
    };

    //default_world();
    bonded_world_3d();

    uni.write(); // write the universe into the POETS memory

    // uni.print_occupancy();
    printf("Number of beads in simulation: %u\n", beads_added);

    printf("running...\n");
    uni.run(printBeadNum, beads_added); // start the simulation

    return 0;
}
