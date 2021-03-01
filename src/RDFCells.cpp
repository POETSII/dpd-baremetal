// Holds cell information and provides access to cell states.
// For any form of cell which will be used in a DPD simulation

#ifndef __RDF_CELLS_IMPL
#define __RDF_CELLS_IMPL

#include "RDFCells.hpp"

// Constructor
RDFCells::RDFCells(unsigned cells_per_dimension, float cell_length) : Cells<std::vector<DPDState>>(cells_per_dimension, cell_length) {

    // Create a device for each cell
    for(uint16_t x = 0; x < cells_per_dimension; x++) {
        for(uint16_t y = 0; y < cells_per_dimension; y++) {
            for(uint16_t z = 0; z < cells_per_dimension; z++) {
                    PDeviceId id = cells.size();
                    DPDState state;
                    cells.push_back(state);
                    // Update the mapping
                    cell_t loc = {x, y, z};
                    this->idToLoc[id] = loc;
                    this->locToId[loc] = id;
            }
        }
    }

    initialise_cells();
}

// Destructor
RDFCells::~RDFCells() {

}

bool RDFCells::get_cell_done(cell_t loc) {
    return cells.at(locToId[loc]).done;
}

bool RDFCells::get_device_done(PDeviceId id) {
    return cells.at(id).done;
}

void RDFCells::set_cell_done(cell_t loc) {
    cells.at(locToId[loc]).done = true;
}

void RDFCells::set_device_done(PDeviceId id) {
    cells.at(id).done = true;
}

uint8_t RDFCells::get_cell_bslot(cell_t loc) {
    return cells.at(locToId[loc]).bslot;
}

uint8_t RDFCells::get_device_bslot(PDeviceId id) {
    return cells.at(id).bslot;
}

const bead_t * RDFCells::get_bead_from_cell_slot(cell_t loc, uint8_t slot) {
    return &cells.at(locToId[loc]).bead_slot[slot];
}

const bead_t * RDFCells::get_bead_from_device_slot(PDeviceId id, uint8_t slot) {
    return &cells.at(id).bead_slot[slot];
}

void RDFCells::place_bead_in_cell_slot(bead_t *b, cell_t loc, uint8_t slot) {
    DPDState *state = &cells.at(locToId[loc]);
    state->bead_slot[slot] = *b; // Add the bead
    state->bslot = set_slot(state->bslot, slot); // Set the slot
}

void RDFCells::place_bead_in_device_slot(bead_t *b, PDeviceId id, uint8_t slot) {
    DPDState *state = &cells.at(id);
    state->bead_slot[slot] = *b; // Add the bead
    state->bslot = set_slot(state->bslot, slot); // Set the slot
}

void RDFCells::initialise_cells() {
    for (uint8_t x = 0; x < this->cells_per_dimension; x++) {
        for (uint8_t y = 0; y < this->cells_per_dimension; y++) {
            for (uint8_t z = 0; z < this->cells_per_dimension; z++) {
                cell_t loc = {x, y, z};
                DPDState *state = &cells.at(locToId[loc]);
                state->loc.x = loc.x;
                state->loc.y = loc.y;
                state->loc.z = loc.z;
                state->done = false;
            }
        }
    }
}

#endif /* __RDF_CELLS_IMPL */
