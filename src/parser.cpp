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
#include <random>

// Stores each line of the input file for easier reading
std::vector<std::string> lines;
// Iterator for moving through the lines
std::vector<std::string>::iterator i;
// Class to hold parameters for the simulation
DPDSimulation sim;
// Bool to reduce parsing errors based on anaylsis/sampling
bool analysisErrored = false;

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
            fprintf(stderr, "ERROR: Strings must be started and terminated with \" \" \" (Whitespace, inverted comma, whitespace).\n");
            fprintf(stderr, "The title string has been ignored\n");
            return true;
        }
    } else {
        fprintf(stderr, "No title is provided. Continuing to parse without\n");
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
            fprintf(stderr, "ERROR: The date format has not been recognised. It must be in the form \"DD/MM/YY\".\n");
            fprintf(stderr, "The date will be ignored.\n");
            return true;
        }
        Date d;
        d.day = std::stoi(dates.at(0));
        d.month = std::stoi(dates.at(1));
        d.year = std::stoi(dates.at(2));
        bool valid = sim.setDate(d);
        if (!valid) {
            fprintf(stderr, "ERROR: The date is being ignored\n");
        }
        return true;
    } else {
        fprintf(stderr, "No date is provided. Continuing to parse without\n");
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
        fprintf(stderr, "No comment is provided. Continuing to parse without\n");
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
            fprintf(stderr, "ERROR: Unrecognised initial state \"%s\". Options are: \n", state.c_str());
            fprintf(stderr, "random, restart, lamella or compositelamella\n");
            exit(1);
        }
        sim.setInitialState(initState);
        return true;
    } else {
        fprintf(stderr, "ERROR: No initial state given. This is required. Options are: \n");
        fprintf(stderr, "random, restart, lamella or compositelamella\n");
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
                fprintf(stderr, "ERROR: Identifier for a bead type must be 1 alphabetic character. \"%s\"\n", beadId.c_str());
                exit(1);
            } else if (!((beadId[0] >= 'A' && beadId[0] <= 'Z') || (beadId[0] >= 'a' && beadId[0] <= 'z'))) {
                fprintf(stderr, "ERROR: Identified for a bead type must be 1 alphabetic character. \"%s\"\n", beadId.c_str());
                exit(1);
            }
            bead_type_id bead_id = beadId[0];
            // Radius of bead type
            i = std::next(i);
            std::string rad = *i;
            if (rad == "") {
                fprintf(stderr, "ERROR: No radius of bead type %s given. This must be provided.\n", beadId.c_str());
                exit(1);
            }
            boost::trim(rad);
            float radius = std::stof(rad);
            // Convervative parameters
            i = std::next(i);
            std::string con = *i;
            boost::trim(con);
            if (con == "") {
                fprintf(stderr, "ERROR: No conservative parameters of bead type %s given. This must be provided.\n", beadId.c_str());
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
                fprintf(stderr, "ERROR: Bead type %s has not been given enough conservative parameters\n. Expected %u, have %lu.\n", beadId.c_str(), (beadTypeNum + 1), conservativeParameters.size());
                exit(1);
            }
            // Dissipative paramerters
            i = std::next(i);
            std::string dis = *i;
            boost::trim(dis);
            if (dis == "") {
                fprintf(stderr, "ERROR: No dissipative parameters of bead type %s given. This must be provided.\n", beadId.c_str());
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
                fprintf(stderr, "ERROR: Bead type %s has not been given enough dissipative parameters\n. Expected %u, have %lu.\n", beadId.c_str(), (beadTypeNum + 1), dissipativeParameters.size());
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
        fprintf(stderr, "ERROR: No bead types have been given. Found \"%s\".\n", beadId.c_str());
        fprintf(stderr, "A DPD simulation is not really anything without beads.\n");
        exit(1);
    }
}

// Bond types are optional, but any number can be declared.
// They specify what two types of beads can be connected by a bond, and
// parameters which are used in the calculations.
// Polymers are used to determine the numbers and structures of polymers (made up of many bonds).
// These just specify the values used in the calculations of those structures.
bool bondTypes() {
    std::string bond = *i;
    if (boost::starts_with(bond, "Bond ")) {
        while(boost::starts_with(bond, "Bond ")) {
            // Get first bead type for bond
            bond = bond.substr(4, bond.size() - 4);
            boost::trim(bond);
            if (bond[1] != ' ') {
                fprintf(stderr, "ERROR: Given bead for bond is identified by more than one character.\n");
                fprintf(stderr, "Only one character is allowed for identifying bead types\n");
                exit(1);
            }
            bead_type_id bead1 = bond[0];
            // Get second bead type for bond
            bond = bond.substr(1, bond.size() - 1);
            boost::trim(bond);
            if (bond[1] != ' ') {
                fprintf(stderr, "ERROR: Given bead for bond is identified by more than one character.\n");
                fprintf(stderr, "Only one character is allowed for identifying bead types\n");
                exit(1);
            }
            bead_type_id bead2 = bond[0];
            // Get Hookean spring constants
            bond = bond.substr(1, bond.size() - 1);
            boost::trim(bond);
            std::stringstream sb(bond);
            std::string b;
            if (!std::getline(sb, b, ' ')) {
                fprintf(stderr, "ERROR: Expected both a hookean spring constant and an unstretched length of the spring.\n");
                fprintf(stderr, "This is for the bond with types %c and %c", bead1, bead2);
                exit(1);
            }
            float hookean_constant = std::stof(b);
            // Get unstretched length of spring
            if (!std::getline(sb, b, ' ')) {
                fprintf(stderr, "ERROR: Expected both a hookean spring constant and an unstretched length of the spring.\n");
                fprintf(stderr, "This is for the bond with types %c and %c", bead1, bead2);
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

// Stiff bonds use different calculations, based on the angle between a pair of bonds
// (hence the BondPair identifier). Again, the structure of polymers is done with the
// Polymer identifier, this simply provides the values used in calculations on bonds
// within the described polymer structures
bool stiffBondTypes() {
    std::string bond = *i;
    if (boost::starts_with(bond, "BondPair")) {
        fprintf(stderr, "WARNING: Stiff bond types (BondPair) are not yet implemented.\n");
        fprintf(stderr, "Stiff bond types will still be parsed and stored.\n");
        while (boost::starts_with(bond, "BondPair")) {
            bond = bond.substr(8, bond.size() - 8);
            boost::trim(bond);
            // Get first bead type for bond pair
            if (bond[1] != ' ') {
                fprintf(stderr, "ERROR: Given bead for stiff bond is identified by more than one character.\n");
                fprintf(stderr, "Only one character is allowed for identifying bead types\n");
                exit(1);
            }
            bead_type_id bead1 = bond[0];
            bond = bond.substr(1, bond.size() - 1);
            boost::trim(bond);
            // Get second bead type for bond pair
            if (bond[1] != ' ') {
                fprintf(stderr, "ERROR: Given bead for stiff bond is identified by more than one character.\n");
                fprintf(stderr, "Only one character is allowed for identifying bead types\n");
                exit(1);
            }
            bead_type_id bead2 = bond[0];
            bond = bond.substr(1, bond.size() - 1);
            boost::trim(bond);
            // Get third bead type for bond pair
            if (bond[1] != ' ') {
                fprintf(stderr, "ERROR: Given bead for stiff bond is identified by more than one character.\n");
                fprintf(stderr, "Only one character is allowed for identifying bead types\n");
                exit(1);
            }
            bead_type_id bead3 = bond[0];
            bond = bond.substr(1, bond.size() - 1);
            boost::trim(bond);
            // Get bending constant for stiff bond
            std::stringstream sb(bond);
            std::string b;
            if (!std::getline(sb, b, ' ')) {
                fprintf(stderr, "ERROR: Expected both a bending constant and a preferred angle of the bonds.\n");
                fprintf(stderr, "This is for the stiff bond with types %c, %c and %c", bead1, bead2, bead3);
                exit(1);
            }
            float bending_constant = std::stof(b);
            // Get preferred angle for stiff bond
            if (!std::getline(sb, b, ' ')) {
                fprintf(stderr, "ERROR: Expected both a bending constant and a preferred angle of the bonds.\n");
                fprintf(stderr, "This is for the stiff bond with types %c, %c and %c", bead1, bead2, bead3);
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

std::string tidyString(std::string s) {
    std::string r;
    bool first;
    for (int i = 0; i < s.size(); i++) {
        if (s[i] != ' ') {
            r += s[i];
            first = false;
        } else {
            if (!first) {
                r+= s[i];
                first = true;
            }
        }
    }
    boost::trim(r);
    return r;
}

// Polymers indicates the structure of bonded beads, if they are actually bonded.
// When beads are to be left unbonded, this can be done simply with " ( W ) " for example.
// Note, this still uses the "whitespace-speech mark-whitespace" to being and terminate the string.
// Particularly complex bonds are not yet implemented (as of 13/09/2019), but they will be parsed
// (in a separate function) for now, so something can be done with them in the future.
// (Assuming parsing these isn't incredibly difficult)
bool polymers() {
    std::string polymer = *i;
    float fractionTotal = 0.0;
    if (boost::starts_with(polymer, "Polymer")) {
        while (boost::starts_with(polymer, "Polymer")) {
            polymer = tidyString(polymer);
            // Clear "Polymer" identifier
            polymer = polymer.substr(7, polymer.size() - 7);
            boost::trim(polymer);
            // Get polymer name
            std::stringstream sp(polymer);
            std::string p;
            if (!std::getline(sp, p, ' ')) {
                fprintf(stderr, "ERROR: Expected a polymer name, fraction (< 1) and a structure of the polymer.\n");
                exit(1);
            }
            std::string polymerName = p;
            polymer = polymer.substr(polymerName.size(), polymer.size() - polymerName.size()); // Remove it from the string so structure is easier to grab
            boost::trim(polymer);
            // Get fraction of total beads that this polymer is to be
            if (!std::getline(sp, p, ' ')) {
                fprintf(stderr, "ERROR: Expected a polymer name, fraction (< 1) and a structure of the polymer.\n");
                exit(1);
            }
            float polymerFraction = std::stof(p);
            fractionTotal += polymerFraction;
            polymer = polymer.substr(p.size(), polymer.size() - p.size());
            boost::trim(polymer);
            // Get structure of polymer
            if (!(boost::starts_with(polymer, "\" ") && boost::ends_with(polymer, " \""))){
                fprintf(stderr, "ERROR: Polymer string must be started and ended with \" (whitespace-speech mark-whitespace)\n");
                exit(1);
            }
            std::string structure = polymer.substr(2, polymer.size() - 4);
            // Create polymer object and add to sim
            Polymer poly;
            poly.name = polymerName;
            poly.fraction = polymerFraction;
            poly.structure_string = structure;
            std::tuple<Polymer_structure, int> x = sim.parsePolymerStructure(structure, 1);
            poly.structure = std::get<0>(x);
            sim.addPolymer(poly);
            // Get next possible polymer - will return if not a polymer
            i = getNextLine();
            polymer = *i;
        }
        if (fractionTotal > 1) {
            fprintf(stderr, "ERROR: Total of fractions for beads is greater than 1. Total = %1.10f\n", fractionTotal);
            exit(1);
        }
        return false;
    } else {
        fprintf(stderr, "ERROR: At least one type of polymer must be defined, along with fraction of total beads that are this polymer, and its structure.\n");
        exit(1);
    }
}

bool box() {
    std::string box = *i;
    if (boost::starts_with(box, "Box")) {
        box = box.substr(3, box.size() - 3);
        boost::trim(box);
        std::stringstream bs(box);
        std::string b;
        Volume volume;
        if (!std::getline(bs, b, ' ')) {
            fprintf(stderr, "ERROR: Expecting the X dimension of the box\n");
            exit(1);
        }
        volume.x = std::stof(b);
        if (volume.x < 3) {
            fprintf(stderr, "ERROR: X dimension is too small. Must be 3 or larger");
            exit(1);
        }
        b = "";
        while (b == " " || b == "") {
            if (!std::getline(bs, b, ' ')) {
                fprintf(stderr, "ERROR: Expecting the Y dimension of the box\n");
                exit(1);
            }
        }
        volume.y = std::stof(b);
        if (volume.y < 3) {
            fprintf(stderr, "ERROR: Y dimension is too small. Must be 3 or larger");
            exit(1);
        }
        b = "";
        while (b == " " || b == "") {
            if (!std::getline(bs, b, ' ')) {
                fprintf(stderr, "ERROR: Expecting the Y dimension of the box\n");
                exit(1);
            }
        }
        volume.z = std::stof(b);
        if (volume.z < 3) {
            fprintf(stderr, "ERROR: Z dimension is too small. Must be 3 or larger");
            exit(1);
        }
        sim.setVolume(volume);
        // Cell dimensions (Must be set at 1 1 1)
        b = "";
        bool fail = false;
        Cell cell;
        while (b == " " || b == "") {
            if (!std::getline(bs, b, ' ')) {
                fprintf(stderr, "WARNING: No cell dimensions given, assuming 1 1 1\n");
                fail = true;
                break;
            }
        }
        if (fail) {
            cell = {1, 1, 1};
            sim.setCell(cell);
            return true;
        }
        cell.x = std::stof(b);
        b = "";
        while (b == " " || b == "") {
            if (!std::getline(bs, b, ' ')) {
                fprintf(stderr, "WARNING: No cell dimensions given, assuming 1 1 1\n");
                fail = true;
                break;
            }
        }
        if (fail) {
            cell = {1, 1, 1};
            sim.setCell(cell);
            return true;
        }
        cell.y = std::stof(b);
        b = "";
        while (b == " " || b == "") {
            if (!std::getline(bs, b, ' ')) {
                fprintf(stderr, "WARNING: No cell dimensions given, assuming 1 1 1\n");
                fail = true;
                break;
            }
        }
        if (fail) {
            cell = {1, 1, 1};
            sim.setCell(cell);
            return true;
        }
        cell.z = std::stof(b);
        if (cell.x != 1 || cell.y != 1 || cell.z != 1) {
            fprintf(stderr, "ERROR: All cell dimensions must be equal to 1.\n");
            fprintf(stderr, "Cells with larger or smaller dimensions are not yet implemented\n");
            exit(1);
        }
        sim.setCell(cell);
        Volume v = sim.getVolume();
        Cell c = sim.getCell();
        return true;
    } else {
        fprintf(stderr, "ERROR: The box dimensions must be defined, or it a simulation cannot be run\n");
        exit(1);
    }
    return false;
}

bool density() {
    std::string den = *i;
    if (boost::starts_with(den, "Density")) {
        den = den.substr(7, den.size() - 7);
        boost::trim(den);
        float density = std::stof(den);
        sim.setDensity(density);
        return true;
    } else {
        fprintf(stderr, "WARNING: No density was given, so assuming a density of 1\n");
        sim.setDensity(1);
        return false;
    }
}

bool temperature() {
    std::string temp = *i;
    if (boost::starts_with(temp, "Temp")) {
        temp = temp.substr(4, temp.size() - 4);
        boost::trim(temp);
        float temperature = std::stof(temp);
        if (temperature != 1) {
            fprintf(stderr, "WARNING: A temperature of %f was given. This value should be 1. Continuing with given temperature\n", temperature);
        }
        sim.setTemp(temperature);
        return true;
    } else {
        fprintf(stderr, "WARNING: No temperature was given, so assuming a temperature of 1\n");
        sim.setTemp(1);
        return false;
    }
}

bool rngSeed() {
    std::string seed = *i;
    if (boost::starts_with(seed, "RNGSeed")) {
        seed = seed.substr(7, seed.size() - 7);
        boost::trim(seed);
        int32_t rngSeed = std::stof(seed);
        sim.setRNGSeed(rngSeed);
        return true;
    } else {
        fprintf(stderr, "WARNING: No RNG seed was given. Randomly generating one. (Not ideal)\n");
        sim.setRNGSeed(rand());
        return false;
    }
}

bool lambda() {
    std::string lam = *i;
    if (boost::starts_with(lam, "Lambda")) {
        lam = lam.substr(6, lam.size() - 6);
        boost::trim(lam);
        float lambda = std::stof(lam);
        if (lambda != 0.5) {
            fprintf(stderr, "WARNING: Lambda was given at %f. It is best to keep this at 0.5. Setting this to 0.5 now.\n", lambda);
            lambda = 0.5;
        }
        sim.setLambda(lambda);
        return true;
    } else {
        fprintf(stderr, "WARNING: No Lambda has been given. Setting this to 0.5 and continuing.\n");
        sim.setLambda(0.5);
        return false;
    }
}

bool step() {
    std::string s = *i;
    if (boost::starts_with(s, "Step")) {
        s = s.substr(4, s.size() - 4);
        boost::trim(s);
        float step = std::stof(s);
        sim.setStep(step);
        return true;
    } else {
        fprintf(stderr, "ERROR: No step value given. This is needed. Reccomended: 0.02\n");
        exit(0);
    }
}

bool time() {
    std::string t = *i;
    if (boost::starts_with(t, "Time")) {
        t = t.substr(4, t.size() - 4);
        boost::trim(t);
        uint32_t time = std::stoul(t);
        if (time == 0) {
            fprintf(stderr, "INFO: Max time is set to 0, so this simulation will run indefinitely.\n");
        }
        sim.setTime(time);
        return true;
    } else {
        fprintf(stderr, "WARNING: No Time value is given, so this simulation will run indefinitely.\n");
        return false;
    }
}

bool samplePeriod() {
    std::string sample = *i;
    if (boost::starts_with(sample, "SamplePeriod")) {
        sample = sample.substr(12, sample.size() - 12);
        boost::trim(sample);
        uint32_t samplePeriod = std::stoul(sample);
        bool sampling = true;
        if (samplePeriod == 0) {
            fprintf(stderr, "INFO: Sample period is set to 0. No samples of the simulation will be taken.\n");
            sampling = false;
        }
        uint32_t time = sim.getTime();
        if (samplePeriod > time && time != 0) {
            fprintf(stderr, "INFO: Sample period is larger than the maximum timestep. No samples of the simulation will be taken.\n");
            sampling = false;
        }
        sim.setSamplePeriod(samplePeriod);
        if (sampling && !analysisErrored) {
            fprintf(stderr, "WARNING: Samples of analysis period are not yet implemented.\n");
            analysisErrored = true;
        }
        return true;
    } else {
        fprintf(stderr, "WARNING: No Sample period value is given. No samples of the simulation will be taken.\n");
        return false;
    }
}

bool analysisPeriod() {
    std::string analysis = *i;
    if (boost::starts_with(analysis, "AnalysisPeriod")) {
        analysis = analysis.substr(14, analysis.size() - 14);
        boost::trim(analysis);
        uint32_t analysisPeriod = std::stoul(analysis);
        bool analysing = true;
        if (analysisPeriod == 0) {
            fprintf(stderr, "INFO: Analysis period is set to 0. No analysis of the simulation will be performed.\n");
            analysing = false;
        }
        uint32_t time = sim.getTime();
        if (analysisPeriod > time && time != 0) {
            fprintf(stderr, "INFO: Analysis period is larger than the maximum timestep. No analysis of the simulation will be performed.\n");
            analysing = false;
        }
        sim.setAnalysisPeriod(analysisPeriod);
        if (analysing && !analysisErrored) {
            fprintf(stderr, "WARNING: Samples of analysis period are not yet implemented.\n");
        }
        return true;
    } else {
        fprintf(stderr, "WARNING: No analysis period value is given. No analysis of the simulation will be performed.\n");
        return false;
    }
}

bool densityPeriod() {
    std::string density = *i;
    if (boost::starts_with(density, "DensityPeriod")) {
        density = density.substr(13, density.size() - 13);
        boost::trim(density);
        uint32_t densityPeriod = std::stoul(density);
        uint32_t time = sim.getTime();
        if (densityPeriod != time) {
            fprintf(stderr, "WARNING: Density period must be equal to to the Time. Setting this to the time.\n");
            densityPeriod = time;
        }
        sim.setDensityPeriod(densityPeriod);
        return true;
    } else {
        // Density period wants to be ignored anyway, so no problem if it's not included
        return false;
    }
}

bool displayPeriod() {
    std::string d = *i;
    if (boost::starts_with(d, "DisplayPeriod")) {
        d = d.substr(13, d.size() - 13);
        boost::trim(d);
        uint32_t displayPeriod = std::stoul(d);
        uint32_t time = sim.getTime();
        if (displayPeriod > time && time != 0) {
            fprintf(stderr, "INFO: Display period is greater than the max timestep. No display information will be output.\n");
            displayPeriod = time;
        }
        if (displayPeriod == 0) {
            fprintf(stderr, "INFO: Display period is 0. No display information will be output.\n");
        }
        sim.setDisplayPeriod(displayPeriod);
        return true;
    } else {
        fprintf(stderr, "No display period is given. No display information will be output.\n");
        return false;
    }
}

bool restartPeriod() {
    std::string d = *i;
    if (boost::starts_with(d, "RestartPeriod")) {
        d = d.substr(13, d.size() - 13);
        boost::trim(d);
        uint32_t restartPeriod = std::stoul(d);
        uint32_t time = sim.getTime();
        if (restartPeriod > time && time != 0) {
            fprintf(stderr, "INFO: Restart period is greater than the max timestep. No restart states will be saved.\n");
            restartPeriod = time;
        }
        if (restartPeriod == 0) {
            fprintf(stderr, "INFO: Restart period is 0. No restart states will be saved.\n");
        }
        sim.setRestartPeriod(restartPeriod);
        fprintf(stderr, "WARNING: Restart states are not yet implemented.\n");
        return true;
    } else {
        fprintf(stderr, "No Restart period is given. No restart states will be saved.\n");
        return false;
    }
}

int main(int argc, char *argv[]) {

    if (argc < 2) {
        fprintf(stderr, "Not enough arguments. Please provide an input filepath\n");
        return 1;
    } else if (argc > 2) {
        fprintf(stderr, "Too many arguments. Please provide an input filepath ONLY\n");
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
        fprintf(stderr, "First line of input file is not \"dpd\". This is required and will not parse correctly otherwise\n");
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

    if (needToGetNextLine) {
        i = getNextLine();
    }

    needToGetNextLine = box();

    if (needToGetNextLine) {
        i = getNextLine();
    }

    needToGetNextLine = density();

    if (needToGetNextLine) {
        i = getNextLine();
    }

    needToGetNextLine = temperature();

    if (needToGetNextLine) {
        i = getNextLine();
    }

    needToGetNextLine = rngSeed();

    if (needToGetNextLine) {
        i = getNextLine();
    }

    needToGetNextLine = lambda();

    if (needToGetNextLine) {
        i = getNextLine();
    }

    needToGetNextLine = step();

    if (needToGetNextLine) {
        i = getNextLine();
    }

    needToGetNextLine = time();

    if (needToGetNextLine) {
        i = getNextLine();
    }

    needToGetNextLine = samplePeriod();

    if (needToGetNextLine) {
        i = getNextLine();
    }

    needToGetNextLine = analysisPeriod();

    if (needToGetNextLine) {
        i = getNextLine();
    }

    needToGetNextLine = densityPeriod();

    if (needToGetNextLine) {
        i = getNextLine();
    }

    needToGetNextLine = displayPeriod();

    if (needToGetNextLine) {
        i = getNextLine();
    }

    needToGetNextLine = restartPeriod();

    if (needToGetNextLine) {
        i = getNextLine();
    }

    std::cout << *i << "\n";
}