#if defined(OUTPUT_MAPPING) || defined(MESSAGE_COUNTER)
    typedef struct _FPGALinks {
        std::vector<std::vector<uint64_t>> x;
        std::vector<std::vector<uint64_t>> y;
        std::vector<std::vector<uint64_t>> intra;
    } FPGALinks;

#endif

#if defined(MESSAGE_COUNTER) || defined(OUTPUT_MAPPING)
    FPGALinks _link_messages, _link_edges;
#endif

// Prepare variables for message counting and output mapping
// #if defined(MESSAGE_COUNTER) || defined(OUTPUT_MAPPING)
//     // Prep link 2D vectors
//     _link_messages.x = std::vector<std::vector<uint64_t>>(6);
//     _link_messages.y = std::vector<std::vector<uint64_t>>(6);
//     _link_messages.intra = std::vector<std::vector<uint64_t>>(6);
//     _link_edges.x = std::vector<std::vector<uint64_t>>(6);
//     _link_edges.y = std::vector<std::vector<uint64_t>>(6);
//     _link_edges.intra = std::vector<std::vector<uint64_t>>(6);
//     for (int i = 0; i < 6; i++) {
//         _link_messages.x.at(i).resize(8);
//         _link_messages.y.at(i).resize(8);
//         _link_messages.intra.at(i).resize(8);
//         _link_edges.x.at(i).resize(8);
//         _link_edges.y.at(i).resize(8);
//         _link_edges.intra.at(i).resize(8);
//     }
// #endif

void clearLinks(FPGALinks* links) {
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 8; j++) {
            links->x.at(i).at(j) = 0;
            links->y.at(i).at(j) = 0;
            links->intra.at(i).at(j) = 0;
        }
    }
}

// Find the link and add an edge to it
void followEdge(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, FPGALinks* links) {
    if (x0 == x1 && y0 == y1) {
        links->intra.at(x0).at(y0)++;
        return;
    }

    while (x0 != x1) {
        if (x0 > x1) {
            links->x.at(x0 - 1).at(y0)++;
            x0--;
        } else if (x0 < x1) {
            links->x.at(x0).at(y0)++;
            x0++;
        }
    }
    while (y0 != y1) {
        if (y0 > y1) {
            links->y.at(x0).at(y0 - 1)++;
            y0--;
        } else if (y0 < y1) {
            links->y.at(x0).at(y0)++;
            y0++;
        }
    }
}

// Find the number of edges which cross links
void updateLinkInfo(PThreadId c_thread, cell_t c_loc, FPGALinks* links) {

    uint32_t xmask = ((1<<TinselMeshXBits)-1);
    // Get FPGA coordinates of origin cell
    uint32_t c_FPGA_y = c_thread >> (TinselLogThreadsPerBoard + TinselMeshXBits);
    uint32_t c_FPGA_x = (c_thread >> TinselLogThreadsPerBoard) & xmask;
    //Loop through all neighbours
    for (int16_t x_off = -1; x_off <= 1; x_off++) {
        for (int16_t y_off = -1; y_off <= 1; y_off++) {
            for (int16_t z_off = -1; z_off <= 1; z_off++) {
                if (x_off == 0 && y_off == 0 && z_off == 0) {
                    continue;
                }
                // Get neighbour location
                cell_t n_loc;
                n_loc.x = locOffset(c_loc.x, x_off, _D);
                n_loc.y = locOffset(c_loc.y, y_off, _D);
                n_loc.z = locOffset(c_loc.z, z_off, _D);

                PDeviceId nId = this->locToId[n_loc];
                PDeviceAddr n_addr = _volume->toDeviceAddr[nId];
                PThreadId n_thread = getThreadId(n_addr);
                uint32_t n_FPGA_y = n_thread >> (TinselLogThreadsPerBoard + TinselMeshXBits);
                uint32_t n_FPGA_x = (n_thread >> TinselLogThreadsPerBoard) & xmask;

                followEdge(c_FPGA_x, c_FPGA_y, n_FPGA_x, n_FPGA_y, links);
            }
        }
    }
}

uint16_t locOffset(const uint16_t current, const int16_t offset, const float vol_max) {
    if (offset == -1 && current == 0) {
        return vol_max - 1;
    } else if (offset == 1 && current == vol_max - 1) {
        return 0;
    } else {
        return current + offset;
    }
}

// Output mapping info as JSON
void outputMapping() {
    // Number of edges using the links in x and y direction for FPGAs
    clearLinks(&_link_edges);
    // JSON file
    std::string fileName = "../mapping/DPD_mapping_" + std::to_string(_D) + "_" + std::to_string(_D) + "_" + std::to_string(_D) + ".json";
    std::string output = "";
    // Open JSON
    output = "{\n";
    output += "\t\"vertices\": {\n";
    for(std::map<PDeviceId, cell_t>::iterator i = this->idToLoc.begin(); i != this->idToLoc.end(); ++i) {
        PDeviceId cId = i->first;
        cell_t loc = i->second;
        std::string cellName = "\"cell_" + std::to_string(loc.x) + "_" + std::to_string(loc.y) + "_" + std::to_string(loc.z)+"\"";
        PDeviceAddr cellAddr = cells->toDeviceAddr[cId];
        PThreadId cellThread = getThreadId(cellAddr);
        output += "\t\t" + cellName +": " + std::to_string(cellThread) + ", \n";
        updateLinkInfo(cellThread, loc, &_link_edges);
    }
    // Remove trailing comma
    output = output.substr(0, output.length() - 3);
    // Close vertices section
    output += "\n\t},\n";
    // Open links section
    output += "\t\"links\": [\n";
    // For each FPGA print link information for it's x (east) and y (south) link.
    for (int i = 0; i < 6; i++) {
        output += "\t\t[ ";
        for (int j = 0; j < 8; j++) {
            uint64_t link_e = _link_edges.x.at(i).at(j);
            uint64_t link_n = _link_edges.y.at(i).at(j);
            output += "{\"x\": ";
            output += std::to_string(link_e);
            output += ", \"y\": ";
            output += std::to_string(link_n);
            output += "}, ";
        }
        output = output.substr(0, output.length() - 2);
        output += " ],\n";
    }
    output = output.substr(0, output.length() - 2);
    // Close links section
    output += "\n\t]\n";
    // Close JSON
    output += "}\n";
    // Write to file
    FILE* f = fopen(fileName.c_str(), "w+");
    fprintf(f, "%s", output.c_str());
    fclose(f);
    // File closed
    exit(0);
}

void calculateMessagesPerLink(std::map<cell_t, uint32_t> cell_messages) {
    clearLinks(&_link_messages);
    for (std::map<cell_t, uint32_t>::iterator i = cell_messages.begin(); i != cell_messages.end(); ++i) {
        clearLinks(&_link_edges);
        cell_t loc = i->first;
        uint32_t cellId = this->locToId[loc];
        PDeviceAddr cellAddr = _volume->toDeviceAddr[cellId];
        PThreadId cellThread = getThreadId(cellAddr);
        uint32_t messages = i->second;
        updateLinkInfo(cellThread, loc, &_link_edges);
        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 8; j++) {
                if (_link_edges.x.at(i).at(j) > 0) {
                    uint32_t messages_before_x = _link_messages.x.at(i).at(j);
                    _link_messages.x.at(i).at(j) += (messages * _link_edges.x.at(i).at(j));
                    // Overflow check - if messages_before is less, we've gone back to 0
                    assert(messages_before_x <= _link_messages.x.at(i).at(j));
                }
                if (_link_edges.y.at(i).at(j) > 0) {
                    uint32_t messages_before_y = _link_messages.y.at(i).at(j);
                    _link_messages.y.at(i).at(j) += (messages * _link_edges.y.at(i).at(j));
                    // Overflow check - if messages_before is less, we've gone back to 0
                    assert(messages_before_y <= _link_messages.y.at(i).at(j));
                }
                if (_link_edges.intra.at(i).at(j) > 0) {
                    uint32_t messages_before_intra = _link_messages.intra.at(i).at(j);
                    _link_messages.intra.at(i).at(j) += (messages * _link_edges.intra.at(i).at(j));
                    // Overflow check - if messages_before is less, we've gone back to 0
                    assert(messages_before_intra <= _link_messages.intra.at(i).at(j));
                }
            }
        }
    }

    // DEBUG: Print link message numbers to screen - link (0, 0) is bottom left corner of printed
    for (int j = 7; j >= 0; j--) {
        for (int i = 0; i < 6; i++) {
            std::cout <<_link_messages.intra.at(i).at(j) << " (" << _link_messages.x.at(i).at(j) << ", " << _link_messages.y.at(i).at(j) << "), ";
        }
        std::cout << "\n";
    }
    std::string s = "../link_messages/link_messages_" + std::to_string(_D) + ".csv";
    FILE* f = fopen(s.c_str(), "w+");

    fprintf(f, "%u\n", _D);
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 8; j++) {
            std::string intra_name = "intra (" + std::to_string(i) + " " + std::to_string(j) + ")";
            std::string x_name = "(" + std::to_string(i) + " " + std::to_string(j) + ")E - W(" + std::to_string(i+1) + " " + std::to_string(j) + ")";
            std::string y_name = "(" + std::to_string(i) + " " + std::to_string(j) + ")N - S(" + std::to_string(i) + " " + std::to_string(j+1) + ")";

            fprintf(f, "%s, %lu\n", intra_name.c_str(), _link_messages.intra.at(i).at(j));

            if (i < 5) {
                fprintf(f, "%s, %lu\n", x_name.c_str(), _link_messages.x.at(i).at(j));
            }
            if (j < 7) {
                fprintf(f, "%s, %lu\n", y_name.c_str(), _link_messages.y.at(i).at(j));
            }
        }
    }

    fclose(f);
}
