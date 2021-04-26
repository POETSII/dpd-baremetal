// Holds cell information and provides access to cell states.

#include "Cells.hpp"

#ifndef __CELLS_IMPL
#define __CELLS_IMPL

// Constructor
template <class C>
Cells<C>::Cells(unsigned cells_per_dimension, float cell_length) {
    this->cells_per_dimension = cells_per_dimension;
    this->cell_length = cell_length;
};

// Destructor
template <class C>
Cells<C>::~Cells() {};

// Getters and Setters

template <class C>
std::map<PDeviceId, cell_t> * Cells<C>::get_idToLoc() {
    return &idToLoc;
}

template <class C>
std::map<cell_t, PDeviceId> * Cells<C>::get_locToId() {
    return &locToId;
}

template <class C>
unsigned Cells<C>::get_cells_per_dimension() {
    return cells_per_dimension;
}

template <class C>
float Cells<C>::get_cell_length() {
    return cell_length;
}

template <class C>
cell_t Cells<C>::get_cell_loc(PDeviceId id) {
   return idToLoc[id];
}

template <class C>
PDeviceId Cells<C>::get_device_id(cell_t loc) {
    return locToId[loc];
}

#endif /* __CELLS_IMPL */
