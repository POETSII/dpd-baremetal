#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <map>
#include "../inc/Vector3D.hpp"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <boost/algorithm/string.hpp>
#include <vector>
#include <sstream>

int main() {

    uint32_t uni_len = 10;

    std::string line;

    std::ostringstream oss;
    oss << "../perf-results/force_update_bead_positions_" << uni_len << ".csv";
    // std::string force_update_bead_file = "../perf-results/force_update_bead_positions_10.csv";
    std::cerr << "Loading beads from " << oss.str().c_str() << "\n";
    std::ifstream force_update_beads(oss.str().c_str());
    std::map<uint32_t, std::map<uint32_t, Vector3D<float>>> force_update_map;

    // Loop through and add the beads to the map
    while(std::getline(force_update_beads, line)) {
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
        for (uint32_t i = 1; i < lines.size(); i += 4) {
            // Get bead id
            uint32_t id = std::stoi(lines.at(i));
            // Get bead position
            Vector3D<float> pos;
            pos.set(std::stof(lines.at(i + 1)), std::stof(lines.at(i + 2)), std::stof(lines.at(i + 3)));
            force_update_map[timestep][id] = pos;
        }
    }

    std::string accelerator_bead_file = "../perf-results/accelerator_bead_positions_10.csv";
    std::cerr << "Loading beads from " << accelerator_bead_file << "\n";
    std::ifstream accelerator_beads(accelerator_bead_file);
    std::map<uint32_t, std::map<uint32_t, Vector3D<float>>> accelerator_map;

    std::map<uint32_t, std::map<uint32_t, Vector3D<float>>> difference_map;
    std::map<uint32_t, std::map<uint32_t, float>> euc_dist_map;

    // Loop through and add the beads to the map
    while(std::getline(accelerator_beads, line)) {
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
        for (uint32_t i = 1; i < lines.size(); i += 4) {
            // Get bead id
            uint32_t id = std::stoi(lines.at(i));
            // Get bead position
            Vector3D<float> pos;
            pos.set(std::stof(lines.at(i + 1)), std::stof(lines.at(i + 2)), std::stof(lines.at(i + 3)));
            accelerator_map[timestep][id] = pos;

            Vector3D<float> force_update_pos = force_update_map[timestep][id];

            Vector3D<float> difference;
            difference = force_update_pos - pos;

            if (difference.x() < -2) {
                difference.set(difference.x() + uni_len, difference.y(), difference.z());
            } else if (difference.x() > 2) {
                difference.set(difference.x() - uni_len, difference.y(), difference.z());
            }

            if (difference.y() < -2) {
                difference.set(difference.x(), difference.y() + uni_len, difference.z());
            } else if (difference.y() > 2) {
                difference.set(difference.x(), difference.y() - uni_len, difference.z());
            }

            if (difference.z() < -2) {
                difference.set(difference.x(), difference.y(), difference.z() + uni_len);
            } else if (difference.z() > 2) {
                difference.set(difference.x(), difference.y(), difference.z() - uni_len);
            }

            float euc_distance = difference.mag();
            difference_map[timestep][id] = difference;
            euc_dist_map[timestep][id] = euc_distance;

        }
    }

// Average difference for each timestep
    std::map<uint32_t, Vector3D<float>> average_difference_map;
    for (std::map<uint32_t, std::map<uint32_t, Vector3D<float>>>::iterator i = difference_map.begin(); i != difference_map.end(); ++i) {
        uint32_t timestep = i->first;
        Vector3D<float> accumulator;
        for (std::map<uint32_t, Vector3D<float>>::iterator j = i->second.begin(); j != i->second.end(); ++j) {
            accumulator = accumulator + j->second;
        }
        average_difference_map[timestep] = accumulator / i->second.size();
    }

// Average euclidian distance for each timestep
    std::map<uint32_t, float> average_euc_dist_map;
    std::ostringstream out_ss;
    out_ss << "../perf-results/averages_" << uni_len << ".csv";
    FILE* outFile = fopen(out_ss.str().c_str(), "w");
    fprintf(outFile, "Timestep, Average Diff x, Average Diff y, Average Diff z, Diff Variance x, Diff Variance y, Diff Variance z, Average Euclidean distance, Euclidean Distance Variance\n");
    for (std::map<uint32_t, std::map<uint32_t, float>>::iterator i = euc_dist_map.begin(); i != euc_dist_map.end(); ++i) {
        uint32_t timestep = i->first;
        float accumulator;
        for (std::map<uint32_t, float>::iterator j = i->second.begin(); j != i->second.end(); ++j) {
            accumulator += j->second;
        }
        average_euc_dist_map[timestep] = accumulator / i->second.size();
    }
    // fclose(outFile);

// Variance for difference for each timestep
    std::map<uint32_t, Vector3D<float>> diff_variance_map;
    for (std::map<uint32_t, std::map<uint32_t, Vector3D<float>>>::iterator i = difference_map.begin(); i != difference_map.end(); ++i) {
        uint32_t timestep = i->first;
        Vector3D<float> average = average_difference_map[timestep];
        Vector3D<float> accumulator;
        for (std::map<uint32_t, Vector3D<float>>::iterator j = i->second.begin(); j != i->second.end(); ++j) {
            accumulator = accumulator + (j->second - average) * (j->second - average);
        }
        diff_variance_map[timestep] = accumulator / i->second.size();
    }

// Variance for euclidian distance for each timestep
    std::map<uint32_t, float> euc_dist_variance_map;
    for (std::map<uint32_t, std::map<uint32_t, float>>::iterator i = euc_dist_map.begin(); i != euc_dist_map.end(); ++i) {
        uint32_t timestep = i->first;
        float average = average_euc_dist_map[timestep];
        float accumulator;
        for (std::map<uint32_t, float>::iterator j = i->second.begin(); j != i->second.end(); ++j) {
            accumulator = accumulator + (j->second - average) * (j->second - average);
        }
        euc_dist_variance_map[timestep] = accumulator / i->second.size();
        fprintf(outFile, "%u, %1.20f, %1.20f, %1.20f, %1.20f, %1.20f, %1.20f, %1.20f, %1.20f\n", timestep, average_difference_map[timestep].x(), average_difference_map[timestep].y(), average_difference_map[timestep].z(), diff_variance_map[timestep].x(), diff_variance_map[timestep].y(), diff_variance_map[timestep].z(), average_euc_dist_map[timestep], euc_dist_variance_map[timestep]);
    }

    fclose(outFile);
    return 0;
}
