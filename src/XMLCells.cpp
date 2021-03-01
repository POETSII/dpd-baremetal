// Holds cell information and provides access to cell states.
// Provides an interface for PGraph

#include "XMLCells.hpp"

#ifndef __XML_CELLS_IMPL
#define __XML_CELLS_IMPL

#include <iostream>

// Constructor
XMLCells::XMLCells(unsigned cells_per_dimension, ptype cell_length) : SimulationCells<std::vector<DPDState>>(cells_per_dimension, cell_length) {

    // Set the graph properties
    graph_properties.cell_length = cell_length;
    graph_properties.cells_per_dimension = cells_per_dimension;

    // Create a device for each cell
    for(uint16_t x = 0; x < cells_per_dimension; x++) {
        for(uint16_t y = 0; y < cells_per_dimension; y++) {
            for(uint16_t z = 0; z < cells_per_dimension; z++) {
                    PDeviceId id = cells.size();
                    // Create state
                    DPDState state;
                    cells.push_back(state);
                    // Create properties
                    DPDProperties props;
                    cell_properties.push_back(props);
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
XMLCells::~XMLCells() {

}

void XMLCells::write(void *dest) {

}

void XMLCells::set_start_timestep(uint32_t start_timestep) {
    graph_properties.start_timestep = start_timestep;
}

void XMLCells::set_end_timestep(uint32_t end_timestep) {
    graph_properties.max_timestep = end_timestep;
}

DPDProperties * XMLCells::get_cell_properties(PDeviceId id) {
    return &cell_properties.at(id);
}

DPDProperties * XMLCells::get_cell_properties(cell_t loc) {
    return get_cell_properties(locToId[loc]);
}

uint8_t XMLCells::get_cell_bslot(cell_t loc) {
    return this->cells.at(locToId[loc]).bslot;
}

const bead_t * XMLCells::get_bead_from_cell_slot(cell_t loc, uint8_t slot) {
    DPDState *state = &this->cells.at(locToId[loc]);
    bead_t b;
    b.id = state->bead_slot_id[slot];
    b.type = state->bead_slot_type[slot];
    b.pos.x(state->bead_slot_pos[slot][0]);
    b.pos.y(state->bead_slot_pos[slot][1]);
    b.pos.z(state->bead_slot_pos[slot][2]);
    b.velo.x(state->bead_slot_vel[slot][0]);
    b.velo.y(state->bead_slot_vel[slot][1]);
    b.velo.z(state->bead_slot_vel[slot][2]);
    b.acc.x(state->bead_slot_acc[slot][0]);
    b.acc.y(state->bead_slot_acc[slot][1]);
    b.acc.z(state->bead_slot_acc[slot][2]);
}

void XMLCells::place_bead_in_cell_slot(bead_t *b, cell_t loc, uint8_t slot) {
    DPDState *state = &this->cells.at(locToId[loc]);
    state->bead_slot_id[slot] = b->id;
    state->bead_slot_type[slot] = b->type;
    state->bead_slot_pos[slot][0] = b->pos.x();
    state->bead_slot_pos[slot][1] = b->pos.y();
    state->bead_slot_pos[slot][2] = b->pos.z();
    state->bead_slot_vel[slot][0] = b->velo.x();
    state->bead_slot_vel[slot][1] = b->velo.y();
    state->bead_slot_vel[slot][2] = b->velo.z();
    state->bead_slot_acc[slot][0] = b->acc.x();
    state->bead_slot_acc[slot][1] = b->acc.y();
    state->bead_slot_acc[slot][2] = b->acc.z();

    set_slot(state->bslot, slot); // Set the slot
    state->sentslot = state->bslot; // Set sentslot so the beads is shared at timestep 0
}

void XMLCells::addNeighbour(PDeviceId a, PDeviceId b) {
    neighbours[a].push_back(b);
}

void XMLCells::initialise_cells() {

    // Place all cell locations in its state
    for (std::map<PDeviceId, cell_t>::iterator i = this->idToLoc.begin(); i != this->idToLoc.end(); ++i) {
        // Device ID
        PDeviceId id = i->first;
        // Related cell_t identifying it's position in the volume
        cell_t loc = i->second;

        // Get its state
        DPDState *state = &this->cells.at(id);
        DPDProperties *props = &cell_properties.at(id);

        // Set its location in the volume
        props->loc[0] = loc.x;
        props->loc[1] = loc.y;
        props->loc[2] = loc.z;

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
            state->force_slot[s][0] = 0.0;
            state->force_slot[s][1] = 0.0;
            state->force_slot[s][2] = 0.0;
            state->old_vel[s][0] = 0.0;
            state->old_vel[s][1] = 0.0;
            state->old_vel[s][2] = 0.0;
        }
    }
}


#endif /* __XML_CELLS_IMPLE */
