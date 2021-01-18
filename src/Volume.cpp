// Implementation file for the host simulation volume class

#include "Volume.hpp"

#ifndef __VOLUME_IMPL
#define __VOLUME_IMPL

// Print out the occupancy of each device
template<class S>
void Volume<S>::print_occupancy() {
    // Loop through all devices in the volume and print their number of particles assigned
    printf("DeviceId\t\tbeads\n--------------\n");
    for(auto const& x : _idToLoc) {
        PDeviceId t = x.first;
      #if defined(SERIAL) || defined(RDF)
        uint8_t beads = get_num_beads(_cells.at(t)->bslot);
      #else
        uint8_t beads = get_num_beads(_cells->devices[t]->state.bslot);
      #endif
        if(beads > 0)
            printf("%x\t\t\t%d\n", t, (uint32_t)beads);
    }
}

// Constructor
template<class S>
Volume<S>::Volume(S volume_length, unsigned cells_per_dimension) {
    _vol_length = vol_length;
    _cells_per_dimension = cells_per_dimension;
    _cell_length = _volume_length / S(D);

#ifndef SERIAL
    _cells = new PGraph<DPDDevice, DPDState, None, DPDMessage>(_boxesX, _boxesY);
#endif

    // Create the cells
    for(uint16_t x=0; x<D; x++) {
        for(uint16_t y=0; y<D; y++) {
            for(uint16_t z=0; z<D; z++) {
                  #if defined(SERIAL) || defined(RDF)
                    PDeviceId id = _cells.newCell();
                  #else
                    PDeviceId id = _cells->newDevice();
                  #endif
                    // Update the mapping
                    cell_t loc = {x, y, z};
                    _idToLoc[id] = loc;
                    _locToId[loc] = id;

                    // Include the cell's location in its state
                  #if defined(SERIAL) || defined(RDF)
                    DPDState *state = _cells.at(id);
                  #else
                    DPDState *state = &_cells->devices[id]->state;
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
Volume<S>::~VOLUME() {
// #ifndef SERIAL
    delete _cells;
// #endif
}

// add a bead to the simulation volume
template<class S>
cell_t Volume<S>::add_bead(const bead_t *in) {
    bead_t b = *in;
    cell_pos_t x = floor(b.pos.x()/_cell_length);
    cell_pos_t y = floor(b.pos.y()/_cell_length);
    cell_pos_t z = floor(b.pos.z()/_cell_length);
    cell_t t = {x,y,z};

    // Lookup the device
    PDeviceId b_su = _locToId[t];

    // Get the devices state
#ifdef SERIAL
    DPDState *state = _cells.at(b_su);
#else
    DPDState *state = &_cells->devices[b_su]->state;
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
        b.pos.x(b.pos.x() - (S(float(t.x))*_cell_length));
        b.pos.y(b.pos.y() - (S(float(t.y))*_cell_length));
        b.pos.z(b.pos.z() - (S(float(t.z))*_cell_length));

        // Get the next free slot in this device
        uint8_t slot = get_next_free_slot(state->bslot);
        state->bead_slot[slot] = b;
        state->bslot = set_slot(state->bslot, slot);

        _beads_added++;
    }

    return t;
}

template<class S>
void Volume<S>::add_bead_to_cell(const cell_t cell, const bead_t *in) {
    bead_t b = *in;

    if (b.pos.x() > _cell_length || b.pos.y() > _cell_length || b.pos.z() > _cell_length) {
        printf("Error: Bead position given (%f, %f, %f) is outside the bounds of this cell (%d, %d, %d) which has side length %f\n", b.pos.x(), b.pos.y(), b.pos.z(), cell.x, cell.y, cell.z, _cell_length);
        fflush(stdout);
        exit(EXIT_FAILURE);
    }

    // Lookup the device
    PDeviceId b_su = _locToId[cell];

    // Get the device state
#ifdef SERIAL
    DPDState *state = _volume.getCell(b_su);
#else
    DPDState *state = &_cells->devices[b_su]->state;
#endif

    // Get the next free slot in this device
    uint8_t slot = get_next_free_slot(state->bslot);
    state->bead_slot[slot] = b;
    state->bslot = set_slot(state->bslot, slot);
    _beads_added++;
}

#endif /* __VOLUME_IMPL */
