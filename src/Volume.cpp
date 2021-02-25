
// Implementation file for the host simulation volume class

#include "Volume.hpp"

#ifndef __VOLUME_IMPL
#define __VOLUME_IMPL

// Constructor
template<class S, class C>
Volume<S, C>::Volume(S volume_length, unsigned cells_per_dimension) {
    this->volume_length = volume_length;
    this->cells_per_dimension = cells_per_dimension;
    this->cell_length = this->volume_length / S(this->cells_per_dimension);

    this->boxes_x = 1;
    this->boxes_y = 1;

#if !defined(SERIAL) && !defined(RDF)
    cells = new PGraph<DPDDevice, DPDState, None, DPDMessage>(this->boxes_x, this->boxes_y);
#endif

    // Create the cells
    for(uint16_t x = 0; x < cells_per_dimension; x++) {
        for(uint16_t y = 0; y < cells_per_dimension; y++) {
            for(uint16_t z = 0; z < cells_per_dimension; z++) {
                  #if defined(SERIAL) || defined(RDF)
                    DPDState new_state;
                    PDeviceId id = cells.size();
                    cells.push_back(new_state);
                  #else
                    PDeviceId id = cells->newDevice();
                  #endif
                    // Update the mapping
                    cell_t loc = {x, y, z};
                    idToLoc[id] = loc;
                    locToId[loc] = id;
            }
        }
    }

}

// Deconstructor
template<class S, class C>
Volume<S, C>::~Volume() {

}

// Print out the occupancy of each device
template<class S, class C>
void Volume<S, C>::print_occupancy() {
    // Loop through all devices in the volume and print their number of particles assigned
    printf("DeviceId\t\tbeads\n--------------\n");
    for(auto const& x : idToLoc) {
        PDeviceId t = x.first;
      #if defined(SERIAL) || defined(RDF)
        uint8_t beads = get_num_beads(cells.at(t).bslot);
      #else
        uint8_t beads = get_num_beads(cells.devices[t]->state.bslot);
      #endif
        if(beads > 0)
            printf("%x\t\t\t%d\n", t, (uint32_t)beads);
    }
}

// add a bead to the simulation volume
template<class S, class C>
cell_t Volume<S, C>::add_bead(const bead_t *in) {
    bead_t b = *in;
    cell_pos_t x = floor(b.pos.x()/cell_length);
    cell_pos_t y = floor(b.pos.y()/cell_length);
    cell_pos_t z = floor(b.pos.z()/cell_length);
    cell_t t = {x,y,z};

    // Lookup the device
    PDeviceId b_su = locToId[t];

    // Get the devices state
#if defined(SERIAL) || defined(RDF)
    DPDState *state = &cells.at(b_su);
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

template<class S, class C>
void Volume<S, C>::add_bead_to_cell(const bead_t *in, const cell_t cell) {
    bead_t b = *in;

    if (b.pos.x() > cell_length || b.pos.y() > cell_length || b.pos.z() > cell_length) {
        printf("Error: Bead position given (%f, %f, %f) is outside the bounds of this cell (%d, %d, %d) which has side length %f\n", b.pos.x(), b.pos.y(), b.pos.z(), cell.x, cell.y, cell.z, cell_length);
        fflush(stdout);
        exit(EXIT_FAILURE);
    }

    // Lookup the device
    PDeviceId b_su = locToId[cell];

    // Get the device state
#if defined(SERIAL) || defined(RDF)
    DPDState *state = cells.at(b_su);
#else
    DPDState *state = &cells.devices[b_su]->state;
#endif

    // Get the next free slot in this device
    uint8_t slot = get_next_free_slot(state->bslot);
    state->bead_slot[slot] = b;
    state->bslot = set_slot(state->bslot, slot);
    beads_added++;
}

template<class S, class C>
unsigned Volume<S, C>::get_cells_per_dimension() {
    return this->cells_per_dimension;
}

template<class S, class C>
DPDState * Volume<S, C>::get_state_of_cell(cell_t loc) {
  #if defined(SERIAL) || defined(RDF)
    PDeviceId id = locToId[loc];
    return &cells.at(id);
  #else
    PDeviceId id = this->locToId[loc];
    return &cells->devices[id]->state;
  #endif
}

template<class S, class C>
uint32_t Volume<S, C>::get_boxes_x() {
    return this->boxes_x;
}

template<class S, class C>
uint32_t Volume<S, C>::get_boxes_y() {
    return this->boxes_y;
}

template<class S, class C>
C * Volume<S, C>::get_cells() {
    return &cells;
}

template<class S, class C>
uint32_t Volume<S, C>::get_number_of_cells() {
    return (cells_per_dimension * cells_per_dimension * cells_per_dimension);
}

template<class S, class C>
uint32_t Volume<S, C>::get_number_of_beads() {
    return beads_added;
}

template<class S, class C>
S Volume<S, C>::get_volume_length() {
    return volume_length;
}

#endif /* __VOLUME_IMPL */
