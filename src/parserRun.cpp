#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <stdint.h>
#include <string>
#include <assert.h>
#include "parser.cpp"
#include "DPDSimulation.hpp"
#include "parseUniverse.hpp"
#include "parsedDPD.h"
// #include <map>
// #include <math.h>
// #ifdef TIMER
//     #include "timer.cpp"
// #endif



void print_help() {
    std::cerr << "POETS DPD simulator - POLite version\n";
    std::cerr << "Usage:\n";
    std::cerr << "./run <DMPCI filepath> [--help]\n";
    std::cerr << "\n";
    std::cerr << "DMPCI filepath - Path to an input DMPCI file specifying a DPD simulation.\n";
    std::cerr << "\n";
    std::cerr << "help           - Optional. Print this help information and quit.\n";
}

int main(int argc, char *argv[]) {

    if (argc < 2) {
        std::cerr << "Not enough arguments. Please provide an input filepath\n";
        print_help();
        return(1);
    }

    std::string filepath;

    for (int i = 1; i < argc; i++) {
        std::string arg(argv[i]);
        if (argv[i][0] == '-' && argv[i][1] == '-') {
            std::string arg(argv[i]);
            if (arg == "--help") {
                print_help();
                return(0);
            }
        } else {
                filepath = argv[i];
        }
    }

    DPDSimulation sim = parse(filepath);
    std::cout << "\nFile parsed. Building universe.\n\n";

    Universe<ptype> uni(sim);

    uni.write(); // write the universe into the POETS memory

    // printf("running...\n");
    uni.run(); // start the simulation

    return 0;
}
