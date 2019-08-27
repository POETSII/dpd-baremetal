#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <map>
#include "../inc/Vector3D.hpp"
#include "../inc/dpd.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <boost/algorithm/string.hpp>
#include <vector>
#include <sstream>
#include <set>
#include <tuple>

const uint32_t uni_len = 10;
const float min_dist = 1.0;

// Map of timestep to list of blobs
std::map<uint32_t, std::vector<std::set<uint32_t>>> blob_map;

unit_t getNeighbourUnit(unit_t current, int n_x, int n_y, int n_z) {
    unit_t result;
    int16_t x = 0, y = 0, z = 0;

    x = current.x + n_x;
    y = current.y + n_y;
    z = current.z + n_z;

    if (x == uni_len) {
        x = 0;
    } else if (x == -1) {
        x = uni_len - 1;
    }

    if (y == uni_len) {
        y = 0;
    } else if (y == -1) {
        y = uni_len - 1;
    }

    if (z == uni_len) {
        z = 0;
    } else if (z == -1) {
        z = uni_len - 1;
    }

    result = {x, y, z};
    assert(result.x < uni_len && result.y < uni_len && result.z < uni_len);
    assert(result.x >= 0 && result.y >= 0 && result.z >= 0);
    // std::cerr << "In: (" << current.x << ", " << current.y << ", " << current.z << ")\n";
    // std::cerr << "ns: (" << n_x << ", " << n_y << ", " << n_z << ")\n";
    // std::cerr << "Rs: (" << result.x << ", " << result.y << ", " << result.z << ")\n";
    return result;
}

std::tuple<Vector3D<float>,Vector3D<float>> correctPositions(unit_t unit, unit_t neighbour, Vector3D<float> i, Vector3D<float> j) {
    Vector3D<float> i_pos;
    i_pos.set(i.x(), i.y(), i.z());
    Vector3D<float> j_pos;
    j_pos.set(j.x(), j.y(), j.z());

    if (unit.x == 0 && neighbour.x == uni_len - 1) {
        i_pos.x(i.x() + uni_len);
    } else if (unit.x == uni_len - 1 && neighbour.x == 0) {
        j_pos.x(j.x() + uni_len);
    }

    if (unit.y == 0 && neighbour.y == uni_len - 1) {
        i_pos.y(i.y() + uni_len);
    } else if (unit.y == uni_len - 1 && neighbour.y == 0) {
        j_pos.y(j.y() + uni_len);
    }

    if (unit.z == 0 && neighbour.z == uni_len - 1) {
        i_pos.z(i.z() + uni_len);
    } else if (unit.z == uni_len - 1 && neighbour.z == 0) {
        j_pos.z(j.z() + uni_len);
    }

    // std::cerr << "Unit: (" << unit.x << ", " << unit.y << ", " << unit.z << ")\n";
    // std::cerr << "Neig: (" << neighbour.x << ", " << neighbour.y << ", " << neighbour.z << ")\n";
    // std::cerr << "i: (" << i.x() << ", " << i.y() << ", " << i.z() << ")\n";
    // std::cerr << "j: (" << j.x() << ", " << j.y() << ", " << j.z() << ")\n";

    assert(i_pos.x() < uni_len + 1 && i_pos.y() < uni_len + 1 && i_pos.z() < uni_len + 1);
    assert(i_pos.x() >= 0 && i_pos.y() >= 0 && i_pos.z() >= 0);
    assert(j_pos.x() < uni_len + 1 && j_pos.y() < uni_len + 1 && j_pos.z() < uni_len + 1);
    assert(j_pos.x() >= 0 && j_pos.y() >= 0 && j_pos.z() >= 0);

    return std::make_tuple(i_pos, j_pos);
}

std::set<uint32_t> getBlob(uint32_t timestep, uint32_t id) {
    for (std::vector<std::set<uint32_t>>::iterator l = blob_map[timestep].begin(); l != blob_map[timestep].end(); ++l) {
        if (l->find(id) != l->end()) {
            return *l;
        }
    }
    std::set<uint32_t> result;
    result.insert(id);
    blob_map[timestep].push_back(result);
    return result;
}

std::vector<std::set<uint32_t>> eraseFromBlobMap(uint32_t timestep, std::set<uint32_t> blob_to_be_removed) {
    std::vector<std::set<uint32_t>> result;
    for (std::vector<std::set<uint32_t>>::iterator i = blob_map[timestep].begin(); i != blob_map[timestep].end(); ++ i) {
        if (*i == blob_to_be_removed) {
            continue;
        }
        result.push_back(*i);
    }
    return result;
}

void mergeBlobs(uint32_t i, std::set<uint32_t> blob, uint32_t timestep) {
    std::set<uint32_t> main_blob = getBlob(timestep, i);
    for (std::set<uint32_t>::iterator bead = blob.begin(); bead != blob.end(); ++bead) {
        std::set<uint32_t> bead_blob = getBlob(timestep, *bead);
        if (bead_blob == main_blob) {
            continue;
        }
        // std::cerr << "main_blob.size() before: " << main_blob.size() << "\n";
        for (std::set<uint32_t>::iterator b = bead_blob.begin(); b != bead_blob.end(); ++b) {
            // std::cerr << "Adding " << *b << " to main_blob\n";
            main_blob.insert(*b);
        }
        // std::cerr << "main_blob.size() after: " << main_blob.size() << "\n";
        // std::cerr << "blob_map[timestep].size() before: " << blob_map[timestep].size() << "\n";
        blob_map[timestep] = eraseFromBlobMap(timestep, bead_blob);
        // std::cerr << "blob_map[timestep].size() after: " << blob_map[timestep].size() << "\n";
    }
    blob_map[timestep] = eraseFromBlobMap(timestep, main_blob);
    blob_map[timestep].push_back(main_blob);
    // std::cerr << "main_blob.size() = " << main_blob.size() << " and getBlob(timestep, i).size() = " << getBlob(timestep, i).size() << "\n";
}

int main(int argc, char *argv[]) {

    if (argc > 2) {
        printf("Too many arguments. Please indicate whether this should find blobbyness of force_update or accelerated DPD\n");
        return 1;
    }
    bool accelerate = false;

    if (argc > 1 && boost::contains(argv[1], "accelerator")) {
        accelerate = true;
    } else if (argc != 1) {
        std::cerr << "Didn't recognise argument\n";
        return 1;
    }

    std::string line;

    std::ostringstream oss;
    if (!accelerate)
        oss << "../perf-results/force_update_bead_positions_" << uni_len << ".csv";
    else
        oss << "../perf-results/accelerator_bead_positions_" << uni_len << ".csv";

    std::cerr << "Loading beads from " << oss.str().c_str() << "\n";
    std::ifstream beads(oss.str().c_str());
    std::map<uint32_t, std::map<unit_t, std::vector<bead_t>>> bead_map;

    // Loop through and add the beads to the map
    while(std::getline(beads, line)) {
        // Used for splitting line into separate timesteps
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
        // Get timestep
        uint32_t timestep = std::stoi(lines.at(0));
        // std::cerr << "TIMESTEP = " << timestep << "\n";
        for (uint32_t i = 1; i < lines.size(); i += 5) {
            // Get bead id
            uint32_t id = std::stoi(lines.at(i));
            // Get bead type
            uint32_t type = std::stoi(lines.at(i+1));
            assert(type < 3 && type >= 0);
            // Get bead position
            Vector3D<float> pos;
            pos.set(std::stof(lines.at(i + 2)), std::stof(lines.at(i + 3)), std::stof(lines.at(i + 4)));

            // Rounding error correction (A bug from storing to and then reading from a file)
            if (pos.x() == 10) {
                pos.set(0, pos.y(), pos.z());
            }
            if (pos.y() == 10) {
                pos.set(pos.x(), 0, pos.z());
            }
            if (pos.z() == 10) {
                pos.set(pos.x(), pos.y(), 0);
            }

            unit_pos_t x = floor(pos.x());
            unit_pos_t y = floor(pos.y());
            unit_pos_t z = floor(pos.z());
            unit_t loc = {x, y, z};
            assert(loc.x < uni_len && loc.y < uni_len && loc.z < uni_len);
            assert(loc.x >= 0 && loc.y >= 0 && loc.z >= 0);

            pos.set(pos.x(), pos.y(), pos.z());
            bead_t bead;
            bead.id = id;
            bead.type = type;
            bead.pos = pos;
            bead_map[timestep][loc].push_back(bead);
            // std::cerr << "Loc: (" << loc.x << ", " << loc.y << ", " << loc.z << ")\n";
            // std::cerr << "Pos aft: (" << pos.x() << ", " << pos.y() << ", " << pos.z() << ")\n";
            assert(pos.x() < uni_len && pos.y() < uni_len && pos.z() < uni_len);
        }
    }

    // For each timestep
    // for (std::map<uint32_t, std::map<unit_t, std::vector<bead_t>>>::iterator outer_map_it = bead_map.begin(); outer_map_it != bead_map.end(); ++outer_map_it) {
    for (uint32_t timestep = 0; timestep < 100; timestep++) {
        std::cerr << "Timestep = " << timestep << "\n";
        // uint32_t timestep = outer_map_it->first;
        // std::map<unit_t, std::vector<bead_t>> universe = outer_map_it->second;
        std::map<unit_t, std::vector<bead_t>> universe = bead_map[timestep];

        // For each unit
        for (std::map<unit_t, std::vector<bead_t>>::iterator current_unit = universe.begin(); current_unit != universe.end(); ++current_unit) {
            unit_t unit = current_unit->first;
            std::vector<bead_t> unit_beads = current_unit->second;

            // For each bead in this unit
            for (std::vector<bead_t>::iterator i = unit_beads.begin(); i != unit_beads.end(); ++i) {
                // If water, ignore
                if (i->type == 0) {
                    // std::cerr << i->id << " is water\n";
                    // std::cin.get();
                    continue;
                }
                // std::cerr << i->id << " is oil\n";
                // std::cin.get();
                // Add this bead to a blob, in case it is part of a blob (is discarded if not part of a blob)
                std::set<uint32_t> current_blob;

                // Used to break from loops early if bead i is found not to be in a blob
                bool not_blob = false;

                // Nested loops for all neighbours (including self for simplicity)
                for (int x = -1; x <= 1; x++) {
                    for (int y = -1; y <= 1; y++) {
                        for (int z = -1; z <= 1; z++) {
                            // Get the neighbour
                            unit_t neighbour = getNeighbourUnit(unit, x, y, z);
                            // std::cerr << "Examining neighbour " << neighbour.x << ", " << neighbour.y << ", " << neighbour.z <<"\n";
                            // std::cin.get();
                            // For all beads in the neighbour
                            for (std::vector<bead_t>::iterator j = universe[neighbour].begin(); j != universe[neighbour].end(); ++j) {
                                // If beads i and j are the same just move one
                                if (i->id != j->id) {
                                    std::tuple<Vector3D<float>, Vector3D<float>> corrected_positions =  correctPositions(unit, neighbour, i->pos, j->pos);
                                    Vector3D<float> i_pos = std::get<0>(corrected_positions);
                                    Vector3D<float> j_pos = std::get<1>(corrected_positions);
                                    // Get euclidean distance.
                                    float euc_dist = i_pos.dist(j_pos);
                                    // If euclidean distance is larger than the minimum we are considering, we don't care about bead j
                                    if (euc_dist > min_dist) {
                                        // std::cerr << "Euc dist = " << euc_dist << " which is too large.\n";
                                        // std::cin.get();
                                        continue;
                                    }
                                    // If bead j is water and within range, then this is not a blob and we need to move onto new bead i
                                    if (j->type == 0) {
                                        // std::cerr << j->id << " bead J is water, this is not a blob therefore\n";
                                        // std::cin.get();
                                        not_blob = true;
                                        break;
                                    }
                                    // Bead j is an oil bead. Add this to the current blob
                                    // std::cerr << j->id << " bead J is also oil. Adding it to current blob\n";
                                    // std::cin.get();
                                    current_blob.insert(j->id);
                                } else {
                                    // std::cerr << "beads I and J were the same so skipping\n";
                                    // std::cin.get();
                                }
                            }
                            if (not_blob) {
                                break;
                            }
                        }
                        if (not_blob) {
                            break;
                        }
                    }
                    if (not_blob) {
                        break;
                    }
                }
                if (not_blob) {
                    continue;
                }
                // std::cerr << i->id << " was in a blob. Adding this to the map.\n";
                // std::cin.get();
                // std::cerr << "Merging blobs\n";
                mergeBlobs(i->id, current_blob, timestep);
            }
        }
    }

    for (std::map<uint32_t, std::vector<std::set<uint32_t>>>::iterator i = blob_map.begin(); i != blob_map.end(); ++i) {
        std::cerr << "Timestep " << i->first << "\n";
        std::cerr << i->second.size() << " blobs\n";
        uint32_t beads_in_blobs = 0;
        for (std::vector<std::set<uint32_t>>::iterator j = i->second.begin(); j != i->second.end(); ++j) {
            beads_in_blobs += j->size();
        }
        std::cerr << beads_in_blobs << " beads in blobs\n";
    }

    return 0;
}
