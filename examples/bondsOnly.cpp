#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <sys/time.h>
#include <HostLink.h>
#ifndef GALS
#include "dpd.h"
#else
#include "dpdGALS.h"
#endif
#include "universe.hpp"
#include <map>
#include <math.h>
#include <random>

// This should be defined in the Makefile, but put it in here just in case
#ifndef BONDS
  #define BONDS
#endif

#define BEAD_DENSITY 3

void print_help() {
    std::cerr << "POETS DPD simulator - POLite version\n";
    std::cerr << "Usage:\n";
    std::cerr << "./run [--time t] [--help]\n";
    std::cerr << "\n";
    std::cerr << "time=t                  - Optional integer. The number of timesteps for this sumulation to run for.\n";
    std::cerr << "                        - If not provided, a default of 10000 will be used\n";
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

    float problem_size = 25; // Hard-coded as for this example we just want a few bonds, nothing special
    int N = 25;
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
            } else {
                std::cerr << "Unrecognised argument: " << arg << "\n";
                return 1;
            }
        }
    }

    int beads_added = 0;
    printf("\nStarting the DPD application\n");
    printf("Volume dimensions: %f, %f, %f\n", problem_size, problem_size, problem_size);
    printf("A simple example to place 3 pairs of bonded beads in a volume.\n");
    printf("Each has a different bond placement, and tests something that could be a problem: \n");
    printf("\t1. Both beads, and thus the bond, within the same cell.\n");
    printf("\t   Does a bond correctly hold 2 beads when both are within a cell?\n");
    printf("\t2. Beads in different neighbouring cells, the bond crosses a cell boundary.\n");
    printf("\t   If the bond crosses a cell boundary, does the bond still work correctly?\n");
    printf("\t3. Beads at extremes of position value, one at each side of the perceived volume.\n");
    printf("\t   If the bond crosses the wrapped volume edges, does the bond still work?\n\n");

    Universe<ptype> uni(problem_size, N, max_time);

    printf("Universe setup -- adding beads\n");

    // Declare temperature for simulation
    const float temp = 1.0;

    // Count how many bonds there are
    uint32_t bonds = 0;
    uint32_t b_uid_bonded = 0x80000000ul; // IDs of bonded beads are used to determine a bond connects them

    uint32_t total_beads = 6; // 3 pairs bonded beads

    uint32_t const bead_chain_numbers = 10; // Each chain is 10 beads, 5 water and 5 oil. Hopefully we'll see a membrane-like-thing form

    // Generate initial velocities (Maxwell distribution, courtesy of Julian)
    std::vector<float> rvelDist = maxwellDist(total_beads); // Have to add a few more as we may end up having a few more beads. Unused initial velocities isn't an issue really
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


    vtotal = sqrt(vtotal / static_cast<double>(total_beads));


    // finally normalize the velocities to the required temperature,
    for(int i = 0; i < total_beads; i++)
    {
        velDist.at(i).x(sqrt(temp) * velDist.at(i).x() / vtotal);
        velDist.at(i).y(sqrt(temp) * velDist.at(i).y() / vtotal);
        velDist.at(i).z(sqrt(temp) * velDist.at(i).z() / vtotal);
    }


    // Add the beads
    // Pair 1
    bead_t *b0 = new bead_t;
    bead_t *b1 = new bead_t;
    b0->id = b_uid_bonded++; // Has an ID which identifies it as bonded
    b1->id = b_uid_bonded++;
    b0->type = 0; // Water
    b1->type = 0;
    b0->pos.set(2.5, 2.1, 2.5); // Place both in the same cell but at 0.5 Euclidean distance so the bond is at standard length
    b1->pos.set(2.5, 2.6, 2.5);
    // b0->velo.set(velDist.at(beads_added).x(), velDist.at(beads_added).y(), velDist.at(beads_added).z());
    // Add 0 velocities
    b0->velo.set(0.0, 0.0, 0.0);
    beads_added++;
    // b1->velo.set(velDist.at(beads_added).x(), velDist.at(beads_added).y(), velDist.at(beads_added).z());
    b1->velo.set(0.0, 0.0, 0.0);
    beads_added++;
#ifdef BETTER_VERLET
    // BETTER_VERLET has acceleration as part of bead_t so clear this
    b0->acc.set(0.0, 0.0, 0.0);
    b1->acc.set(0.0, 0.0, 0.0);
#endif
    // Add these to the volume

    FILE* f = fopen("../25_bond_frames/state_0.json", "w+");
    fprintf(f, "{\n\t\"beads\":[\n");

    unit_t u0 = uni.add(b0);
    printf("Bead 0 in cell (%u, %u, %u)\n", u0.x, u0.y, u0.z);
    fprintf(f, "\t\t{\"id\":%u, \"x\":%f, \"y\":%f, \"z\":%f, \"vx\":0, \"vy\":0, \"vz\":0, \"type\":%u},\n", b0->id, b0->pos.x(), b0->pos.y(), b0->pos.z(), b0->type);
    unit_t u1 = uni.add(b1);
    printf("Bead 1 in cell (%u, %u, %u)\n", u1.x, u1.y, u1.z);
    fprintf(f, "\t\t{\"id\":%u, \"x\":%f, \"y\":%f, \"z\":%f, \"vx\":0, \"vy\":0, \"vz\":0, \"type\":%u},\n", b1->id, b1->pos.x(), b1->pos.y(), b1->pos.z(), b1->type);
    double dist1 = b0->pos.dist(b1->pos);
    printf("Dist = %f\n\n", dist1);
    b_uid_bonded++; // Create a break;


    // Pair 2
    bead_t *b2 = new bead_t;
    bead_t *b3 = new bead_t;
    b2->id = b_uid_bonded++; // Has an ID which identifies it as bonded
    b3->id = b_uid_bonded++;
    b2->type = 0; // Water
    b3->type = 0;
    b2->pos.set(10.5, 10.5, 10.75); // Place in neighbouring cells but at 0.5 Euclidean distance so the bond is at standard length
    b3->pos.set(10.5, 10.5, 11.25);
    // b2->velo.set(velDist.at(beads_added).x(), velDist.at(beads_added).y(), velDist.at(beads_added).z());
    // Add 0 velocities
    b2->velo.set(0.0, 0.0, 0.0);
    beads_added++;
    // b3->velo.set(velDist.at(beads_added).x(), velDist.at(beads_added).y(), velDist.at(beads_added).z());
    b3->velo.set(0.0, 0.0, 0.0);
    beads_added++;
#ifdef BETTER_VERLET
    // BETTER_VERLET has acceleration as part of bead_t so clear this
    b2->acc.set(0.0, 0.0, 0.0);
    b3->acc.set(0.0, 0.0, 0.0);
#endif
    // Add these to the volume
    unit_t u2 = uni.add(b2);
    printf("Bead 2 in cell (%u, %u, %u)\n", u2.x, u2.y, u2.z);
    fprintf(f, "\t\t{\"id\":%u, \"x\":%f, \"y\":%f, \"z\":%f, \"vx\":0, \"vy\":0, \"vz\":0, \"type\":%u},\n", b2->id, b2->pos.x(), b2->pos.y(), b2->pos.z(), b2->type);
    unit_t u3 = uni.add(b3);
    printf("Bead 3 in cell (%u, %u, %u)\n", u3.x, u3.y, u3.z);
    fprintf(f, "\t\t{\"id\":%u, \"x\":%f, \"y\":%f, \"z\":%f, \"vx\":0, \"vy\":0, \"vz\":0, \"type\":%u},\n", b3->id, b3->pos.x(), b3->pos.y(), b3->pos.z(), b3->type);
    double dist2 = b2->pos.dist(b3->pos);
    printf("Dist = %f\n\n", dist2);
    b_uid_bonded++; // Create a break;


    // Pair 3
    bead_t *b4 = new bead_t;
    bead_t *b5 = new bead_t;
    b4->id = b_uid_bonded++; // Has an ID which identifies it as bonded
    b5->id = b_uid_bonded++;
    b4->type = 0; // Water
    b5->type = 0;
    b4->pos.set(0.25, 0.5, 0.5); // Place in neighbouring cells but at 0.5 Euclidean distance so the bond is at standard length
    b5->pos.set(24.75, 0.5, 0.5);
    // b4->velo.set(velDist.at(beads_added).x(), velDist.at(beads_added).y(), velDist.at(beads_added).z());
    // Add 0 velocities
    b4->velo.set(0.0, 0.0, 0.0);
    beads_added++;
    // b5->velo.set(velDist.at(beads_added).x(), velDist.at(beads_added).y(), velDist.at(beads_added).z());
    b5->velo.set(0.0, 0.0, 0.0);
    beads_added++;
#ifdef BETTER_VERLET
    // BETTER_VERLET has acceleration as part of bead_t so clear this
    b4->acc.set(0.0, 0.0, 0.0);
    b5->acc.set(0.0, 0.0, 0.0);
#endif
    // Add these to the volume
    unit_t u4 = uni.add(b4);
    printf("Bead 2 in cell (%u, %u, %u)\n", u4.x, u4.y, u4.z);
    fprintf(f, "\t\t{\"id\":%u, \"x\":%f, \"y\":%f, \"z\":%f, \"vx\":0, \"vy\":0, \"vz\":0, \"type\":%u},\n", b4->id, b4->pos.x(), b4->pos.y(), b4->pos.z(), b4->type);
    unit_t u5 = uni.add(b5);
    printf("Bead 3 in cell (%u, %u, %u)\n", u5.x, u5.y, u5.z);
    fprintf(f, "\t\t{\"id\":%u, \"x\":%f, \"y\":%f, \"z\":%f, \"vx\":0, \"vy\":0, \"vz\":0, \"type\":%u}\n", b5->id, b5->pos.x(), b5->pos.y(), b5->pos.z(), b5->type);
    double dist3 = b4->pos.dist(b5->pos);
    Vector3D<float> pos4;
    pos4.set(25.0, 0.0, 0.0);
    pos4 = b4->pos + pos4;
    Vector3D<float> pos5 = b4->pos;
    pos5.set(25.0, 0.0, 0.0);
    pos5 = b5->pos - pos5;
    double dist4 = b4->pos.dist(pos5);
    double dist5 = b5->pos.dist(pos4);
    printf("Dist = %f\n", dist3);
    printf("Dist = %f\n", dist4);
    printf("Dist = %f\n\n", dist5);
    b_uid_bonded++; // Create a break;

    fprintf(f, "]}");
    fclose(f);

    uni.set_beads_added(beads_added);

    uni.write(); // write the universe into the POETS memory

    // uni.print_occupancy();

    printf("running...\n");
    uni.run(max_time + 10); // start the simulation

    return 0;
}
