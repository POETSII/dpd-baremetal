// A class that contains a DPD volume

#ifndef _VOLUME_H
#define _VOLUME_H

#include <vector>
#include <iostream>

#include "dpd.hpp"
#include "Cells.hpp"

template<class S, class C>
class Volume {

    public:

    // Constructors and destructors
    Volume(S volume_length, unsigned cells_per_dimension);
    ~Volume();

    // Add a bead to the volume
    cell_t add_bead(const bead_t* in);
    // Adds a bead to the volume in given cell space. Value of all positions must be less than cell length
    void add_bead_to_cell(const bead_t* bead, const cell_t in);

    // Debugging
    // Prints the number of beads assigned to each cell
    void print_occupancy();

    // Getters
    unsigned get_cells_per_dimension();
    uint32_t get_boxes_x();
    uint32_t get_boxes_y();
    Cells<C> * get_cells();
    uint32_t get_number_of_cells();
    uint32_t get_number_of_beads();
    S get_volume_length();

    protected:

    // Length of one side of cubic volume in generic
    S volume_length;

    // Total beads this volume contains
    uint32_t beads_added = 0;

    Cells<C> *cells;

    // Number and arrangement of boxes to use
    uint32_t boxes_x, boxes_y;
};

#include "../src/Volume.cpp"

#endif /*_VOLUME_H */
