// Dissipative particle dynamics simulation serial x86 version.
// This is aimed at simulating DPD systems using the same code as the POLite
// version but serially on an x86 machine. This way, it will produce the same
// results, but we are not restricted by message sizes or instruction space
// and so can add traces and debugging to determine any issues with the POETS
// DPD code.

#include "serial.hpp"

// Create a new cell
// Return the ID of this cell
PDeviceId SerialSim::newCell() {
    DPDState s;
    _cells.push_back(s);
    return _num_cells++;
}
// Get a pointer to a cell
DPDState* SerialSim::getCell(PDeviceId id) {
    return &_cells.at(id);
}

// Add cell b as a neighbour to cell a
void SerialSim::addEdge(PDeviceId a, PDeviceId b) {
    DPDState *state_a = getCell(a);
    uint8_t num_neighbours = state_a->num_neighbours;
    state_a->neighbours[num_neighbours] = b;
}

int run() {

   return 0;
}
