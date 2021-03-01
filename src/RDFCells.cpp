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

// Get state of cell from its device ID
DPDState * RDFCells::get_cell_state(PDeviceId id) {
    return &cells.at(id);
}

// Get state of cell from its location
DPDState * RDFCells::get_cell_state(cell_t loc) {
    return get_cell_state(locToId[loc]);
}

void RDFCells::initialise_cells() {
    for (uint8_t x = 0; x < this->cells_per_dimension; x++) {
        for (uint8_t y = 0; y < this->cells_per_dimension; y++) {
            for (uint8_t z = 0; z < this->cells_per_dimension; z++) {
                cell_t loc = {x, y, z};
                DPDState *state = get_cell_state(loc);
                state->loc.x = loc.x;
                state->loc.y = loc.y;
                state->loc.z = loc.z;
                state->done = false;
            }
        }
    }
}

#endif /* __RDF_CELLS_IMPL */
