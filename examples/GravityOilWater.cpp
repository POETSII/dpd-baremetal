/* This will generate an XML file for a DPD simulation featuring water beads
and two types of oil*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h>
#include <math.h>
#include <random>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

#ifndef SERIAL
#include "POLiteSimulator.hpp"
#else
#include "SerialSimulator.hpp"
#endif

#define BEAD_DENSITY 3

void print_help() {
    std::cerr << "POETS DPD simulator - POLite version\n";
    std::cerr << "Usage:\n";
    std::cerr << "./run <Volume length> [--time t] ";
#ifdef XML
    std::cerr << "[--timed] ";
#endif
#ifndef SERIAL
    std::cerr << "[--boxes-x x] [--boxes-y y] ";
#endif
    std::cerr << "[--help]\n";
    std::cerr << "\n";
    std::cerr << "Volume length           - The length of one side of the simulation volume.\n";
    std::cerr << "                          Simulation volumes are (currently) assumed to be cubes.\n";
    std::cerr << "                          This value must be 3 or larger, no string.\n";
    std::cerr << "\n";
#ifdef XML
    std::cerr << "timed                   - Optional Boolean. If the run of the generated XML is to be timed.\n";
    std::cerr << "                        - Removes state exfiltration and ensures that it self-terminates";
    std::cerr << "                          reporting the wallclock runtime.\n";
    std::cerr << "\n";
#endif
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
    bool timed = false;

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
  #ifdef XML
    printf("Generating a DPD XML\n");
  #endif
    printf("Volume dimensions: %f, %f, %f\n", problem_size, problem_size, problem_size);

    // Increase the volume so there's somewhere to put wall beads
    float problem_size_with_walls = problem_size + 1;
    uint32_t cells_per_dimension_with_walls = N + 1;

    // Get the directory to store simulation states
    char cwd_buffer[PATH_MAX], *unused;
    unused = getcwd(cwd_buffer, sizeof(cwd_buffer));
    std::cout << cwd_buffer << "\n";

  #ifndef SERIAL
    std::string state_dir = std::string(cwd_buffer) + "/../polite-dpd-states/";
  #else
    std::string state_dir = std::string(cwd_buffer) + "/../serial-dpd-states/";
  #endif

  #ifndef SERIAL
    // Default box numbers are x = 1, y = 1.
    // These can be set at run time, or hard coded.
    POLiteSimulator simulator(problem_size_with_walls, cells_per_dimension_with_walls, 0, max_time, state_dir, boxes_x, boxes_y);
    POLiteVolume *volume = simulator.get_volume();
  #else
    SerialSimulator simulator(problem_size_with_walls, cells_per_dimension_with_walls, 0, max_time, state_dir);
    SerialVolume *volume = simulator.get_volume();
  #endif

    printf("Volume setup\n");

    int total_beads = N * N * N * BEAD_DENSITY;
    int w = 0.6 * total_beads;
    int r = 0.3 * total_beads;
    int o = 0.1 * total_beads;

    // May lose one or two bead in conversion from float to int, so let's get the real number
    total_beads = w + r + o;

    // ************* Generate initial velocities ********************
    // Declare temperature for simulation
    const float temp = 1.0;

    // Generate initial velocities (Maxwell distribution, courtesy of Julian)
    std::vector<float> rvelDist = maxwellDist(total_beads);

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

    vtotal = sqrt(vtotal / static_cast<double>(3 * total_beads)); // 3 for axes of movement

    // finally normalize the velocities to the required temperature,
    for(int i = 0; i < total_beads; i++)
    {
        velDist.at(i).x(sqrt(temp) * velDist.at(i).x() / vtotal);
        velDist.at(i).y(sqrt(temp) * velDist.at(i).y() / vtotal);
        velDist.at(i).z(sqrt(temp) * velDist.at(i).z() / vtotal);
    }

    std::string init_state_file = state_dir + "state_0.json";

    FILE* f = fopen(init_state_file.c_str(), "w+");
    fprintf(f, "{\n\t\"beads\":[\n");
    bool first_bead = true;

    std::cout << "Building walls\n";

        uint32_t b_uid = 0;

    // Edge x = 0
    for (uint16_t y = 0; y < problem_size_with_walls; y++) {
        for (uint16_t z = 0; z < problem_size_with_walls; z++) {
            cell_t cell = {0, y, z};

            bead_t *w1 = new bead_t;
            w1->id = b_uid++;
            w1->type = 3;
            w1->pos.set(0.25, 0.25, 0.25);
            volume->add_bead_to_cell(w1, cell);
            // if (first_bead) {
            //     fprintf(f, "\t\t{\"id\":%u, \"x\":%f, \"y\":%f, \"z\":%f, \"vx\":%f, \"vy\":%f, \"vz\":%f, \"type\":%u}", w1->id, w1->pos.x() + cell.x, w1->pos.y() + cell.y, w1->pos.z() + cell.z, w1->velo.x(), w1->velo.y(), w1->velo.z(), w1->type);
            //     first_bead = false;
            // } else {
            //     fprintf(f, ",\n\t\t{\"id\":%u, \"x\":%f, \"y\":%f, \"z\":%f, \"vx\":%f, \"vy\":%f, \"vz\":%f, \"type\":%u}", w1->id, w1->pos.x() + cell.x, w1->pos.y() + cell.y, w1->pos.z() + cell.z, w1->velo.x(), w1->velo.y(), w1->velo.z(), w1->type);
            // }

            bead_t *w2 = new bead_t;
            w2->id = b_uid++;
            w2->type = 3;
            w2->pos.set(0.25, 0.25, 0.75);
            volume->add_bead_to_cell(w2, cell);
            // fprintf(f, ",\n\t\t{\"id\":%u, \"x\":%f, \"y\":%f, \"z\":%f, \"vx\":%f, \"vy\":%f, \"vz\":%f, \"type\":%u}", w2->id, w2->pos.x() + cell.x, w2->pos.y() + cell.y, w2->pos.z() + cell.z, w2->velo.x(), w2->velo.y(), w2->velo.z(), w2->type);

            bead_t *w3 = new bead_t;
            w3->id = b_uid++;
            w3->type = 3;
            w3->pos.set(0.25, 0.75, 0.25);
            volume->add_bead_to_cell(w3, cell);
            // fprintf(f, ",\n\t\t{\"id\":%u, \"x\":%f, \"y\":%f, \"z\":%f, \"vx\":%f, \"vy\":%f, \"vz\":%f, \"type\":%u}", w3->id, w3->pos.x() + cell.x, w3->pos.y() + cell.y, w3->pos.z() + cell.z, w3->velo.x(), w3->velo.y(), w3->velo.z(), w3->type);

            bead_t *w4 = new bead_t;
            w4->id = b_uid++;
            w4->type = 3;
            w4->pos.set(0.25, 0.75, 0.75);
            volume->add_bead_to_cell(w4, cell);
            // fprintf(f, ",\n\t\t{\"id\":%u, \"x\":%f, \"y\":%f, \"z\":%f, \"vx\":%f, \"vy\":%f, \"vz\":%f, \"type\":%u}", w4->id, w4->pos.x() + cell.x, w4->pos.y() + cell.y, w4->pos.z() + cell.z, w4->velo.x(), w4->velo.y(), w4->velo.z(), w4->type);
        }
    }

    // Edge x = N - 1
    for (uint16_t y = 0; y < problem_size_with_walls; y++) {
        for (uint16_t z = 0; z < problem_size_with_walls; z++) {
            cell_t cell = {(uint16_t)N, y, z};

            bead_t *w1 = new bead_t;
            w1->id = b_uid++;
            w1->type = 3;
            w1->pos.set(0.75, 0.25, 0.25);
            volume->add_bead_to_cell(w1, cell);
            // fprintf(f, ",\n\t\t{\"id\":%u, \"x\":%f, \"y\":%f, \"z\":%f, \"vx\":%f, \"vy\":%f, \"vz\":%f, \"type\":%u}", w1->id, w1->pos.x() + cell.x, w1->pos.y() + cell.y, w1->pos.z() + cell.z, w1->velo.x(), w1->velo.y(), w1->velo.z(), w1->type);

            bead_t *w2 = new bead_t;
            w2->id = b_uid++;
            w2->type = 3;
            w2->pos.set(0.75, 0.25, 0.75);
            volume->add_bead_to_cell(w2, cell);
            // fprintf(f, ",\n\t\t{\"id\":%u, \"x\":%f, \"y\":%f, \"z\":%f, \"vx\":%f, \"vy\":%f, \"vz\":%f, \"type\":%u}", w2->id, w2->pos.x() + cell.x, w2->pos.y() + cell.y, w2->pos.z() + cell.z, w2->velo.x(), w2->velo.y(), w2->velo.z(), w2->type);

            bead_t *w3 = new bead_t;
            w3->id = b_uid++;
            w3->type = 3;
            w3->pos.set(0.75, 0.75, 0.25);
            volume->add_bead_to_cell(w3, cell);
            // fprintf(f, ",\n\t\t{\"id\":%u, \"x\":%f, \"y\":%f, \"z\":%f, \"vx\":%f, \"vy\":%f, \"vz\":%f, \"type\":%u}", w3->id, w3->pos.x() + cell.x, w3->pos.y() + cell.y, w3->pos.z() + cell.z, w3->velo.x(), w3->velo.y(), w3->velo.z(), w3->type);

            bead_t *w4 = new bead_t;
            w4->id = b_uid++;
            w4->type = 3;
            w4->pos.set(0.75, 0.75, 0.75);
            volume->add_bead_to_cell(w4, cell);
            // fprintf(f, ",\n\t\t{\"id\":%u, \"x\":%f, \"y\":%f, \"z\":%f, \"vx\":%f, \"vy\":%f, \"vz\":%f, \"type\":%u}", w4->id, w4->pos.x() + cell.x, w4->pos.y() + cell.y, w4->pos.z() + cell.z, w4->velo.x(), w4->velo.y(), w4->velo.z(), w4->type);
        }
    }

    // Edge y = 0
    for (uint16_t x = 0; x < problem_size_with_walls; x++) {
        for (uint16_t z = 0; z < problem_size_with_walls; z++) {
            cell_t cell = {x, 0, z};

            if (x != 0) {
                bead_t *w1 = new bead_t;
                w1->id = b_uid++;
                w1->type = 3;
                w1->pos.set(0.25, 0.25, 0.25);
                volume->add_bead_to_cell(w1, cell);
                // fprintf(f, ",\n\t\t{\"id\":%u, \"x\":%f, \"y\":%f, \"z\":%f, \"vx\":%f, \"vy\":%f, \"vz\":%f, \"type\":%u}", w1->id, w1->pos.x() + cell.x, w1->pos.y() + cell.y, w1->pos.z() + cell.z, w1->velo.x(), w1->velo.y(), w1->velo.z(), w1->type);

                bead_t *w2 = new bead_t;
                w2->id = b_uid++;
                w2->type = 3;
                w2->pos.set(0.25, 0.25, 0.75);
                volume->add_bead_to_cell(w2, cell);
                // fprintf(f, ",\n\t\t{\"id\":%u, \"x\":%f, \"y\":%f, \"z\":%f, \"vx\":%f, \"vy\":%f, \"vz\":%f, \"type\":%u}", w2->id, w2->pos.x() + cell.x, w2->pos.y() + cell.y, w2->pos.z() + cell.z, w2->velo.x(), w2->velo.y(), w2->velo.z(), w2->type);
            }

            if (x != problem_size) {
                bead_t *w3 = new bead_t;
                w3->id = b_uid++;
                w3->type = 3;
                w3->pos.set(0.75, 0.25, 0.25);
                volume->add_bead_to_cell(w3, cell);
                // fprintf(f, ",\n\t\t{\"id\":%u, \"x\":%f, \"y\":%f, \"z\":%f, \"vx\":%f, \"vy\":%f, \"vz\":%f, \"type\":%u}", w3->id, w3->pos.x() + cell.x, w3->pos.y() + cell.y, w3->pos.z() + cell.z, w3->velo.x(), w3->velo.y(), w3->velo.z(), w3->type);

                bead_t *w4 = new bead_t;
                w4->id = b_uid++;
                w4->type = 3;
                w4->pos.set(0.75, 0.25, 0.75);
                volume->add_bead_to_cell(w4, cell);
                // fprintf(f, ",\n\t\t{\"id\":%u, \"x\":%f, \"y\":%f, \"z\":%f, \"vx\":%f, \"vy\":%f, \"vz\":%f, \"type\":%u}", w4->id, w4->pos.x() + cell.x, w4->pos.y() + cell.y, w4->pos.z() + cell.z, w4->velo.x(), w4->velo.y(), w4->velo.z(), w4->type);
            }
        }
    }

    // Edge y = N - 1
    for (uint16_t x = 0; x < problem_size_with_walls; x++) {
        for (uint16_t z = 0; z < problem_size_with_walls; z++) {
            cell_t cell = {x, (uint16_t)N, z};

            if (x != 0) {
                bead_t *w1 = new bead_t;
                w1->id = b_uid++;
                w1->type = 3;
                w1->pos.set(0.25, 0.75, 0.25);
                volume->add_bead_to_cell(w1, cell);
                // fprintf(f, ",\n\t\t{\"id\":%u, \"x\":%f, \"y\":%f, \"z\":%f, \"vx\":%f, \"vy\":%f, \"vz\":%f, \"type\":%u}", w1->id, w1->pos.x() + cell.x, w1->pos.y() + cell.y, w1->pos.z() + cell.z, w1->velo.x(), w1->velo.y(), w1->velo.z(), w1->type);

                bead_t *w2 = new bead_t;
                w2->id = b_uid++;
                w2->type = 3;
                w2->pos.set(0.25, 0.75, 0.75);
                volume->add_bead_to_cell(w2, cell);
                // fprintf(f, ",\n\t\t{\"id\":%u, \"x\":%f, \"y\":%f, \"z\":%f, \"vx\":%f, \"vy\":%f, \"vz\":%f, \"type\":%u}", w2->id, w2->pos.x() + cell.x, w2->pos.y() + cell.y, w2->pos.z() + cell.z, w2->velo.x(), w2->velo.y(), w2->velo.z(), w2->type);
            }

            if (x != problem_size) {
                bead_t *w3 = new bead_t;
                w3->id = b_uid++;
                w3->type = 3;
                w3->pos.set(0.75, 0.75, 0.25);
                volume->add_bead_to_cell(w3, cell);
                // fprintf(f, ",\n\t\t{\"id\":%u, \"x\":%f, \"y\":%f, \"z\":%f, \"vx\":%f, \"vy\":%f, \"vz\":%f, \"type\":%u}", w3->id, w3->pos.x() + cell.x, w3->pos.y() + cell.y, w3->pos.z() + cell.z, w3->velo.x(), w3->velo.y(), w3->velo.z(), w3->type);

                bead_t *w4 = new bead_t;
                w4->id = b_uid++;
                w4->type = 3;
                w4->pos.set(0.75, 0.75, 0.75);
                volume->add_bead_to_cell(w4, cell);
                // fprintf(f, ",\n\t\t{\"id\":%u, \"x\":%f, \"y\":%f, \"z\":%f, \"vx\":%f, \"vy\":%f, \"vz\":%f, \"type\":%u}", w4->id, w4->pos.x() + cell.x, w4->pos.y() + cell.y, w4->pos.z() + cell.z, w4->velo.x(), w4->velo.y(), w4->velo.z(), w4->type);
            }

        }
    }

    // Edge z = 0
    for (uint16_t x = 0; x < problem_size_with_walls; x++) {
        for (uint16_t y = 0; y < problem_size_with_walls; y++) {
            cell_t cell = {x, y, 0};

            if (x != 0 && y != 0) {
                bead_t *w1 = new bead_t;
                w1->id = b_uid++;
                w1->type = 3;
                w1->pos.set(0.25, 0.25, 0.25);
                volume->add_bead_to_cell(w1, cell);
                // fprintf(f, ",\n\t\t{\"id\":%u, \"x\":%f, \"y\":%f, \"z\":%f, \"vx\":%f, \"vy\":%f, \"vz\":%f, \"type\":%u}", w1->id, w1->pos.x() + cell.x, w1->pos.y() + cell.y, w1->pos.z() + cell.z, w1->velo.x(), w1->velo.y(), w1->velo.z(), w1->type);
            }

            if (x != 0 && y != problem_size) {
                bead_t *w2 = new bead_t;
                w2->id = b_uid++;
                w2->type = 3;
                w2->pos.set(0.25, 0.75, 0.25);
                volume->add_bead_to_cell(w2, cell);
                // fprintf(f, ",\n\t\t{\"id\":%u, \"x\":%f, \"y\":%f, \"z\":%f, \"vx\":%f, \"vy\":%f, \"vz\":%f, \"type\":%u}", w2->id, w2->pos.x() + cell.x, w2->pos.y() + cell.y, w2->pos.z() + cell.z, w2->velo.x(), w2->velo.y(), w2->velo.z(), w2->type);
            }

            if (x != problem_size && y != 0) {
                bead_t *w3 = new bead_t;
                w3->id = b_uid++;
                w3->type = 3;
                w3->pos.set(0.75, 0.25, 0.25);
                volume->add_bead_to_cell(w3, cell);
                // fprintf(f, ",\n\t\t{\"id\":%u, \"x\":%f, \"y\":%f, \"z\":%f, \"vx\":%f, \"vy\":%f, \"vz\":%f, \"type\":%u}", w3->id, w3->pos.x() + cell.x, w3->pos.y() + cell.y, w3->pos.z() + cell.z, w3->velo.x(), w3->velo.y(), w3->velo.z(), w3->type);
            }

            if (x != problem_size && y != problem_size) {
                bead_t *w4 = new bead_t;
                w4->id = b_uid++;
                w4->type = 3;
                w4->pos.set(0.75, 0.75, 0.25);
                volume->add_bead_to_cell(w4, cell);
                // fprintf(f, ",\n\t\t{\"id\":%u, \"x\":%f, \"y\":%f, \"z\":%f, \"vx\":%f, \"vy\":%f, \"vz\":%f, \"type\":%u}", w4->id, w4->pos.x() + cell.x, w4->pos.y() + cell.y, w4->pos.z() + cell.z, w4->velo.x(), w4->velo.y(), w4->velo.z(), w4->type);
            }
        }
    }

    // Edge y = N - 1
    for (uint16_t x = 0; x < problem_size_with_walls; x++) {
        for (uint16_t y = 0; y < problem_size_with_walls; y++) {
            cell_t cell = {x, y, (uint16_t)N};

            if (x != 0 && y != 0) {
                bead_t *w1 = new bead_t;
                w1->id = b_uid++;
                w1->type = 3;
                w1->pos.set(0.25, 0.25, 0.75);
                volume->add_bead_to_cell(w1, cell);
                // fprintf(f, ",\n\t\t{\"id\":%u, \"x\":%f, \"y\":%f, \"z\":%f, \"vx\":%f, \"vy\":%f, \"vz\":%f, \"type\":%u}", w1->id, w1->pos.x() + cell.x, w1->pos.y() + cell.y, w1->pos.z() + cell.z, w1->velo.x(), w1->velo.y(), w1->velo.z(), w1->type);
            }

            if (x != 0 && y != problem_size) {
                bead_t *w2 = new bead_t;
                w2->id = b_uid++;
                w2->type = 3;
                w2->pos.set(0.25, 0.75, 0.75);
                volume->add_bead_to_cell(w2, cell);
                // fprintf(f, ",\n\t\t{\"id\":%u, \"x\":%f, \"y\":%f, \"z\":%f, \"vx\":%f, \"vy\":%f, \"vz\":%f, \"type\":%u}", w2->id, w2->pos.x() + cell.x, w2->pos.y() + cell.y, w2->pos.z() + cell.z, w2->velo.x(), w2->velo.y(), w2->velo.z(), w2->type);
            }

            if (x != problem_size && y != 0) {
                bead_t *w3 = new bead_t;
                w3->id = b_uid++;
                w3->type = 3;
                w3->pos.set(0.75, 0.25, 0.75);
                volume->add_bead_to_cell(w3, cell);
                // fprintf(f, ",\n\t\t{\"id\":%u, \"x\":%f, \"y\":%f, \"z\":%f, \"vx\":%f, \"vy\":%f, \"vz\":%f, \"type\":%u}", w3->id, w3->pos.x() + cell.x, w3->pos.y() + cell.y, w3->pos.z() + cell.z, w3->velo.x(), w3->velo.y(), w3->velo.z(), w3->type);
            }

            if (x != problem_size && y != problem_size) {
                bead_t *w4 = new bead_t;
                w4->id = b_uid++;
                w4->type = 3;
                w4->pos.set(0.75, 0.75, 0.75);
                volume->add_bead_to_cell(w4, cell);
                // fprintf(f, ",\n\t\t{\"id\":%u, \"x\":%f, \"y\":%f, \"z\":%f, \"vx\":%f, \"vy\":%f, \"vz\":%f, \"type\":%u}", w4->id, w4->pos.x() + cell.x, w4->pos.y() + cell.y, w4->pos.z() + cell.z, w4->velo.x(), w4->velo.y(), w4->velo.z(), w4->type);
            }

        }
    }

    std::cout << "Adding beads\n";

    // Add water beads
    for(int i=0; i<w; i++) {
        bool added = false;
        while(!added) {
            bead_t *b1 = new bead_t;
            b1->id = b_uid++;
            b1->type = 0;
            // Add 1 to each dimension as these only go in the centre. We don't want them in the edge cells as that's for walls only
            b1->pos.set((rand() / (float)RAND_MAX * problem_size) + 1, (rand() / (float)RAND_MAX * problem_size) + 1, (rand() / (float)RAND_MAX * problem_size) + 1);
            b1->velo.set(velDist.at(beads_added).x(), velDist.at(beads_added).y(), velDist.at(beads_added).z());
          #ifdef BETTER_VERLET
            b1->acc.set(0.0, 0.0, 0.0);
          #endif
            if(volume->space_for_bead(b1)) {
                volume->add_bead(b1);
                added = true;
                beads_added++;
                if (first_bead) {
                    fprintf(f, "\t\t{\"id\":%u, \"x\":%f, \"y\":%f, \"z\":%f, \"vx\":%f, \"vy\":%f, \"vz\":%f, \"type\":%u}", b1->id, b1->pos.x(), b1->pos.y(), b1->pos.z(), b1->velo.x(), b1->velo.y(), b1->velo.z(), b1->type);
                    first_bead = false;
                } else {
                    fprintf(f, ",\n\t\t{\"id\":%u, \"x\":%f, \"y\":%f, \"z\":%f, \"vx\":%f, \"vy\":%f, \"vz\":%f, \"type\":%u}", b1->id, b1->pos.x(), b1->pos.y(), b1->pos.z(), b1->velo.x(), b1->velo.y(), b1->velo.z(), b1->type);
                }
            }
        }
    }

    // Add oil 1 beads
    for(int i=0; i<r; i++) {
        bool added = false;
        while(!added) {
            bead_t *b1 = new bead_t;
            b1->id = b_uid++;
            b1->type = 1;
            b1->pos.set((rand() / (float)RAND_MAX * problem_size) + 1, (rand() / (float)RAND_MAX * problem_size) + 1, (rand() / (float)RAND_MAX * problem_size) + 1);
            b1->velo.set(velDist.at(beads_added).x(), velDist.at(beads_added).y(), velDist.at(beads_added).z());
          #ifdef BETTER_VERLET
            b1->acc.set(0.0, 0.0, 0.0);
          #endif
            if(volume->space_for_bead(b1)) {
                volume->add_bead(b1);
                added = true;
                beads_added++;
                fprintf(f, ",\n\t\t{\"id\":%u, \"x\":%f, \"y\":%f, \"z\":%f, \"vx\":%f, \"vy\":%f, \"vz\":%f, \"type\":%u}", b1->id, b1->pos.x(), b1->pos.y(), b1->pos.z(), b1->velo.x(), b1->velo.y(), b1->velo.z(), b1->type);
            }
        }
    }

    // Add oil 2 beads
    for(int i=0; i<o; i++) {
        bool added = false;
        while(!added) {
            bead_t *b1 = new bead_t;
            b1->id = b_uid++;
            b1->type = 2;
            b1->pos.set((rand() / (float)RAND_MAX * problem_size) + 1, (rand() / (float)RAND_MAX * problem_size) + 1, (rand() / (float)RAND_MAX * problem_size) + 1);
            b1->velo.set(velDist.at(beads_added).x(), velDist.at(beads_added).y(), velDist.at(beads_added).z());
          #ifdef BETTER_VERLET
            b1->acc.set(0.0, 0.0, 0.0);
          #endif
            if(volume->space_for_bead(b1)) {
                volume->add_bead(b1);
                added = true;
                beads_added++;
                fprintf(f, ",\n\t\t{\"id\":%u, \"x\":%f, \"y\":%f, \"z\":%f, \"vx\":%f, \"vy\":%f, \"vz\":%f, \"type\":%u}", b1->id, b1->pos.x(), b1->pos.y(), b1->pos.z(), b1->velo.x(), b1->velo.y(), b1->velo.z(), b1->type);
            }
        }
    }

    fprintf(f, "\n\t]\n}");
    fclose(f);

    simulator.write();

    simulator.run();

    return 0;
}