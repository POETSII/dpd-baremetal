// Implementation file for the host simulation volume class

#include "Volume.hpp"

#ifndef __VOLUME_IMPL
#define __VOLUME_IMPL

// Print out the occupancy of each device
template<class S>
void Volume<S>::print_occupancy() {
    // Loop through all devices in the volume and print their number of particles assigned
    printf("DeviceId\t\tbeads\n--------------\n");
    for(auto const& x : idToLoc) {
        PDeviceId t = x.first;
      #if defined(SERIAL) || defined(RDF)
        uint8_t beads = get_num_beads(cells.at(t)->bslot);
      #else
        uint8_t beads = get_num_beads(cells->devices[t]->state.bslot);
      #endif
        if(beads > 0)
            printf("%x\t\t\t%d\n", t, (uint32_t)beads);
    }
}

// Constructor
template<class S>
Volume<S>::Volume(S volume_length, unsigned cells_per_dimension) {
    this->volume_length = volume_length;
    this->cells_per_dimension = cells_per_dimension;
    this->cell_length = this->volume_length / S(this->cells_per_dimension);
    std::cout << "Volume length in = " << volume_length << ". Volume length stored = " << this->volume_length << "\n";

#ifndef SERIAL
    // cells = new PGraph<DPDDevice, DPDState, None, DPDMessage>(_boxesX, _boxesY);
    cells = new PGraph<DPDDevice, DPDState, None, DPDMessage>();
#endif

    // Create the cells
    for(uint16_t x = 0; x < cells_per_dimension; x++) {
        for(uint16_t y = 0; y < cells_per_dimension; y++) {
            for(uint16_t z = 0; z < cells_per_dimension; z++) {
                  #if defined(SERIAL) || defined(RDF)
                    PDeviceId id = cells.newCell();
                  #else
                    PDeviceId id = cells->newDevice();
                  #endif
                    // Update the mapping
                    cell_t loc = {x, y, z};
                    idToLoc[id] = loc;
                    locToId[loc] = id;

                    // Include the cell's location in its state
                  #if defined(SERIAL) || defined(RDF)
                    DPDState *state = cells.at(id);
                  #else
                    DPDState *state = &cells->devices[id]->state;
                  #endif
                    state->loc.x = loc.x;
                    state->loc.y = loc.y;
                    state->loc.z = loc.z;
            }
        }
    }
}

// Deconstructor
template<class S>
Volume<S>::~Volume() {
// #ifndef SERIAL
    delete cells;
// #endif
}

// add a bead to the simulation volume
template<class S>
cell_t Volume<S>::add_bead(const bead_t *in) {
    bead_t b = *in;
    cell_pos_t x = floor(b.pos.x()/cell_length);
    cell_pos_t y = floor(b.pos.y()/cell_length);
    cell_pos_t z = floor(b.pos.z()/cell_length);
    cell_t t = {x,y,z};

    // Lookup the device
    PDeviceId b_su = locToId[t];

    // Get the devices state
#ifdef SERIAL
    DPDState *state = cells.at(b_su);
#else
    DPDState *state = &cells->devices[b_su]->state;
#endif

    // Check to make sure there is still enough room in the device
    if (get_num_beads(state->bslot) > MAX_BEADS) {
        std::cerr << "Error: there is not enough space in cell: " << t.x << ", " << t.y << ", " << t.z << " for bead: " << in->id << ".\n";
        std::cerr << "There is already " << get_num_beads(state->bslot) << " beads in this cell for a max of\n";
        fflush(stdout);
        exit(EXIT_FAILURE);
    } else {
        // We can add the bead

        // Make the position of the bead relative to the cell (0.0 - < 1.0)
        b.pos.x(b.pos.x() - (S(float(t.x)) * cell_length));
        b.pos.y(b.pos.y() - (S(float(t.y)) * cell_length));
        b.pos.z(b.pos.z() - (S(float(t.z)) * cell_length));

        // Get the next free slot in this device
        uint8_t slot = get_next_free_slot(state->bslot);
        state->bead_slot[slot] = b;
        state->bslot = set_slot(state->bslot, slot);

        beads_added++;
    }

    return t;
}

template<class S>
void Volume<S>::add_bead_to_cell(const bead_t *in, const cell_t cell) {
    bead_t b = *in;

    if (b.pos.x() > cell_length || b.pos.y() > cell_length || b.pos.z() > cell_length) {
        printf("Error: Bead position given (%f, %f, %f) is outside the bounds of this cell (%d, %d, %d) which has side length %f\n", b.pos.x(), b.pos.y(), b.pos.z(), cell.x, cell.y, cell.z, cell_length);
        fflush(stdout);
        exit(EXIT_FAILURE);
    }

    // Lookup the device
    PDeviceId b_su = locToId[cell];

    // Get the device state
#ifdef SERIAL
    DPDState *state = cells.getCell(b_su);
#else
    DPDState *state = &cells->devices[b_su]->state;
#endif

    // Get the next free slot in this device
    uint8_t slot = get_next_free_slot(state->bslot);
    state->bead_slot[slot] = b;
    state->bslot = set_slot(state->bslot, slot);
    beads_added++;
}

#endif /* __VOLUME_IMPL */
