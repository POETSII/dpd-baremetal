#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <sys/time.h>
#ifndef SERIAL
#include <HostLink.h>
#endif
#ifdef GALS
#include "gals.h"
#elif defined(SERIAL)
#include "SerialSimulator.hpp"
#else
#include "sync.h"
#endif
#ifndef SERIAL
#include "POLiteVolume.hpp"
#include "POLiteSimulator.hpp"
#endif
#include <map>
#include <math.h>
#include <random>
#include <typeinfo>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

// This should be defined in the Makefile, but put it in here just in case
#ifndef BONDS
  #define BONDS
#endif

#define BEAD_DENSITY 3

float randf() {
    return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

int main(int argc, char *argv[]) {

    float problem_size = 3; // Hard-coded as for this example we just want a couple of beads
    int N = 3;
    uint32_t max_time = 10000;

    int beads_added = 0;
    printf("Starting the DPD application\n");
    printf("Volume dimensions: %f, %f, %f\n", problem_size, problem_size, problem_size);
    printf("A simple example to place a pair of beads in a volume and\n");
    printf("test them migrating and interacting across corners of the volume.\n");
    printf("Options in the cpp file allow for different corners.\n");

    char cwd_buffer[PATH_MAX], *unused;
    unused = getcwd(cwd_buffer, sizeof(cwd_buffer));

  #ifndef SERIAL
    std::string state_dir = std::string(cwd_buffer) + "/../polite-dpd-states/";
  #else
    std::string state_dir = std::string(cwd_buffer) + "/../serial-dpd-states/";
  #endif

#ifndef SERIAL
    POLiteSimulator simulator(problem_size, N, 0, max_time, state_dir);
    POLiteVolume *volume = (POLiteVolume *) simulator.get_volume();
#else
    SerialSimulator simulator(problem_size, N, 0, max_time, state_dir);
    SerialVolume *volume = (SerialVolume *) simulator.get_volume();
#endif
    printf("Universe setup -- adding beads\n");

    std::string init_state_file = state_dir + "state_0.json";

    // Add the beads
    // Pair 1
    bead_t *b0 = new bead_t;
    bead_t *b1 = new bead_t;
    b0->id = 0; // Differing IDs
    b1->id = 1;
    b0->type = 0; // Water
    b1->type = 0;
    // Place both in the same cell but so one gets forced out of the corner
    b0->pos.set(2.75, 2.75, 0.25);
    b1->pos.set(2.25, 2.25, 0.75);
    // Add 0 velocities
    b0->velo.set(0.0, 0.0, 0.0);
    beads_added++;
    b1->velo.set(0.0, 0.0, 0.0);
    beads_added++;
#ifdef BETTER_VERLET
    // BETTER_VERLET has acceleration as part of bead_t so clear this
    b0->acc.set(0.0, 0.0, 0.0);
    b1->acc.set(0.0, 0.0, 0.0);
#endif

    // Add these to the volume
    FILE* f = fopen(init_state_file.c_str(), "w+");
    fprintf(f, "{\n\t\"beads\":[\n");

    cell_t u0 = volume->add_bead(b0);
    printf("Bead 0 in cell (%u, %u, %u)\n", u0.x, u0.y, u0.z);
    fprintf(f, "\t\t{\"id\":%u, \"x\":%f, \"y\":%f, \"z\":%f, \"vx\":0, \"vy\":0, \"vz\":0, \"type\":%u},\n", b0->id, b0->pos.x(), b0->pos.y(), b0->pos.z(), b0->type);
    cell_t u1 = volume->add_bead(b1);
    printf("Bead 1 in cell (%u, %u, %u)\n", u1.x, u1.y, u1.z);
    fprintf(f, "\t\t{\"id\":%u, \"x\":%f, \"y\":%f, \"z\":%f, \"vx\":0, \"vy\":0, \"vz\":0, \"type\":%u}\n", b1->id, b1->pos.x(), b1->pos.y(), b1->pos.z(), b1->type);

    // Close the file
    fprintf(f, "]}");
    fclose(f);

    simulator.write(); // write the universe into the POETS memory

    simulator.run(); // start the simulation

    return 0;
}
