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

    if (argc != 2) {
        printf("Not enough arguments. Please provide simulation volume length\n");
        return 1;
    }

    float problem_size = (float) std::stoi(argv[1]);
    int N = std::stoi(argv[1]);

    printf("starting the DPD application\n");
    printf("Volume dimensions: %f, %f, %f\n", problem_size, problem_size, problem_size);

    Universe<ptype> uni(problem_size, N);

    printf("Universe setup -- adding beads\n");

    int total_beads = N*N*N * 4;
    int w = 0.6 * total_beads;
    int r = 0.3 * total_beads;
    int o = 0.1 * total_beads;

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
            }
        }
    }

    uni.write(); // write the universe into the POETS memory

    uni.print_occupancy();

    printf("running...\n");
    uni.run(); // start the simulation

    return 0;
}
