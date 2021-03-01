// Holds cell information and provides access to cell states.
// Provides an interface for PGraph

#include "POLiteCells.hpp"

#ifndef __POLITECELLS_IMPL
#define __POLITECELLS_IMPL

#include <iostream>

// Constructor
POLiteCells::POLiteCells(unsigned cells_per_dimension, ptype cell_length, uint32_t boxes_x, uint32_t boxes_y) : SimulationCells<PGraph<DPDDevice, DPDState, None, DPDMessage> *>(cells_per_dimension, cell_length) {
    // Store the members
    this->cells_per_dimension = cells_per_dimension;
    this->cell_length = cell_length;

    // Create the PGraph
    this->cells = new PGraph<DPDDevice, DPDState, None, DPDMessage>(boxes_x, boxes_y);

    // Create a device for each cell
    for(uint16_t x = 0; x < cells_per_dimension; x++) {
        for(uint16_t y = 0; y < cells_per_dimension; y++) {
            for(uint16_t z = 0; z < cells_per_dimension; z++) {
                    PDeviceId id = cells->newDevice();
                    // Update the mapping
                    cell_t loc = {x, y, z};
                    this->idToLoc[id] = loc;
                    this->locToId[loc] = id;
            }
        }
    }

    connect_cells(cells_per_dimension);

    // Larger runs will need cells mapped to DRAM instead of SRAM
  #ifdef DRAM
    this->cells->mapVerticesToDRAM = true;
    std::cout << "Mapping vertices to DRAM\n";
  #endif
    // Map to the hardware
    this->cells->map();

    initialise_cells();
}

// Destructor
POLiteCells::~POLiteCells() {
    delete cells;
}

void POLiteCells::write(void *dest) {
    HostLink *hostLink = (HostLink *)dest;
    this->cells->write(hostLink);
}

void POLiteCells::set_start_timestep(uint32_t start_timestep) {
    for (std::map<PDeviceId, cell_t>::iterator c = idToLoc.begin(); c != idToLoc.end(); ++c) {
        cells->devices[c->first]->state.timestep = start_timestep;
    }
}

void POLiteCells::set_end_timestep(uint32_t end_timestep) {
    for (std::map<PDeviceId, cell_t>::iterator c = idToLoc.begin(); c != idToLoc.end(); ++c) {
        cells->devices[c->first]->state.max_timestep = end_timestep;
    }
}

DPDState * POLiteCells::get_cell_state(PDeviceId id) {
    return &this->cells->devices[id]->state;
};

DPDState * POLiteCells::get_cell_state(cell_t loc) {
    return get_cell_state(locToId[loc]);
};

void POLiteCells::addNeighbour(PDeviceId a, PDeviceId b) {
    this->cells->addEdge(a,0,b);
}

void POLiteCells::initialise_cells() {

    // Place all cell locations in its state
    for (std::map<PDeviceId, cell_t>::iterator i = this->idToLoc.begin(); i != this->idToLoc.end(); ++i) {
        // Device ID
        PDeviceId id = i->first;
        // Related cell_t identifying it's position in the volume
        cell_t loc = i->second;

        // Get its state
        DPDState *state = &this->cells->devices[id]->state;

        // Set its location in the volume
        state->loc.x = loc.x;
        state->loc.y = loc.y;
        state->loc.z = loc.z;

        // Set the volume information in the cell
        state->cell_length = this->cell_length;
        state->cells_per_dimension = this->cells_per_dimension;

        state->mode = UPDATE; // First mode is update
        state->rngstate = 1234; // start with a seed

    #ifdef SMALL_DT_EARLY
        state->dt = early_dt;
        state->inv_sqrt_dt = early_inv_sqrt_dt;
    #endif

    #if defined(VISUALISE) && !defined(SERIAL)
        state->emitcnt = 1; // Don't emit until emitperiod has been reached
    #endif
        state->error = 0; // Error returned if something goes wrong (!= 0)

        // Clear the force and old_velo data
        for (int s = 0; s < MAX_BEADS; s++) {
            state->force_slot[s].set(0.0, 0.0, 0.0);
        #ifdef BETTER_VERLET
            state->old_velo[s].set(0.0, 0.0, 0.0);
        #endif
        }
    }
}


#endif /* __POLITECELLS_IMPLE */
