#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <sys/time.h>
#include <HostLink.h>
#ifdef GALS
#include "gals.h"
#elif defined(SERIAL)
#include "serial.hpp"
#else
#include "sync.h"
#endif
#include "universe.hpp"
#include <map>
#include <math.h>
#include <random>

#ifndef BONDS
  float bond_r0 = 0.5;
#endif

#define BEAD_DENSITY 3

void print_help() {
    std::cerr << "POETS DPD simulator - POLite version\n";
    std::cerr << "Usage:\n";
    std::cerr << "./run <Simulation length> [--time t] [--print-number-of-beads] [--help]\n";
    std::cerr << "\n";
    std::cerr << "Simulation length       - The length of one side of the simulation volume.\n";
    std::cerr << "                          Simulation volumes are (currently) assumed to be cubes.\n";
    std::cerr << "                          This value must be 3 or larger, no string.\n";
    std::cerr << "\n";
    std::cerr << "time=t                  - Optional integer. The number of timesteps for this sumulation to run for.\n";
    std::cerr << "                        - If not provided, a default of 10000 will be used\n";
    std::cerr << "\n";
    std::cerr << "print-number-of-beads=p - Optional boolean. Used in testing.\n";
    std::cerr << "                          Print the number of beads in the simulation to the results file.\n";
    std::cerr << "\n";
    std::cerr << "help                    - Optional. Print this help information\n";
}

float randf() {
    return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

// Generate some values for initial velocity (Maxwell distribution, courtesy of Julian)
std::vector<float> maxwellDist(uint32_t n) {
    long maxPoints;

    const long m_MaxwellPointNo = 10000000;

    if(n > m_MaxwellPointNo)
        maxPoints = n;
    else
        maxPoints = m_MaxwellPointNo;

    std::vector<float> velDist(maxPoints, 0.0);

    float vel;
    float factor = 1.0;
    float dmaxPoints = static_cast<double>(maxPoints);

    for(long j = 0; j < maxPoints - 1; j++) {
        vel = sqrt(2.0 * log(dmaxPoints * factor / (dmaxPoints - factor)));
        factor = exp(0.5 * vel * vel);
        velDist.at(j) = vel;
    }

    velDist.at(maxPoints-1) = velDist.at(maxPoints-2);

    return velDist;
}

int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("Not enough arguments. Please provide simulation volume length\n");
        print_help();
        return 1;
    }

    bool printBeadNum = false;
    float problem_size = 0;
    int N = 0;
    uint32_t max_time = 10000;

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-' && argv[i][1] == '-') {
            std::string arg(argv[i]);
            if (arg == "--help") {
                print_help();
                return(0);
            } else if (boost::contains(arg, "--time")) {
                max_time = std::stoi(argv[i+1]);
                i++;
            } else if (boost::contains(arg, "--print-number-of-beads")) {
                printBeadNum = true;
            } else {
                std::cerr << "Unrecognised argument: " << arg << "\n";
                return 1;
            }
        } else if (argv[i][0] <= '9' && argv[i][0] >= '0') {
            problem_size = std::stoi(argv[i]);
            N = std::stoi(argv[i]);
        }
    }

    if (problem_size < 3) {
        std::cerr << "Length of volume is too small. Must be 3 or larger\n";
        print_help();
        return(0);
    }

    int beads_added = 0;
    printf("starting the DPD application\n");
    printf("Volume dimensions: %f, %f, %f\n", problem_size, problem_size, problem_size);

    Universe<ptype> uni(problem_size, N, 0, max_time);

    printf("Universe setup -- adding beads\n");

    // Declare temperature for simulation
    const float temp = 1.0;

    // Count how many bonds there are
    uint32_t bonds = 0;
    uint32_t b_uid_bonded = 0x80000000ul; // IDs of bonded beads are used to determine a bond connects them
    unsigned failures = 0;

    uint32_t total_beads = BEAD_DENSITY * N * N * N; // Get the total number of beads we ideally want (Volume * Bead density)

    uint32_t const bead_chain_numbers = 10; // Each chain is 10 beads, 5 water and 5 oil. Hopefully we'll see a membrane-like-thing form
    uint32_t bonded_oil_water = 0.1 * total_beads; // 10% of these are oil 1 and water chains
    // Each chain is a certain length, in this case, 10 beads.
    // To ensure we have all chains at the same length, we decrease this number until we hit the next multiple of 10
    // It is better to have fewer beads so we are less than the number density, than larger
    while ((bonded_oil_water % bead_chain_numbers) != 0) {
        bonded_oil_water--;
    }
    uint32_t w = 0.55 * total_beads; // 55% are single water beads
    uint32_t o1 = 0.25 * total_beads; // 25% are single oil 1 beads
    uint32_t o2 = 0.1 * total_beads; // 10% are single oil 2 beads

    // May lose one or two bead in conversion from float to int, so let's get the real number
    total_beads = bonded_oil_water + w + o1 + o2;

    // Generate initial velocities (Maxwell distribution, courtesy of Julian)
    std::vector<float> rvelDist = maxwellDist(total_beads); // Unused initial velocities isn't an issue really

    // Initialise the vector size to the number of beads in advance to avoid allocation later
    std::vector<Vector3D<float>> velDist(total_beads, Vector3D<float>(0.0, 0.0, 0.0));

    // Accumulate the total initial velocity to remove this later
    Vector3D<float> vcm(0.0, 0.0, 0.0);

    for(int i=0; i < total_beads; i++) {
        long index = static_cast<long>(rvelDist.size() * randf());
        float vmag  = rvelDist.at(index);

        float vtheta = acos(1.0-2.0 * randf());
        float vphi   = M_PI * randf();

        float vp_x   = vmag * sin(vtheta) * cos(vphi);
        float vp_y   = vmag * sin(vtheta) * sin(vphi);
        float vp_z   = vmag * cos(vtheta);

        vcm.x(vcm.x() + vp_x);
        vcm.y(vcm.y() + vp_y);
        vcm.z(vcm.z() + vp_z);

        velDist.at(i) = Vector3D<float>(vp_x, vp_y, vp_z); // store the components in vector for later use
    }

    // Get the total CM velocity in the 3 axes
    vcm.x(vcm.x() / total_beads);
    vcm.y(vcm.y() / total_beads);
    vcm.z(vcm.z() / total_beads);

    // remove CM velocity from bead velocities

    float vtotal = 0.0;

    for(int i=0; i < total_beads; i++) {
        velDist.at(i) = Vector3D<float>(velDist.at(i).x() - vcm.x(), velDist.at(i).y() - vcm.y(), velDist.at(i).z() - vcm.z());

        vtotal = vtotal + velDist.at(i).x() * velDist.at(i).x() + velDist.at(i).y() * velDist.at(i).y() + velDist.at(i).z() * velDist.at(i).z();
    }

    vtotal = sqrt(vtotal / static_cast<double>(3 * total_beads)); // 3 for axis of movement

    // finally normalize the velocities to the required temperature,
    for(int i = 0; i < total_beads; i++)
    {
        velDist.at(i).x(sqrt(temp) * velDist.at(i).x() / vtotal);
        velDist.at(i).y(sqrt(temp) * velDist.at(i).y() / vtotal);
        velDist.at(i).z(sqrt(temp) * velDist.at(i).z() / vtotal);
    }

    FILE* f = fopen("../25_bond_frames/state_0.json", "w+");
    fprintf(f, "{\n\t\"beads\":[\n");

    for(int i = 0; i < bonded_oil_water; i += bead_chain_numbers) {
        bool added = false;
        auto prev_bead = std::make_shared<bead_t>();
        prev_bead->id = b_uid_bonded++;
        while(!added) {
            prev_bead->type = 0; // First bead is  water
            // prev_bead->type = 1;
            prev_bead->pos.set((rand() / (float)RAND_MAX * problem_size), (rand() / (float)RAND_MAX * problem_size), (rand() / (float)RAND_MAX * problem_size));
            prev_bead->velo.set(velDist.at(beads_added).x(), velDist.at(beads_added).y(), velDist.at(beads_added).z());
        #ifndef GALS
            prev_bead->acc.set(0.0, 0.0, 0.0);
        #elif defined(BETTER_VERLET)
            prev_bead->acc.set(0.0, 0.0, 0.0);
        #endif
            if (uni.space(prev_bead.get())) {
                uni.add(prev_bead.get());
                fprintf(f, "\t\t{\"id\":%u, \"x\":%f, \"y\":%f, \"z\":%f, \"vx\":%f, \"vy\":%f, \"vz\":%f, \"type\":%u},\n", prev_bead->id, prev_bead->pos.x(), prev_bead->pos.y(), prev_bead->pos.z(), prev_bead->velo.x(), prev_bead->velo.y(), prev_bead->velo.z(), prev_bead->type);
                added = true;
                beads_added++;
            } else {
            }
        }
        for (int j = 1; j < bead_chain_numbers; j++) {
            bool added = false;
            uint32_t bid_a=b_uid_bonded++;
            while(!added) {
                auto b1=std::make_shared<bead_t>();
                b1->id = bid_a;
                if (j > 4) {
                    b1->type = 1;
                } else {
                    b1->type = 0;
                }
                bool fine = false;
                while (!fine) {
                    b1->pos.set(((rand() / (float)RAND_MAX) - bond_r0) + prev_bead.get()->pos.x(), ((rand() / (float)RAND_MAX) - bond_r0) + prev_bead.get()->pos.y(), ((rand() / (float)RAND_MAX) - bond_r0) + prev_bead.get()->pos.z());
                    ptype dist = prev_bead->pos.dist(b1->pos);
                    if (dist <= 1.0) {
                        fine = true;
                        if (b1->pos.x() > problem_size) {
                            b1->pos.x(b1->pos.x() - problem_size);
                        } else if (b1->pos.x() < 0) {
                            b1->pos.x(b1->pos.x() + problem_size);
                        }
                        if (b1->pos.y() > problem_size) {
                            b1->pos.y(b1->pos.y() - problem_size);
                        } else if (b1->pos.y() < 0) {
                            b1->pos.y(b1->pos.y() + problem_size);
                        }
                        if (b1->pos.z() > problem_size) {
                            b1->pos.z(b1->pos.z() - problem_size);
                        } else if (b1->pos.z() < 0) {
                            b1->pos.z(b1->pos.z() + problem_size);
                        }
                    }
                }
                b1->velo.set(velDist.at(beads_added).x(), velDist.at(beads_added).y(), velDist.at(beads_added).z());
            #ifndef GALS
                b1->acc.set(0.0, 0.0, 0.0);
            #elif defined(BETTER_VERLET)
                b1->acc.set(0.0, 0.0, 0.0);
            #endif
                if(uni.space(b1.get())) {
                    uni.add(b1.get());
                    fprintf(f, "\t\t{\"id\":%u, \"x\":%f, \"y\":%f, \"z\":%f, \"vx\":%f, \"vy\":%f, \"vz\":%f, \"type\":%u},\n", b1->id, b1->pos.x(), b1->pos.y(), b1->pos.z(), b1->velo.x(), b1->velo.y(), b1->velo.z(), b1->type);
                    added = true;
                    prev_bead = b1;
                    beads_added++;
                    bonds++;
                }
            }
        }
        b_uid_bonded++; // Create a break;
    }

    uint32_t b_uid = 0; // Reset bead IDs
    // Add water beads
    for(int i = 0; i < w; i++) {
        bool added = false;
        while(!added) {
            bead_t *b1 = new bead_t;
            b1->id = b_uid++;
            b1->type = 0;
            b1->pos.set((rand() / (float)RAND_MAX * problem_size), (rand() / (float)RAND_MAX * problem_size), (rand() / (float)RAND_MAX * problem_size));
            b1->velo.set(velDist.at(beads_added).x(), velDist.at(beads_added).y(), velDist.at(beads_added).z());
        #ifndef GALS
            b1->acc.set(0.0, 0.0, 0.0);
        #elif defined(BETTER_VERLET)
            b1->acc.set(0.0, 0.0, 0.0);
        #endif
            if (uni.space(b1)) {
                uni.add(b1);
                fprintf(f, "\t\t{\"id\":%u, \"x\":%f, \"y\":%f, \"z\":%f, \"vx\":%f, \"vy\":%f, \"vz\":%f, \"type\":%u},\n", b1->id, b1->pos.x(), b1->pos.y(), b1->pos.z(), b1->velo.x(), b1->velo.y(), b1->velo.z(), b1->type);
                added = true;
                beads_added++;
            }
        }
    }

    // Add oil 1 beads
    for(int i = 0; i < o1; i++) {
        bool added = false;
        while(!added) {
            bead_t *b1 = new bead_t;
            b1->id = b_uid++;
            b1->type = 1;
            b1->pos.set((rand() / (float)RAND_MAX * problem_size), (rand() / (float)RAND_MAX * problem_size), (rand() / (float)RAND_MAX * problem_size));
            b1->velo.set(velDist.at(beads_added).x(), velDist.at(beads_added).y(), velDist.at(beads_added).z());
        #ifndef GALS
            b1->acc.set(0.0, 0.0, 0.0);
        #elif defined(BETTER_VERLET)
            b1->acc.set(0.0, 0.0, 0.0);
        #endif
            if (uni.space(b1)) {
                uni.add(b1);
                fprintf(f, "\t\t{\"id\":%u, \"x\":%f, \"y\":%f, \"z\":%f, \"vx\":%f, \"vy\":%f, \"vz\":%f, \"type\":%u},\n", b1->id, b1->pos.x(), b1->pos.y(), b1->pos.z(), b1->velo.x(), b1->velo.y(), b1->velo.z(), b1->type);
                added = true;
                beads_added++;
            }
        }
    }

    // Add oil 2 beads
    bool first = true;
    for(int i = 0; i < o2; i++) {
        bool added = false;
        while(!added) {
            bead_t *b1 = new bead_t;
            b1->id = b_uid++;
            b1->type = 2;
            b1->pos.set((rand() / (float)RAND_MAX * problem_size), (rand() / (float)RAND_MAX * problem_size), (rand() / (float)RAND_MAX * problem_size));
            b1->velo.set(velDist.at(beads_added).x(), velDist.at(beads_added).y(), velDist.at(beads_added).z());
        #ifndef GALS
            b1->acc.set(0.0, 0.0, 0.0);
        #elif defined(BETTER_VERLET)
            b1->acc.set(0.0, 0.0, 0.0);
        #endif
            if (uni.space(b1)) {
                uni.add(b1);
                if (first) {
                    fprintf(f, "\t\t{\"id\":%u, \"x\":%f, \"y\":%f, \"z\":%f, \"vx\":%f, \"vy\":%f, \"vz\":%f, \"type\":%u}", b1->id, b1->pos.x(), b1->pos.y(), b1->pos.z(), b1->velo.x(), b1->velo.y(), b1->velo.z(), b1->type);
                    first = false;
                } else {
                    fprintf(f, ",\n\t\t{\"id\":%u, \"x\":%f, \"y\":%f, \"z\":%f, \"vx\":%f, \"vy\":%f, \"vz\":%f, \"type\":%u}", b1->id, b1->pos.x(), b1->pos.y(), b1->pos.z(), b1->velo.x(), b1->velo.y(), b1->velo.z(), b1->type);
                }
                added = true;
                beads_added++;
            }
        }
    }

    fprintf(f, "]}");
    fclose(f);

    uni.store_initial_bead_distances();

    uni.set_beads_added(beads_added);

#ifndef SERIAL
    uni.write(); // write the universe into the POETS memory
#endif

    // uni.print_occupancy();

    printf("running...\n");
    uni.run(); // start the simulation

    return 0;
}
