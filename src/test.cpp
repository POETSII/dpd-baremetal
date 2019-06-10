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
#include <boost/algorithm/string.hpp>

int main() {

    float problem_size = 18;
    int N = 18;

    printf("Testing the DPD application\n");
    printf("Volume dimensions: %f, %f, %f\n", problem_size, problem_size, problem_size);

    Universe<ptype> uni(problem_size, N);

    std::string bead_file = "../tests/beads_in_18.csv";

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
        b1->id = std::stoi(lines.at(0));
        b1->type = std::stoi(lines.at(1));
        b1->pos.set(std::stof(lines.at(2)), std::stof(lines.at(3)), std::stof(lines.at(4)));
        b1->velo.set(0.0, 0.0, 0.0);
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
    // Open expected bead file
    std::string expected = "../tests/beads_out_18.csv";
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
        b1.id = std::stoi(lines.at(0));
        b1.type = std::stoi(lines.at(1));
        b1.pos.set(std::stof(lines.at(2)), std::stof(lines.at(3)), std::stof(lines.at(4)));
        b1.velo.set(0.0, 0.0, 0.0);
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

    uni.print_occupancy();

    printf("running...\n");
    // Run the test and get the result
    std::map<uint32_t, DPDMessage> actual_out = uni.test();

    for (std::map<uint32_t, bead_t>::iterator i = expected_beads_map.begin(); i!=expected_beads_map.end(); ++i) {
        // Expected values
        bead_id_t expected_id = i->second.id;
        bead_class_t expected_type = i->second.type;
        Vector3D<ptype> expected_pos = i->second.pos;
        // Expected cell location
        unit_t expected_cell;
        expected_cell.x = expected_cell_map[i->first].x;
        expected_cell.y = expected_cell_map[i->first].y;
        expected_cell.z = expected_cell_map[i->first].z;
        // Actual values
        bead_id_t actual_id = actual_out[i->first].beads[0].id;
        bead_class_t actual_type = actual_out[i->first].beads[0].type;
        Vector3D<ptype> actual_pos = actual_out[i->first].beads[0].pos;
        // Actual cell location
        unit_t actual_cell;
        actual_cell.x = actual_out[i->first].from.x;
        actual_cell.y = actual_out[i->first].from.y;
        actual_cell.z = actual_out[i->first].from.z;

        std::cerr << "ID: " << expected_id << "\n";
        std::cerr << "Type: Expected " << expected_type << " Actual " << actual_type << "\n";
        std::cerr << "Cell: Expected (" << expected_cell.x << ", " << expected_cell.y << ", " << expected_cell.z << ") Actual " << actual_cell.x << ", " << actual_cell.y << ", " << actual_cell.z << ")\n";
        std::cerr << "Position: Expected (" << expected_pos.x() << ", " << expected_pos.y() << ", " << expected_pos.z() << ") ";
        std::cerr << " Actual (" << actual_pos.x() << ", " << actual_pos.y() << ", " << actual_pos.z() << ")\n";
    }

    return 0;
}
