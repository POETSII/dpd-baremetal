#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <sys/time.h>
#include <HostLink.h>

#define HALF_HALF

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
    int w1 = 0.5 * total_beads;
    int w2 = 0.5 * total_beads;

    uint32_t b_uid = 0;
    for(int i=0; i<w1; i++) {
        bool added = false;
        while(!added) {
            bead_t *b1 = new bead_t;
            b1->id = b_uid++;
            b1->type = 0;
            b1->pos.set((rand() / (float)RAND_MAX * (problem_size / 2)), (rand() / (float)RAND_MAX * problem_size), (rand() / (float)RAND_MAX * problem_size));
            // b1->pos.set((rand() / (float)RAND_MAX * problem_size), (rand() / (float)RAND_MAX * problem_size), 0.0);
            b1->velo.set(0.0,0.0,0.0);
            if(uni.space(b1)) {
                uni.add(b1);
                added = true;
                beads_added++;
            }
        }
    }

    for(int i=0; i<w2; i++) {
        bool added = false;
        while(!added) {
            bead_t *b1 = new bead_t;
            b1->id = b_uid++;
            b1->type = 1;
            b1->pos.set((rand() / (float)RAND_MAX * ((problem_size / 2))) + (problem_size / 2), (rand() / (float)RAND_MAX * problem_size), (rand() / (float)RAND_MAX * problem_size));
            // b1->pos.set((rand() / (float)RAND_MAX * problem_size), (rand() / (float)RAND_MAX * problem_size), 0.0);
            b1->velo.set(0.0,0.0,0.0);
            if(uni.space(b1)) {
                uni.add(b1);
                added = true;
                beads_added++;
            }
        }
    }

    uni.write(); // write the universe into the POETS memory

    // uni.print_occupancy();
    printf("Number of beads in simulation: %u\n", beads_added);

    printf("running...\n");
    uni.run(printBeadNum, beads_added); // start the simulation

    return 0;
}
