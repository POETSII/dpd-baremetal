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
#include <boost/algorithm/string.hpp>
#include <iomanip>

int main() {

#ifdef BOND_TESTING
    std::string bead_file = "../tests/beads_bonds_in_25.csv";
    std::string expected = "../tests/beads_bonds_out_25.csv";
    float problem_size = 25;
    int N = 25;
    uint32_t test_length = 1000;
#elif defined(LARGE_TEST)
    std::string bead_file = "../tests/beads_in_44.csv";
    std::string expected = "../tests/beads_out_44.csv";
    float problem_size = 44;
    int N = 44;
    uint32_t test_length = 1000;
#else
    std::string bead_file = "../tests/beads_in_18.csv";
    std::string expected = "../tests/beads_out_18.csv";
    float problem_size = 18;
    int N = 18;
    uint32_t test_length = 1000;
#endif

#ifdef GALS
    printf("Testing the GALS DPD application\n");
#else
    printf("Testing the DPD application\n");
#endif
#if defined(LARGE_TEST)
    printf("Testing a larger volume in the DPD application (no bonds)\n");
#endif
    printf("Volume dimensions: %f, %f, %f\n", problem_size, problem_size, problem_size);

    Universe<ptype> uni(problem_size, N, test_length);

    std::cerr << "Universe setup -- loading beads from " << bead_file << "\n";

    // Get the input beads from the file
    // File holding beads
    std::ifstream input_beads(bead_file);
    // Holds current line
    std::string line;
    // Loop through and add the beads to the universe
    while(std::getline(input_beads, line)) {
        // Used for splitting line into separate parts of bead info
        std::stringstream ss(line);
        // Holds individual peices of information
        std::string s;
        // Vector holding all information used to create the bead
        std::vector<std::string> lines;
        // Loop through the line, split it into separate parts
        while (std::getline(ss, s, ',')) {
            // Remove whitespaces
            boost::trim(s);
            // Add to vector
            lines.push_back(s);
        }
        // Create the bead
        bead_t* b1 = new bead_t;
        b1->id = std::stol(lines.at(0));
        b1->type = std::stoi(lines.at(1));
        b1->pos.set(std::stof(lines.at(2)), std::stof(lines.at(3)), std::stof(lines.at(4)));
        b1->velo.set(0.0, 0.0, 0.0);
    #ifndef GALS
        b1->acc.set(0.0, 0.0, 0.0);
    #elif defined(BETTER_VERLET)
        b1->acc.set(0.0, 0.0, 0.0);
    #endif
        // Cell for bead to go in
        unit_t cell;
        cell.x = std::stoi(lines.at(5));
        cell.y = std::stoi(lines.at(6));
        cell.z = std::stoi(lines.at(7));
        // Add it to the universe
        uni.add(cell, b1);
    }

    // Get the expected bead positionings from the expected output file
    // Store the expected bead positions in a map of bead ID to bead information
    std::map<uint32_t, bead_t> expected_beads_map;
    std::map<uint32_t, unit_t> expected_cell_map;

    std::ifstream expected_out(expected);
    // Reuse line from above
    // Loop through and add the beads to the expected output map
    while(std::getline(expected_out, line)) {
        // Used for splitting line into separate parts of bead info
        std::stringstream ss(line);
        // Holds individual peices of information
        std::string s;
        // Vector holding all information used to create the bead
        std::vector<std::string> lines;
        // Loop through the line, split it into separate parts
        while (std::getline(ss, s, ',')) {
            // Remove whitespaces
            boost::trim(s);
            // Add to vector
            lines.push_back(s);
        }
        // Create the bead
        bead_t b1;
        b1.id = std::stol(lines.at(0));
        b1.type = std::stoi(lines.at(1));
        b1.pos.set(std::stof(lines.at(2)), std::stof(lines.at(3)), std::stof(lines.at(4)));
        b1.velo.set(0.0, 0.0, 0.0);
    #ifndef GALS
        b1.acc.set(0.0, 0.0, 0.0);
    #elif defined(BETTER_VERLET)
        b1.acc.set(0.0, 0.0, 0.0);
    #endif
        // Cell that bead ends up in
        unit_t cell;
        cell.x = std::stoi(lines.at(5));
        cell.y = std::stoi(lines.at(6));
        cell.z = std::stoi(lines.at(7));
        // Add it to the map
        expected_beads_map[b1.id] = b1;
        expected_cell_map[b1.id] = cell;
    }

    uni.write(); // write the universe into the POETS memory

    // uni.print_occupancy();

    printf("running...\n");

    struct timeval start, finish, elapsedTime; // Time the test

    // Time it for interest
    gettimeofday(&start, NULL);

    // Run the test and get the result
    std::map<uint32_t, DPDMessage> actual_out = uni.test();

    // Get the finish time
    gettimeofday(&finish, NULL);
    // Calculate the elapsed time
    timersub(&finish, &start, &elapsedTime);
    double duration = (double) elapsedTime.tv_sec + (double) elapsedTime.tv_usec / 1000000.0;

    bool fail = false;

    // FILE* newFile = fopen("../tests/beads_out_44.csv", "w");

    for (std::map<uint32_t, DPDMessage>::iterator i = actual_out.begin(); i!=actual_out.end(); ++i) {
        // Actual values
        bead_id_t actual_id = i->second.beads[0].id;
        bead_class_t actual_type = i->second.beads[0].type;
        Vector3D<ptype> actual_pos = i->second.beads[0].pos;
        // Actual cell location
        unit_t actual_cell;
        actual_cell.x = i->second.from.x;
        actual_cell.y = i->second.from.y;
        actual_cell.z = i->second.from.z;
        // Expected values
        bead_id_t expected_id = expected_beads_map[i->first].id;
        bead_class_t expected_type = expected_beads_map[i->first].type;
        Vector3D<ptype> expected_pos = expected_beads_map[i->first].pos;
        // Expected cell location
        unit_t expected_cell;
        expected_cell.x = expected_cell_map[i->first].x;
        expected_cell.y = expected_cell_map[i->first].y;
        expected_cell.z = expected_cell_map[i->first].z;

        // fprintf(newFile, "%u, %u, %1.20f, %1.20f, %1.20f, %u, %u, %u\n", actual_id, actual_type, actual_pos.x(), actual_pos.y(), actual_pos.z(), actual_cell.x, actual_cell.y, actual_cell.z);
    #ifndef LARGE_TEST
        std::cerr << "ID: " << expected_id << "\n";

        std::cerr << "Type: Expected " << (uint32_t) expected_type << " Actual " << (uint32_t) actual_type << " ";
    #endif
        if (expected_type == actual_type) {
        #ifndef LARGE_TEST
            std::cerr << "PASS\n";
        #endif
        } else {
        #ifndef LARGE_TEST
            std::cerr << "FAIL\n";
        #endif
            fail = true;
        }

    #ifndef LARGE_TEST
        std::cerr << "Cell: Expected (" << expected_cell.x << ", " << expected_cell.y << ", " << expected_cell.z << ") Actual (" << actual_cell.x << ", " << actual_cell.y << ", " << actual_cell.z << ") ";
    #endif
        if (expected_cell.x == actual_cell.x && expected_cell.y == actual_cell.y && expected_cell.z == actual_cell.z) {
        #ifndef LARGE_TEST
            std::cerr << "PASS\n";
        #endif
        } else {
        #ifndef LARGE_TEST
            std::cerr << "FAIL\n";
        #endif
            fail = true;
        }

    #ifndef LARGE_TEST
        printf("Position: Expected (%1.20f, %1.20f, %1.20f)\n", expected_pos.x(), expected_pos.y(), expected_pos.z());
        printf("          Actual   (%1.20f, %1.20f, %1.20f) ", actual_pos.x(), actual_pos.y() , actual_pos.z());
    #endif
        if (expected_pos.x() == actual_pos.x() && expected_pos.y() == actual_pos.y() && expected_pos.z() == actual_pos.z()) {
        #ifndef LARGE_TEST
            printf("PASS\n");
        #endif
        } else {
        #ifndef LARGE_TEST
            printf("FAIL\n");
        #endif
            fail = true;
        }

    }

    // fclose(newFile);

    printf("TESTING HAS ");
    if (fail) {
        printf("FAILED\n");
        printf("Runtime = %1.10f\n", duration);
        return 1;
    } else {
        printf("PASSED\n");
        printf("Runtime = %1.10f\n", duration);
        return 0;
    }

    return 0;
}
