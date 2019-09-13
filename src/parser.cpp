// This is a parser for DMPCI input files for DPD simulations.
// DMPCI files are comprised of ordered key value pairs of parameters of a
// simulation, and as such, this parser reads the file, and then reads through
// the list and prepares a class to set up and build the universe and run it.

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string.hpp>
#include <vector>
#include "../inc/DPDSimulation.hpp"

// Stores each line of the input file for easier reading
std::vector<std::string> lines;
// Iterator for moving through the lines
std::vector<std::string>::iterator i;
// Class to hold parameters for the simulation
DPDSimulation sim;

// Empty lines are used to separate sections of parameters and can be ignored
std::vector<std::string>::iterator getNextLine() {
    i = std::next(i);
    while (*i == "") {
        i = std::next(i);
    }
    return i;
}

// Get the title
// Strings within a DMPCI file are started and terminated with " " " (A quotation mark with a space either side)
// This needs to be detected and trimmed (in my opinion).
bool title() {
    std::string title = *i;
    if (boost::starts_with(title, "Title")) {
        title =  title.substr(5, title.size() - 5);
        boost::trim(title);
        if (boost::starts_with(title, "\" ") && boost::ends_with(title, " \"")) {
            title = title.substr(2, title.size() - 3);
            boost::trim(title);
            sim.setTitle(title);
            return true;
        } else {
            printf("ERROR: Strings must be started and terminated with \" \" \" (Whitespace, inverted comma, whitespace).\n");
            printf("The title string has been ignored\n");
            return true;
        }
    } else {
        printf("No title is provided. Continuing to parse without\n");
        return false;
    }
}

// Get the date. This has a particular format.
// Also included within the DPDSimulation class is some validation of this date
// simply to check that people aren't running simulations on the 30th of February
// or the 32nd of Fleptober.
bool date() {
    std::string date = *i;
    if (boost::starts_with(date, "Date")) {
        date = date.substr(4, date.size() - 4);
        boost::trim(date);
        std::stringstream ss(date);
        std::string s;
        std::vector<std::string> dates;
        uint8_t c = 0;
        while (std::getline(ss, s, '/')) {
            dates.push_back(s);
        }
        if (dates.size() < 3 || dates.size() > 3) {
            printf("ERROR: The date format has not been recognised. It must be in the form \"DD/MM/YY\".\n");
            printf("The date will be ignored.\n");
            return true;
        }
        Date d;
        d.day = std::stoi(dates.at(0));
        d.month = std::stoi(dates.at(1));
        d.year = std::stoi(dates.at(2));
        bool valid = sim.setDate(d);
        if (!valid) {
            printf("ERROR: The date is being ignored\n");
        }
        return true;
    } else {
        printf("No date is provided. Continuing to parse without\n");
        return false;
    }
}

// Comment is optional, but included in the parsing incase any future output
// of simulations is needed. Comments can be spread over multiple lines so
// looking for the terminating string tag is important here
bool comment() {
    std::string comment = *i;
    if (boost::starts_with(comment, "Comment")) {
        comment = comment.substr(7, comment.size() - 7);
        boost::trim(comment);
        if (boost::starts_with(comment, "\" ") && boost::ends_with(comment, " \"")) {
            sim.setComment(comment);
            return true;
        } else if (boost::starts_with(comment, "\" ")) {
            std::string nextLine;
            do {
                i = std::next(i);
                nextLine = *i;
                boost::trim(nextLine);
                comment = comment + "\n" + nextLine;
            } while (!boost::ends_with(nextLine, "\""));
            comment = comment.substr(2, comment.size() - 2);
            comment = comment.substr(0, comment.size() - 2);
            boost::trim(comment);
            sim.setComment(comment);
            return true;
        }
    } else {
        printf("No comment is provided. Continuing to parse without\n");
        return false;
    }
}

// Initial state can be any 1 of 4 types. These are described in section 6 of the
// manual, which describes the file type. For now, only random is implemented, but
// any of the types can be accepted, they are just ignored.
bool initialState() {
    std::string state = *i;
    initial_state initState;
    if (boost::starts_with(state, "State")) {
        state = state.substr(5, state.size() - 5);
        boost::trim(state);
        if (state == "random") {
            initState = RANDOM;
        } else if (state == "restart") {
            initState = RESTART;
        } else if (state == "lamella") {
            initState = LAMELLA;
        } else if (state == "compositelamella") {
            initState = COMPOSITELAMELLA;
        } else {
            printf("ERROR: Unrecognised initial state \"%s\". Options are: \n", state.c_str());
            printf("random, restart, lamella or compositelamella\n");
            exit(1);
        }
        sim.setInitialState(initState);
        return true;
    } else {
        printf("ERROR: No initial state given. This is required. Options are: \n");
        printf("random, restart, lamella or compositelamella\n");
        exit(1);
    }
}

// At least one bead type is required. Theoretically, any number of them could be included
// but for the parsed implementration of DPD, we're going to assume 5 is a maximum (at least
// until we learn otherwise).
// A specific bead type struct is used, but the conservative and dissipative parts are not
// used as part of this, as these values are needed globally, and will be turned into a
// symmetical matrix for simplicity at the stage of adding them to the DPDSimulation class.
bool beadTypes() {
    std::string beadId = *i;
    if (boost::starts_with(beadId, "Bead")) {
        uint8_t beadTypeNum = 0;
        while (boost::starts_with(beadId, "Bead")) {
            // Character to identify a bead type
            beadId = beadId.substr(4, beadId.size() - 4);
            boost::trim(beadId);
            if (beadId.size() > 1) {
                printf("ERROR: Identifier for a bead type must be 1 aplhabetic character. \"%s\"\n", beadId.c_str());
                exit(1);
            }
            bead_type_id bead_id = beadId[0];
            // Radius of bead type
            i = std::next(i);
            std::string rad = *i;
            if (rad == "") {
                printf("ERROR: No radius of bead type %s given. This must be provided.\n", beadId.c_str());
                exit(1);
            }
            boost::trim(rad);
            float radius = std::stof(rad);
            // Convervative parameters
            i = std::next(i);
            std::string con = *i;
            boost::trim(con);
            if (con == "") {
                printf("ERROR: No conservative parameters of bead type %s given. This must be provided.\n", beadId.c_str());
                exit(1);
            }
            std::stringstream ss(con);
            std::string c;
            std::vector<float> conservativeParameters;
            while (std::getline(ss, c, ' ')) {
                float conservative = std::stof(c);
                conservativeParameters.push_back(conservative);
            }
            if (conservativeParameters.size() < (beadTypeNum + 1)) {
                printf("ERROR: Bead type %s has not been given enough conservative parameters\n. Expected %u, have %lu.\n", beadId.c_str(), (beadTypeNum + 1), conservativeParameters.size());
                exit(1);
            }
            // Dissipative paramerters
            i = std::next(i);
            std::string dis = *i;
            boost::trim(dis);
            if (dis == "") {
                printf("ERROR: No dissipative parameters of bead type %s given. This must be provided.\n", beadId.c_str());
                exit(1);
            }
            std::stringstream sd(dis);
            std::string d;
            std::vector<float> dissipativeParameters;
            while (std::getline(sd, d, ' ')) {
                float dissipative = std::stof(d);
                dissipativeParameters.push_back(dissipative);
            }
            if (dissipativeParameters.size() < (beadTypeNum + 1)) {
                printf("ERROR: Bead type %s has not been given enough dissipative parameters\n. Expected %u, have %lu.\n", beadId.c_str(), (beadTypeNum + 1), dissipativeParameters.size());
                exit(1);
            }
            // Add to sim class
            Bead_type b;
            b.radius = radius;
            b.type = beadTypeNum;
            beadTypeNum++;
            sim.addBeadType(bead_id, b);
            sim.addConservativeParameters(conservativeParameters);
            sim.addDissipativeParameters(dissipativeParameters);
            // Get next possible bead - will return if not a bead.
            i = getNextLine();
            beadId = *i;
        }
        return false;
    } else {
        printf("ERROR: No bead types have been given. Found \"%s\".\n", beadId.c_str());
        printf("A DPD simulation is not really anything without beads.\n");
        exit(1);
    }
}

bool bondTypes() {
    std::string bond = *i;
    if (boost::starts_with(bond, "Bond ")) {
        while(boost::starts_with(bond, "Bond ")) {
            // Get first bead type for bond
            bond = bond.substr(4, bond.size() - 4);
            boost::trim(bond);
            if (bond[1] != ' ') {
                printf("ERROR: Given bead for bond is identified by more than one character.\n");
                printf("Only one character is allowed for identifying bead types\n");
                exit(1);
            }
            bead_type_id bead1 = bond[0];
            // Get second bead type for bond
            bond = bond.substr(1, bond.size() - 1);
            boost::trim(bond);
            if (bond[1] != ' ') {
                printf("ERROR: Given bead for bond is identified by more than one character.\n");
                printf("Only one character is allowed for identifying bead types\n");
                exit(1);
            }
            bead_type_id bead2 = bond[0];
            // Get Hookean spring constants
            bond = bond.substr(1, bond.size() - 1);
            boost::trim(bond);
            std::stringstream sb(bond);
            std::string b;
            if (!std::getline(sb, b, ' ')) {
                printf("ERROR: Expected both a hookean spring constant and an unstretched length of the spring.\n");
                printf("This is for the bond with types %c and %c", bead1, bead2);
                exit(1);
            }
            float hookean_constant = std::stof(b);
            // Get unstretched length of spring
            if (!std::getline(sb, b, ' ')) {
                printf("ERROR: Expected both a hookean spring constant and an unstretched length of the spring.\n");
                printf("This is for the bond with types %c and %c", bead1, bead2);
                exit(1);
            }
            float spring_length = std::stof(b);
            // Create bond type and add to sim
            Bond_type bondType;
            bondType.bead1 = bead1;
            bondType.bead2 = bead2;
            bondType.hookean_constant = hookean_constant;
            bondType.spring_length = spring_length;
            sim.addBondType(bondType);
            // Get next possible bond - will return if not a bond
            i = getNextLine();
            bond = *i;
        }
        return false;
    } else {
        return false;
    }
}

bool stiffBondTypes() {
    std::string bond = *i;
    if (boost::starts_with(bond, "BondPair")) {
        printf("WARNING: Stiff bond types (BondPair) are not yet implemented.\n");
        printf("Stiff bond types will still be parsed and stored.\n");
        while (boost::starts_with(bond, "BondPair")) {
            bond = bond.substr(8, bond.size() - 8);
            boost::trim(bond);
            // Get first bead type for bond pair
            if (bond[1] != ' ') {
                printf("ERROR: Given bead for stiff bond is identified by more than one character.\n");
                printf("Only one character is allowed for identifying bead types\n");
                exit(1);
            }
            bead_type_id bead1 = bond[0];
            bond = bond.substr(1, bond.size() - 1);
            boost::trim(bond);
            // Get second bead type for bond pair
            if (bond[1] != ' ') {
                printf("ERROR: Given bead for stiff bond is identified by more than one character.\n");
                printf("Only one character is allowed for identifying bead types\n");
                exit(1);
            }
            bead_type_id bead2 = bond[0];
            bond = bond.substr(1, bond.size() - 1);
            boost::trim(bond);
            // Get third bead type for bond pair
            if (bond[1] != ' ') {
                printf("ERROR: Given bead for stiff bond is identified by more than one character.\n");
                printf("Only one character is allowed for identifying bead types\n");
                exit(1);
            }
            bead_type_id bead3 = bond[0];
            bond = bond.substr(1, bond.size() - 1);
            boost::trim(bond);
            // Get bending constant for stiff bond
            std::stringstream sb(bond);
            std::string b;
            if (!std::getline(sb, b, ' ')) {
                printf("ERROR: Expected both a bending constant and a preferred angle of the bonds.\n");
                printf("This is for the stiff bond with types %c, %c and %c", bead1, bead2, bead3);
                exit(1);
            }
            float bending_constant = std::stof(b);
            // Get preferred angle for stiff bond
            if (!std::getline(sb, b, ' ')) {
                printf("ERROR: Expected both a bending constant and a preferred angle of the bonds.\n");
                printf("This is for the stiff bond with types %c, %c and %c", bead1, bead2, bead3);
                exit(1);
            }
            float preferred_angle = std::stof(b);
            // Create stiff bond type and add to sim
            // Get next possible bond - will return if not a bond
            i = getNextLine();
            bond = *i;
        }
    } else {
        return false;
    }
}

int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("Not enough arguments. Please provide an input filepath\n");
        return 1;
    } else if (argc > 2) {
        printf("Too many arguments. Please provide an input filepath ONLY\n");
        return 1;
    }

    std::string filepath;
    filepath = argv[1];

    // Input file stream to read the input file one line at a time
    std::ifstream inputFile(filepath);
    // This string holds the current line
    std::string line;

    // Store each line of a file in a vector to iterator over that.
    // Going back and forth in a file is much more difficult
    while(std::getline(inputFile, line)) {
        boost::trim(line);
        lines.push_back(line);
    }

    i = lines.begin();

    // First line must contain "dpd" only.
    if (*i != "dpd") {
        printf("First line of input file is not \"dpd\". This is required and will not parse correctly otherwise\n");
        return 1;
    }

    i = getNextLine();
    bool needToGetNextLine = true;

    // Try to get the title
    needToGetNextLine = title();

    if (needToGetNextLine) {
        i = getNextLine();
    }

    // Try to get the date
    needToGetNextLine = date();

    if (needToGetNextLine) {
        i = getNextLine();
    }

    // Try to get the comment
    needToGetNextLine = comment();

    if (needToGetNextLine) {
        i = getNextLine();
    }

    needToGetNextLine = initialState();

    if (needToGetNextLine) {
        i = getNextLine();
    }

    needToGetNextLine = beadTypes();

    if (needToGetNextLine) {
        i = getNextLine();
    }

    needToGetNextLine = bondTypes();

    if (needToGetNextLine) {
        i = getNextLine();
    }

    needToGetNextLine = stiffBondTypes();

    if (needToGetNextLine) {
        i = getNextLine();
    }

    needToGetNextLine = polymers();
    std::cout << *i << "\n";
}