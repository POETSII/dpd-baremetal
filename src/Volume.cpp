
// Implementation file for the host simulation volume class

#include "Volume.hpp"

#ifndef __VOLUME_IMPL
#define __VOLUME_IMPL

// Constructor
template<class C>
Volume<C>::Volume(const float volume_length, const unsigned cells_per_dimension) {
    this->volume_length = volume_length;
}

// Deconstructor
template<class C>
Volume<C>::~Volume() {

}

// Print out the occupancy of each device
template<class C>
void Volume<C>::print_occupancy() {
    // Loop through all devices in the volume and print their number of particles assigned
    printf("DeviceId\t\tbeads\n--------------\n");
    for(auto const& x : cells->get_idToLoc()) {
        PDeviceId t = x.first;
      #if defined(SERIAL) || defined(RDF)
        uint8_t beads = get_num_beads(cells->at(t).bslot);
      #else
        uint8_t beads = get_num_beads(cells->devices[t]->state.bslot);
      #endif
        if(beads > 0)
            printf("%x\t\t\t%d\n", t, (uint32_t)beads);
    }
}

// add a bead to the simulation volume
template<class C>
cell_t Volume<C>::add_bead(const bead_t *in) {
    float cell_length = cells->get_cell_length();

    bead_t b = *in;
    cell_pos_t x = floor(b.pos.x()/cell_length);
    cell_pos_t y = floor(b.pos.y()/cell_length);
    cell_pos_t z = floor(b.pos.z()/cell_length);
    cell_t t = {x,y,z};

    // Get the devices bslot
    uint8_t bslot = cells->get_cell_bslot(t);

    // Check to make sure there is still enough room in the device
    if (get_num_beads(bslot) > MAX_BEADS) {
        std::cerr << "Error: there is not enough space in cell: " << t.x << ", " << t.y << ", " << t.z << " for bead: " << in->id << ".\n";
        std::cerr << "There is already " << get_num_beads(bslot) << " beads in this cell for a max of\n";
        fflush(stdout);
        exit(EXIT_FAILURE);
    } else {
        // We can add the bead

        // Make the position of the bead relative to the cell (0.0 - < 1.0)
        b.pos.x(b.pos.x() - (float(t.x) * cell_length));
        b.pos.y(b.pos.y() - (float(t.y) * cell_length));
        b.pos.z(b.pos.z() - (float(t.z) * cell_length));

        // Get the next free slot in this device
        uint8_t slot = get_next_free_slot(bslot);

        cells->place_bead_in_cell_slot(&b, t, slot);

        beads_added++;
    }

    return t;
}

template<class C>
void Volume<C>::add_bead_to_cell(bead_t *in, const cell_t cell) {
    float cell_length = cells->get_cell_length();

    if (in->pos.x() > cell_length || in->pos.y() > cell_length || in->pos.z() > cell_length) {
        printf("Error: Bead position given (%f, %f, %f) is outside the bounds of this cell (%d, %d, %d) which has side length %f\n", in->pos.x(), in->pos.y(), in->pos.z(), cell.x, cell.y, cell.z, cells->get_cell_length());
        fflush(stdout);
        exit(EXIT_FAILURE);
    }

    // Get the devices bslot
    uint8_t bslot = cells->get_cell_bslot(cell);

    // Get the next free slot in this device
    uint8_t slot = get_next_free_slot(bslot);

    cells->place_bead_in_cell_slot(in, cell, slot);

    beads_added++;
}

template<class C>
unsigned Volume<C>::get_cells_per_dimension() {
    return cells->get_cells_per_dimension();
}

template<class C>
uint32_t Volume<C>::get_boxes_x() {
    return this->boxes_x;
}

template<class C>
uint32_t Volume<C>::get_boxes_y() {
    return this->boxes_y;
}

template<class C>
Cells<C> * Volume<C>::get_cells() {
    return cells;
}

template<class C>
uint32_t Volume<C>::get_number_of_cells() {
    return (cells->get_cells_per_dimension() * cells->get_cells_per_dimension() * cells->get_cells_per_dimension());
}

template<class C>
uint32_t Volume<C>::get_number_of_beads() {
    return beads_added;
}

template<class C>
float Volume<C>::get_volume_length() {
    return volume_length;
}

#endif /* __VOLUME_IMPL */
