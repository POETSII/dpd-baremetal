#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <map>
#include <boost/algorithm/string.hpp>
#include <vector>
#include <sstream>
#include <fstream>

// Directory with csv for link information
const std::string link_message_dir = "../link_messages/";

// Hard code range for this quick analysis
const uint8_t minWidth = 3;
const uint8_t maxWidth = 50;

// Store times
std::map<uint8_t, double> timeMap;

// Store widths to <link to message count>
std::map<uint8_t, std::map<std::string, uint64_t>> linkMap;

// Store widths to <link to million messages per second>
std::map<uint8_t, std::map<std::string, double>> mpsMap;

// Quick and dirty get the runtimes into the map
void prepareTimeMap() {
    // Data taken from standard DPD measurements for 10000 timesteps
    // jrbeaumont has these stored in a google sheet
    timeMap[3] = 40.23528133;
    timeMap[4] = 41.44295067;
    timeMap[5] = 43.570945;
    timeMap[6] = 45.39652267;
    timeMap[7] = 47.77024267;
    timeMap[8] = 47.89371133;
    timeMap[9] = 47.65607033;
    timeMap[10] = 50.31960433;
    timeMap[11] = 52.467331;
    timeMap[12] = 51.884438;
    timeMap[13] = 51.684438;
    timeMap[14] = 53.46592933;
    timeMap[15] = 54.18004667;
    timeMap[16] = 55.229779;
    timeMap[17] = 54.34473033;
    timeMap[18] = 55.556832;
    timeMap[19] = 57.22933167;
    timeMap[20] = 57.759253;
    timeMap[21] = 59.439629;
    timeMap[22] = 61.53439133;
    timeMap[23] = 65.29526833;
    timeMap[24] = 70.96665633;
    timeMap[25] = 88.572936;
    timeMap[26] = 81.58401733;
    timeMap[27] = 99.70071567;
    timeMap[28] = 113.104124;
    timeMap[29] = 121.5693007;
    timeMap[30] = 118.9589963;
    timeMap[31] = 129.5941133;
    timeMap[32] = 147.769991;
    timeMap[33] = 160.777229;
    timeMap[34] = 167.943044;
    timeMap[35] = 165.0691907;
    timeMap[36] = 149.70544;
    timeMap[37] = 270.7716853;
    timeMap[38] = 331.166137;
    timeMap[39] = 331.9612467;
    timeMap[40] = 374.1729743;
    timeMap[41] = 343.102481;
    timeMap[42] = 320.2935853;
    timeMap[43] = 286.6598107;
    timeMap[44] = 340.4773187;
    timeMap[45] = 307.8512923;
    timeMap[46] = 408.4680033;
    timeMap[47] = 524.932759;
    timeMap[48] = 445.160348;
    timeMap[49] = 465.0305057;
    timeMap[50] = 525.543185;
}

int main(int argc, char *argv[]) {
    // Put the standard DPD times into a map for reference
    prepareTimeMap();
    // Populate link map with data from all available files
    for (uint8_t currentWidth = minWidth; currentWidth <= maxWidth; currentWidth++) {
        std::string currentFileName = link_message_dir + "link_messages_" + std::to_string(currentWidth) + ".csv";
        // Get file for current width
        std::ifstream inputFile(currentFileName);
        // Holds current line
        std::string line;
        // First line is width
        std::getline(inputFile, line);
        uint8_t width = std::stoi(line);
        assert (width == currentWidth);
        // Loop through and build maps
        while(std::getline(inputFile, line)) {
            // Used for splitting each line into separate parts
            std::stringstream ss(line);
            // Holds individual pieces of information
            std::string s;
            // First part is link ID (western/southern board to eastern/northern board)
            std::getline(ss, s, ',');
            boost::trim(s);
            std::string linkId = s;
            // Second part is total number of messages passing across the link in the whole simulation
            std::getline(ss, s, ',');
            boost::trim(s);
            uint64_t messageCount = std::stoull(s);

            linkMap[currentWidth][linkId] = messageCount;
        }
    }

    // For each width, divide every message count for every link by total run time
    // This will give messages per second for each link
    // Divide each of these by 1 million for more reasonable numbers also
    for (std::map<uint8_t, double>::iterator i = timeMap.begin(); i != timeMap.end(); ++i) {
        uint8_t width = i->first;
        double time = i->second;
        std::map<std::string, uint64_t> messageMap = linkMap[width];
        for (std::map<std::string, uint64_t>:: iterator j = messageMap.begin(); j != messageMap.end(); ++j) {
            std::string linkId = j->first;
            // Calculate million messages per second
            double mps = j->second / time / 1000000;
            mpsMap[width][linkId] = mps;
        }
    }

    // Open a new file to store all the new information in
    FILE* f = fopen("../link_messages/range_of_links.csv", "w+");

    // Format the file nicely on the top row
    fprintf(f, "Volume width, ");

    // Rest of the top line is link IDs
    uint8_t numberOfLinkIds = 0;
    for (std::map<uint8_t, std::map<std::string, double>>::iterator i = mpsMap.begin(); i != mpsMap.end(); ++i) {
        std::string output = "";
        for (std::map<std::string, double>::iterator j = i->second.begin(); j != i->second.end(); ++j) {
            output += j->first + ", ";
            numberOfLinkIds++;
        }
        // Remove trailing comma (not required for csv but I like to be neat)
        output = output.substr(0, output.length() - 2);
        // Print to file
        fprintf(f, "%s\n", output.c_str());
        // Dodgey but we only want link IDs printed once
        // And we only store them in nested maps
        break;
    }

    // Now we want to print mps values in the same order as the link IDs were just printed
    for (std::map<uint8_t, std::map<std::string, double>>::iterator i = mpsMap.begin(); i != mpsMap.end(); ++i) {
        std::string output = "";
        uint8_t width = i->first;
        output += std::to_string(width) + ", ";
        for (std::map<std::string, double>::iterator j = i->second.begin(); j != i->second.end(); ++j) {
            output += std::to_string(mpsMap[width][j->first]) + ", ";
        }
        // Remove trailing comma (not required for csv but I like to be neat)
        output = output.substr(0, output.length() - 2);
        // Print to file
        fprintf(f, "%s\n", output.c_str());
    }
    // Close the new file
    fclose(f);

}