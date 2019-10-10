// This is a parser for DMPCI input files for DPD simulations.
// DMPCI files are comprised of ordered key value pairs of parameters of a
// simulation, and as such, this parser reads the file, and then reads through
// the list and prepares a class to set up and build the universe and run it.

#ifndef _PARSER_H
#define _PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string.hpp>
#include <vector>
#include "DPDSimulation.hpp"
#include "Vector3D.hpp"
#include <random>
#include <sstream>

// Stores each line of the input file for easier reading
std::vector<std::string> lines;
// Iterator for moving through the lines
std::vector<std::string>::iterator i;
// Class to hold parameters for the simulation
DPDSimulation sim;
// Bool to reduce parsing errors based on anaylsis/sampling
bool analysisErrored = false;

// Empty lines are used to separate sections of parameters and can be ignored
std::vector<std::string>::iterator getNextLine();

// Get the title
// Strings within a DMPCI file are started and terminated with " " " (A quotation mark with a space either side)
// This needs to be detected and trimmed (in my opinion).
bool title();

// Get the date. This has a particular format.
// Also included within the DPDSimulation class is some validation of this date
// simply to check that people aren't running simulations on the 30th of February
// or the 32nd of Fleptober.
bool date();

// Comment is optional, but included in the parsing incase any future output
// of simulations is needed. Comments can be spread over multiple lines so
// looking for the terminating string tag is important here
bool comment();

// Initial state can be any 1 of 4 types. These are described in section 6 of the
// manual, which describes the file type. For now, only random is implemented, but
// any of the types can be accepted, they are just ignored.
bool initialState();

// At least one bead type is required. Theoretically, any number of them could be included
// but for the parsed implementration of DPD, we're going to assume 5 is a maximum (at least
// until we learn otherwise).
// A specific bead type struct is used, but the conservative and dissipative parts are not
// used as part of this, as these values are needed globally, and will be turned into a
// symmetical matrix for simplicity at the stage of adding them to the DPDSimulation class.
bool beadTypes();

// Bond types are optional, but any number can be declared.
// They specify what two types of beads can be connected by a bond, and
// parameters which are used in the calculations.
// Polymers are used to determine the numbers and structures of polymers (made up of many bonds).
// These just specify the values used in the calculations of those structures.
bool bondTypes();

// Stiff bonds use different calculations, based on the angle between a pair of bonds
// (hence the BondPair identifier). Again, the structure of polymers is done with the
// Polymer identifier, this simply provides the values used in calculations on bonds
// within the described polymer structures
bool stiffBondTypes();

std::string tidyString(std::string s);

// Polymers indicates the structure of bonded beads, if they are actually bonded.
// When beads are to be left unbonded, this can be done simply with " ( W ) " for example.
// Note, this still uses the "whitespace-speech mark-whitespace" to being and terminate the string.
// Particularly complex bonds are not yet implemented (as of 13/09/2019), but they will be parsed
// (in a separate function) for now, so something can be done with them in the future.
// (Assuming parsing these isn't incredibly difficult)
bool polymers();

// The definition of the dimensions of the simulation volume, and the cells which make this up
bool box();

// This is the number density, the number of beads per unit volume.
bool density();

// The temperature of the simulation. This is fixed at 1.
bool temperature();

// The RNG seed used for the random force calculations.
bool rngSeed();

// A velocity verlet parameter constant. This is set to 0.5 and not changed, but could be in the future
bool lambda();

// The delta of real time between timesteps.
bool step();

// The total number of timesteps to run for.
bool time();

// The frequency of taking samples (not yet implemented)
bool samplePeriod();

// THe frequency of taking samples for analysis as defined by the analysis parameter (Not yet implemented)
bool analysisPeriod();

// This is not used and should be the same as the time value.
bool densityPeriod();

// The frequency of emitting positional values of every bead for the purposes of displaying/visualising
bool displayPeriod();

// How often should a sample be taken of all bead information, including velocity, for restarting the simulation from this point if desired
bool restartPeriod();

// Unsure what this is for, it is currently stored for future feature improvements
bool grid();

// This stores the analysis requests as provided by the input file. No analysis information is currently implemented in this simulator
// But it is stored for future analysis updates
bool analysis();

// This stores the list of commands to be executed during the simulation. No commands are currently implemented.
// They are stored for future improvements
bool commands();

// The main function. Takes in a string, a filepath to the DMPCI file to be parsed,
// and executes, in order, all of the above functions (except helper functions)
DPDSimulation parse(std::string filepath);

#endif //__PARSER_H
