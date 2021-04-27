#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h>
#include <math.h>
#include <random>

#include <boost/algorithm/string.hpp>

#ifndef SERIAL
#include "POLiteSimulator.hpp"
#else
#include "SerialSimulator.hpp"
#endif

#ifndef BONDS
  float bond_r0 = 0.3;
#endif

#define BEAD_DENSITY 3

void print_help() {
    std::cerr << "POETS DPD simulator - POLite version\n";
    std::cerr << "Usage:\n";
    std::cerr << "./run <Volume length> [--time t] ";
#ifndef SERIAL
    std::cerr << "[--boxes-x x] [--boxes-y y] ";
#endif
    std::cerr << "[--help]\n";
    std::cerr << "\n";
    std::cerr << "Simulation length       - The length of one side of the simulation volume.\n";
    std::cerr << "                          Simulation volumes are (currently) assumed to be cubes.\n";
    std::cerr << "                          This value must be 3 or larger, no string.\n";
    std::cerr << "\n";
    std::cerr << "time t                  - Optional integer. The number of timesteps for this sumulation to run for.\n";
    std::cerr << "                        - If not provided, a default of 10000 will be used\n";
    std::cerr << "\n";
#ifndef SERIAL
    std::cerr << "boxes-x x               - Optional integer. The number of POETS Boxes to use in the X dimension.\n";
    std::cerr << "                        - The maximum currently is 2\n";
    std::cerr << "                        - If not provided, a default of 1 will be used\n";
    std::cerr << "boxes-y y               - Optional integer. The number of POETS Boxes to use in the Y dimension.\n";
    std::cerr << "                        - The maximum currently is 4\n";
    std::cerr << "                        - If not provided, a default of 1 will be used\n";
    std::cerr << "\n";
#endif
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

    float problem_size = 0;
    int N = 0;
    uint32_t max_time = 10000;

#ifndef SERIAL
    uint32_t boxes_x = 1;
    uint32_t boxes_y = 1;
#endif

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-' && argv[i][1] == '-') {
            std::string arg(argv[i]);
            if (arg == "--help") {
                print_help();
                return(0);
            } else if (boost::contains(arg, "--time")) {
                max_time = std::stoi(argv[i+1]);
                i++;
        #ifndef SERIAL
            } else if (boost::contains(arg, "--boxes-x")) {
                boxes_x = std::stoi(argv[i+1]);
                i++;
            } else if (boost::contains(arg, "--boxes-y")) {
                boxes_y = std::stoi(argv[i+1]);
                i++;
        #endif
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

  #ifndef SERIAL
    // Default box numbers are x = 1, y = 1.
    // These can be set at run time, or hard coded.
    POLiteSimulator simulator(problem_size, N, 0, max_time, boxes_x, boxes_y);
    POLiteVolume *volume = simulator.get_volume();
  #else
    SerialSimulator simulator(problem_size, N, 0, max_time);
    SerialVolume *volume = simulator.get_volume();
  #endif

    printf("Universe setup -- adding beads\n");

    // Declare temperature for simulation
    const float temp = 1.0;

    // Count how many bonds there are
    uint32_t bonds = 0;
    uint32_t b_uid_bonded = 0x80000000ul; // IDs of bonded beads are used to determine a bond connects them
    unsigned failures = 0;

    uint32_t total_beads = BEAD_DENSITY * N * N * N; // Get the total number of beads we ideally want (Volume * Bead density)
    double alphasum = 0.0;

    float const water_fraction = 0.985;
    uint32_t const water_size = 1;
    float const lipid_fraction = 0.015;
    uint32_t const lipid_size = 9; // Each chain is 10 beads, 5 water and 5 oil. Hopefully we'll see a membrane-like-thing form

    alphasum += water_fraction * water_size;
    alphasum += lipid_fraction * lipid_size;

    uint32_t water_total = water_fraction * total_beads / alphasum;
    uint32_t lipid_total = lipid_fraction * total_beads / alphasum;

    // May lose one or two bead in conversion from float to int, so let's get the real number
    total_beads = (lipid_total * lipid_size) + (water_total * water_size);

    // Generate initial velocities (Maxwell distribution, courtesy of Julian)
    std::vector<float> rvelDist = maxwellDist(total_beads);

    // Initialise the vector size to the number of beads in advance to avoid allocation later
    std::vector<Vector3D<float>> velDist(total_beads, Vector3D<float>(0.0, 0.0, 0.0));

    // Accumulate the total initial velocity to remove this later
    Vector3D<float> vcm(0.0, 0.0, 0.0);

    for(int i=0; i < total_beads; i++) {
        long index = static_cast<long>((rvelDist.size() - 1) * randf());
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

    // std::string filepath = "../" + std::to_string(N) + "_vesicle_frames/state_0.json";
    // FILE* f = fopen(filepath.c_str(), "w+");
    // fprintf(f, "{\n\t\"beads\":[\n");

    for(int i = 0; i < lipid_total; i++) {
        bool added = false;
        auto prev_bead = std::make_shared<bead_t>();
        prev_bead->id = b_uid_bonded++;
        while(!added) {
            prev_bead->type = 0; // First bead is H
            prev_bead->pos.set((rand() / (float)RAND_MAX * problem_size), (rand() / (float)RAND_MAX * problem_size), (rand() / (float)RAND_MAX * problem_size));
            prev_bead->velo.set(velDist.at(beads_added).x(), velDist.at(beads_added).y(), velDist.at(beads_added).z());
        #ifndef GALS
            prev_bead->acc.set(0.0, 0.0, 0.0);
        #elif defined(BETTER_VERLET)
            prev_bead->acc.set(0.0, 0.0, 0.0);
        #endif
            if (volume->space_for_bead(prev_bead.get())) {
                volume->add_bead(prev_bead.get());
                // fprintf(f, "\t\t{\"id\":%u, \"x\":%f, \"y\":%f, \"z\":%f, \"vx\":%f, \"vy\":%f, \"vz\":%f, \"type\":%u},\n", prev_bead->id, prev_bead->pos.x(), prev_bead->pos.y(), prev_bead->pos.z(), prev_bead->velo.x(), prev_bead->velo.y(), prev_bead->velo.z(), prev_bead->type);
                added = true;
                beads_added++;
            }
        }
        for (int j = 1; j < lipid_size; j++) {
            bool added = false;
            uint32_t bid_a=b_uid_bonded++;
            while(!added) {
                auto b1=std::make_shared<bead_t>();
                b1->id = bid_a;
                if (j > 2 && j <= 7) {
                    b1->type = 1;
                } else if (j > 7) {
                    b1->type = 3;
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
                if(volume->space_for_bead(b1.get())) {
                    volume->add_bead(b1.get());
                    // fprintf(f, "\t\t{\"id\":%u, \"x\":%f, \"y\":%f, \"z\":%f, \"vx\":%f, \"vy\":%f, \"vz\":%f, \"type\":%u},\n", b1->id, b1->pos.x(), b1->pos.y(), b1->pos.z(), b1->velo.x(), b1->velo.y(), b1->velo.z(), b1->type);
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
    for(int i = 0; i < water_total; i++) {
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
            if (volume->space_for_bead(b1)) {
                volume->add_bead(b1);
                // fprintf(f, "\t\t{\"id\":%u, \"x\":%f, \"y\":%f, \"z\":%f, \"vx\":%f, \"vy\":%f, \"vz\":%f, \"type\":%u},\n", b1->id, b1->pos.x(), b1->pos.y(), b1->pos.z(), b1->velo.x(), b1->velo.y(), b1->velo.z(), b1->type);
                added = true;
                beads_added++;
            }
        }
    }

    // fprintf(f, "]}");
    // fclose(f);

#ifndef SERIAL
//    POETSDPDSimulator *simulator = new POETSDPDSimulator(&volume, 0, max_time);
#else
    SerialDPDSimulator *simulator = new SerialDPDSimulator();
#endif

    simulator.write(); // Write the volume to the simulator memory

    simulator.run(); // Start the simulation

    return 0;
}
