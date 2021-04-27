// Holds cell information and provides access to cell states.
// Provides an interface for PGraph

#include "SerialCells.hpp"

#ifndef __SERIALCELLS_IMPL
#define __SERIALCELLS_IMPL

#include <iostream>
#include <cassert>

// Constructor
SerialCells::SerialCells(unsigned cells_per_dimension, ptype cell_length) : SimulationCells<std::vector<DPDState> *>(cells_per_dimension, cell_length) {
    // Create the vector
    this->cells = new std::vector<DPDState>();

    // Create a device for each cell
    for(uint16_t x = 0; x < cells_per_dimension; x++) {
        for(uint16_t y = 0; y < cells_per_dimension; y++) {
            for(uint16_t z = 0; z < cells_per_dimension; z++) {
                    PDeviceId id = newDevice();
                    // Update the mapping
                    cell_t loc = {x, y, z};
                    this->idToLoc[id] = loc;
                    this->locToId[loc] = id;
            }
        }
    }

    connect_cells(cells_per_dimension);

    initialise_cells();
}

// Destructor
SerialCells::~SerialCells() {
    delete cells;
}

// Dummy - Writing to the simulator isn't necessary
void SerialCells::write(void *dest) {

}

void SerialCells::set_start_timestep(uint32_t start_timestep) {
  #if SMALL_DT_EARLY
    float dt, inv_sqrt_dt;
    if (start_timestep >= 1000) {
        dt = normal_dt;
        inv_sqrt_dt = normal_inv_sqrt_dt;
    } else {
        dt = early_dt;
        inv_sqrt_dt = early_inv_sqrt_dt;
    }
  #endif
    // Timestep is global for serial simulator
    this->timestep = start_timestep;
    // Small DT early is passed in the state
    // so this needs setting in each cell's state
  #if SMALL_DT_EARLY
    for (std::map<PDeviceId, cell_t>::iterator c = idToLoc.begin(); c != idToLoc.end(); ++c) {
        cells->at(c->first).dt = dt;
        cells->at(c->first).inv_sqrt_dt = inv_sqrt_dt;
    }
  #endif
}

void SerialCells::set_end_timestep(uint32_t end_timestep) {
    this->max_timestep = end_timestep;
}

DPDState *SerialCells::get_cell_state(PDeviceId id) {
    return &this->cells->at(id);
}

uint32_t SerialCells::get_timestep() {
    return this->timestep;
}

void SerialCells::increment_timestep() {
    this->timestep++;
}

uint8_t SerialCells::get_cell_bslot(cell_t loc) {
    return cells->at(locToId[loc]).bslot;
}

uint8_t SerialCells::get_device_bslot(PDeviceId id) {
    return cells->at(id).bslot;
}

const bead_t * SerialCells::get_bead_from_cell_slot(cell_t loc, uint8_t slot) {
    return &cells->at(locToId[loc]).bead_slot[slot];
}

const bead_t * SerialCells::get_bead_from_device_slot(PDeviceId id, uint8_t slot) {
    return &cells->at(id).bead_slot[slot];
}

void SerialCells::place_bead_in_cell(bead_t *b, cell_t loc) {
    place_bead_in_device(b, locToId[loc]);
}

void SerialCells::place_bead_in_device(bead_t *b, PDeviceId id) {
    DPDState *state = &cells->at(id);
    uint8_t new_slot = get_next_free_slot(state->bslot);
    if (new_slot == 0xFF) {
        std::cerr << "Error: There were no free slots left in cell " << idToLoc[id].x << ", " << idToLoc[id].y << ", " << idToLoc[id].z << "\n";
        exit(1);
    }
    state->bead_slot[new_slot] = *b; // Add the bead
    state->bslot = set_slot(state->bslot, new_slot); // Set the slot in the bitmap
    state->sentslot = state->bslot; // To make sure the bead is sent at timestep 0
}

// ************* Condition functions ***************
bool SerialCells::reached_max_timestep() {
    return this->timestep >= this->max_timestep;
}

// *************** Protected functions ***********

PDeviceId SerialCells::newDevice() {
    DPDState s;
    this->cells->push_back(s);
    return number_of_cells++;
}

void SerialCells::addNeighbour(PDeviceId a, PDeviceId b) {
    // Push b's device ID onto a's neighbour vector
    this->cells->at(a).neighbours.push_back(a);
    assert(this->cells->at(a).neighbours.size() <= NEIGHBOURS);
}

void SerialCells::initialise_cells() {

    // Place all cell locations in its state
    for (std::map<PDeviceId, cell_t>::iterator i = this->idToLoc.begin(); i != this->idToLoc.end(); ++i) {
        // Device ID
        PDeviceId id = i->first;
        // Related cell_t identifying it's position in the volume
        cell_t loc = i->second;

        // Get its state
        DPDState *state = &this->cells->at(id);

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

        // Clear the force and old_velo data
        for (int s = 0; s < MAX_BEADS; s++) {
            state->force_slot[s].set(0.0, 0.0, 0.0);
        #ifdef BETTER_VERLET
            state->old_velo[s].set(0.0, 0.0, 0.0);
        #endif
        }
    }
}


#endif /* __SERIALCELLS_IMPL */
