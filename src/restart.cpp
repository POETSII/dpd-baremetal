/* The feature implemented in this allows a simulation to be restarted from a
saved point. It takes in a file which holds the state of a simulation at any
timestep, and continues it from this point.
This is hard coded to use the GALS Oil and water simulation.
*/

#include <stdio.h> // Need to output to terminal
#include <stdlib.h> // Need the std libraries
#include <stdint.h> // We use different sizes of integer
#include <sys/time.h> // We are interested in how long simulations can take
#include <HostLink.h> // Allows us access to read/write from the POETS hardware
#include "dpdGALS.h" // We are forcing this to use GALS for now.
// There may come a point when the option is available at compile or runtime
// But for now, they're both about as good as eachother, and GALS is more
// interesting in how it synchronises
#include "universe.hpp" // Used to build and manipulate the simulation volume
#include <map> // Maps come in handy for storing information
#include <math.h> // Maths is quite useful
#include <iomanip> // We'll need to read in from the input file
#include <boost/algorithm/string.hpp> // Need to be able to manipulate strings
#include <assert.h> // Some assertions are needed to ensure restart works correctly

typedef struct _bond_t {
    bead_id_t head;
    bead_id_t tail;
} bond_t;

// Outputs usage instructions for this application
void print_help() {
    std::cerr << "POETS DPD simulator - POLite version\n";
    std::cerr << "Usage:\n";
    std::cerr << "./restart [--time t] [--help]\n";
    std::cerr << "\n";
    std::cerr << "time t                  - Optional integer. The number of timesteps for this sumulation to run for.\n";
    std::cerr << "                        - If not provided, a default of 10000 will be used\n";
    std::cerr << "\n";
    std::cerr << "help                    - Optional. Print this help information\n";
}

// Parse the arguments
void parse_arguments(int argc, char *argv[], uint32_t *time) {
    if (argc > 3) {
        printf("Too many arguments. Provide only the number of additional timesteps\n");
        print_help();
        exit(1);
    }

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-' && argv[i][1] == '-') {
            std::string arg(argv[i]);
            if (arg == "--help") {
                print_help();
                exit(0);
            } else if (boost::contains(arg, "--time")) {
                *time = std::stoul(argv[i+1]);
                i++;
            } else {
                std::cerr << "Unrecognised argument: " << arg << "\n";
                exit(1);
            }
        }
    }
}

// File to read the
void parse_restart_file(const std::string filePath, float *problem_size, int *N, uint32_t *timestep, std::vector<bond_t> *bonds, std::map<bead_id_t, bead_t> *beads, bead_id_t *max_bead_id) {
    // The file format I'm using at present has a strict order:
    // Volume dimensions, Timestep to restart from, Bonds and Bead data
    // This will simply read all that and store it in the passed in variables

    // File holding the simulation data to be restarted
    std::ifstream in(filePath);
    // Holds current line
    std::string line;
    // Get the first line
    std::getline(in, line);
    // First line should be dimensions
    assert(line == "VOLUME DIMENSIONS");
    // Next line should be the dimensions, comma separated
    // Get the next line
    std::getline(in, line);
    // Convert the line into a string stream so it can be split
    std::stringstream sm(line);
    // String to hold each part of the line
    std::string s;
    // Vector to hold all parts of the line
    std::vector<std::string> lines;
    // Loop through the line and split it at the commas
    while(std::getline(sm, s, ',')) {
        // Remove the whitespace
        boost::trim(s);
        // Add to the vector
        lines.push_back(s);
    }
    // We can manage a 2D or 3D simulation so lets check there are the correct number of dimensions
    uint8_t dimensions = lines.size();
    assert(dimensions >= 2 && dimensions <= 3);
    // Convert the strings to floats
    float x_dim = std::stof(lines.at(0));
    float y_dim = std::stof(lines.at(1));
    float z_dim = std::stof(lines.at(2));
    // As our simulator only allows volumes with the same length for each dimension let's check these are the same
    assert(x_dim == y_dim);
    if (dimensions == 3) {
        assert(y_dim == z_dim);
    }
    // Set the volume lengths to be this value
    *problem_size = x_dim;
    *N = static_cast<int>(x_dim);

    // Get the next line
    std::getline(in, line);
    // This next section should be the restart timestep
    assert(line == "TIMESTEP");
    // Get the next line
    std::getline(in, line);
    // This line is an integer representing the first timestep
    *timestep = std::stoi(line);

    // Get the next line
    std::getline(in, line);
    // The next section should be bonds
    assert(line == "BONDED BEAD IDS");
    // Bonds are read in the form:
    // head bead ID, tail bead ID
    // Get the next line
    std::getline(in, line);
    // Keep going until we reach the next section title
    while(line != "BEAD POSITIONS") {
        // Reuse the string stream from before
        sm.clear();
        sm.str(line);
        // Split the line at the comma
        // Reusing string s from before
        // First entry is the head bead
        std::getline(sm, s, ',');
        bead_id_t head = std::stoi(s);
        // Second entry is tail bead
        std::getline(sm, s, ',');
        bead_id_t tail = std::stoi(s);
        // Create the bond
        bond_t bond = { head, tail };
        // Add it to the list of bonds
        bonds->push_back(bond);

        //Get the next line for this while loop
        std::getline(in, line);
    }

    // Final section is bead data
    // The format is ID, type, x pos, y pos, z pos, x vel, y vel, z vel
    // Loop until the end of the file
    while (std::getline(in, line)) {
        // Reuse the string stream
        sm.clear();
        sm.str(line);
        // Split the line at the comma
        std::getline(sm, s, ',');
        // First entry is the ID
        bead_id_t id = std::stoi(s);
        // Check if this is the largest bead ID
        if (id > *max_bead_id) {
            *max_bead_id = id;
        }
        // Second entry is the type
        std::getline(sm, s, ',');
        bead_class_t type = std::stoi(s);
        // Third fourth and fifth entries are X, Y and Z positions
        std::getline(sm, s, ',');
        float x_pos = std::stof(s);
        std::getline(sm, s, ',');
        float y_pos = std::stof(s);
        std::getline(sm, s, ',');
        float z_pos = std::stof(s);
        Vector3D<float> pos(x_pos, y_pos, z_pos);
        // Sixth seventh and eighth entries are X, Y and Z velocities
        std::getline(sm, s, ',');
        float x_vel = std::stof(s);
        std::getline(sm, s, ',');
        float y_vel = std::stof(s);
        std::getline(sm, s, ',');
        float z_vel = std::stof(s);
        Vector3D<float> vel(x_vel, y_vel, z_vel);

        bead_t bead;
        bead.id = id;
        bead.type = type;
        bead.pos = pos;
        bead.velo = vel;
        bead.acc = Vector3D<float>(0.0, 0.0, 0.0);

        (*beads)[id] = bead;
    }

}

// Recursive function to find the tail of the given head in a list of bonds
// add this to the polymer vector, then find the tail of this, and so on
void find_polymer(bead_id_t *head, std::vector<bond_t> *bonds, std::vector<bead_id_t> *polymer) {
    // Search through all bonds to find where this head is
    for (std::vector<bond_t>::iterator bond = bonds->begin(); bond != bonds->end(); ++bond) {
        // When we find where this bond is the head
        if (bond->head == *head) {
            // Add the tail to the polymer
            polymer->push_back(bond->tail);
            // Then recursively find this beads tail
            find_polymer(&bond->tail, bonds, polymer);
            // This will not (currently) be the head of anything else so leave
            return;
        }
    }
    // If the bead id is not the head of anything, we will just leave this
}

// So far we only accept polymers (chains of bonded beads) with nothing too
// complex, we we search the list of bonds to generate polymers from this
std::vector<std::vector<bead_id_t>> find_all_polymers(std::vector<bond_t> *bonds) {
    // Create an empty vector of bonds to store the polymer chain head bonds
    std::vector<bond_t> polymer_heads;

    // This starts by finding any bead IDs which are only the head of a bond
    // This can be assumed to be the start of one polymer
    for (std::vector<bond_t>::iterator i = bonds->begin(); i != bonds->end(); ++i) {
        bead_id_t bead = i->head;
        bool is_tail = false;
        for (std::vector<bond_t>::iterator j = bonds->begin(); j != bonds->end(); ++j) {
            if (bead == j->tail) {
                is_tail = true;
                break;
            }
        }
        // If this was found to only be the head of any bonds
        if (!is_tail) {
            // Add the head of chain bead ID to this polymer vector
            polymer_heads.push_back(*i);
        }
    }

    // Empty list of polymers for returning
    std::vector<std::vector<bead_id_t>> polymers;

    // Now we have a list of heads of polymers we can find the tail of this
    // and then find the tail of that and so on, until there is not a tail
    for (std::vector<bond_t>::iterator head_bond = polymer_heads.begin(); head_bond != polymer_heads.end(); ++head_bond) {
        // Create an empty vector for this polymer
        std::vector<bead_id_t> polymer;
        // Add the head and tail of this bond to the vector
        polymer.push_back(head_bond->head);
        polymer.push_back(head_bond->tail);
        // Find the rest of the polymer from the tail of this bond
        find_polymer(&head_bond->tail, bonds, &polymer);
        // Add this polymer to the list of polymers
        polymers.push_back(polymer);
    }

    return polymers;

}

// Adjust bonded bead IDs so they work with POETS DPD
void adjust_bonded_bead_ids(std::vector<bond_t> *bonds, std::map<bead_id_t, bead_t> *beads, bead_id_t *max_bead_id) {
    // Bonded beads in POETS DPD are identified by:
    // A: Having an ID >= 0x80000000ul
    // B: Having neighbouring IDs, i.e. 0x80000000ul and 0x80000001ul are bonded beads
    // Based on this we need to select a max bead id.
    bead_id_t bonded_bead_id = std::max(0x80000000ul, (unsigned long)*max_bead_id + 1);

    // Get a list of the polymers so we can ensure their numbering is
    // consecutive, and larger than 0x80000000ul so they are identified as
    // bonded beads
    std::vector<std::vector<bead_id_t>> polymers = find_all_polymers(bonds);

    // Iterate through each polymer
    for (std::vector<std::vector<bead_id_t>>::iterator polymer = polymers.begin(); polymer != polymers.end(); ++polymer) {
        // Iterate through each bead in each polymer
        for (std::vector<bead_id_t>::iterator bead_id = polymer->begin(); bead_id != polymer->end(); ++bead_id) {
            // Set the bead id to the new value
            // This need only be set in the bead_t object, not in the map
            (*beads)[*bead_id].id = bonded_bead_id;
            // Ensure we keep max_bead_id up to date in case we need it later
            if (bonded_bead_id > *max_bead_id) {
                *max_bead_id = bonded_bead_id;
            }
            // Increment the id for the next bead
            bonded_bead_id++;
        }
        // Leave a gap between polymers so the application doesn't think all polymers are connected
        bonded_bead_id++;
    }

}

// Main function
int main(int argc, char *argv[]) {
    std::cout << "Starting the DPD application\n";
    // Variables for the simulation to be read from the input file or arguments
    // Simulated volumes are currently limited to the same length for each side
    float problem_size = 0; // Length of one side of the volume
    int N = 0; // Length of one side of the volume
    uint32_t restart_timestep = 0; // The first timestep of this restarted simulation
    uint32_t additional_timesteps = 10000; // The number of additional timesteps to simulate
    std::vector<bond_t> bonds; // Vector containing the bonds
    std::map<bead_id_t, bead_t> beads; // Map of bead ID to bead data
    bead_id_t max_bead_id = 0; // Maximum bead ID used to avoid having duplicate IDs

    // Parse the arguments to the application
    parse_arguments(argc, argv, &additional_timesteps);

    // The file path for the restart data
    // Hard coded for now as we just want to test the initial state from Julian's sim
    std::string restart_file = "../initialState.csv";

    std::cout << "Loading restart state from " << restart_file << "\n";
    // Get data from the input file
    parse_restart_file(restart_file, &problem_size, &N, &restart_timestep, &bonds, &beads, &max_bead_id);

    std::cout << "Volume dimensions       : " << problem_size << ", " << problem_size << ", " << problem_size << "\n";
    std::cout << "Number of beads         : " << beads.size() << "\n";
    std::cout << "Number of bonds         : " << bonds.size() << "\n";
    std::cout << "Continuing from timestep: " << restart_timestep << "\n";

    // Open file to store this state as JSON for analysis by tools in utils/
    std::string stateFilePath = "../" + std::to_string(N) + "_bond_frames/state_" + std::to_string(restart_timestep) + ".json";
    FILE* f = fopen(stateFilePath.c_str(), "w+");
    fprintf(f, "{\n\t\"beads\":[\n");

    // Build the simulation volume (universe)
    // Hard coding float for now
    Universe<float> simulation(problem_size, N, restart_timestep, restart_timestep + additional_timesteps);
    std::cout << "Universe setup -- adding beads\n";

    // Adjust bonded bead IDs so they are identified as bonded in our simulator
    adjust_bonded_bead_ids(&bonds, &beads, &max_bead_id);

    // Write each bead to the JSON file for analysis, then add each bead to the
    // simulation volume
    // For each bead
    bool first = true;
    for (std::map<bead_id_t, bead_t>::iterator i = beads.begin(); i != beads.end(); ++i) {
        if (first) {
            first = false;
        } else {
            fprintf(f, ",\n");
        }
        // Don't need the ID, just the bead
        bead_t b = i->second;
        simulation.add(&b); // This will crash if a bead cannot fit in a cell
        // Write this to the JSON so it can be used for analysis
        fprintf(f, "\t\t{\"id\":%u, \"x\":%f, \"y\":%f, \"z\":%f, \"vx\":%f, \"vy\":%f, \"vz\":%f, \"type\":%u}", b.id, b.pos.x(), b.pos.y(), b.pos.z(), b.velo.x(), b.velo.y(), b.velo.z(), b.type);
    }


    // Close the JSON file
    fprintf(f, "\n\t]\n}")
    fclose(f);

    // Store the minimum initial distances between beads to be used for analysis
    simulation.store_initial_bead_distances();

    // Write the simulation into POETS hardware memory
    simulation.write();

    std::cout << "Running...\n";
    simulation.run();

    return 0;
}
