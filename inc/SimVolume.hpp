// A class that contains the simulation volume.
// used to generate and manage DPD simulation volumes.

#ifndef _SIM_VOLUME_H
#define _SIM_VOLUME_H

#include "Volume.hpp"

#include <boost/algorithm/string.hpp>

template<class S> // S is the type for this simulation i.e. fixap<C,F> or float
class SimVolume : public Volume<S> {
    public:

    // Constructors and destructors
    SimVolume(S volume_length, unsigned cells_per_dimension);
    ~SimVolume();

    // Setup
    void init_cells();
    // Checks to see if this bead can be added to the volume
    bool space_for_bead(const bead_t* in);
    // Checks to see if this pair of beads can be added to the volume
    bool space_for_bead_pair(const bead_t* a, const bead_t *b);

    // Setup
    // Make these two devices neighbours
    void addNeighbour(PDeviceId a, PDeviceId b);
    // Gets single dimension neighbour based on n which is -1, 0 or 1
    uint16_t get_neighbour_cell_dimension(cell_pos_t c, int16_t n);
    // Gets device ID for neighbouring cell. d_x, d_y and d_z are between -1 and 1 and used for to find the 26 neighbours
    PDeviceId get_neighbour_cell_id(cell_t u_i, int16_t d_x, int16_t d_y, int16_t d_z);
    // Find the distance between the given bead and its nearest bead
    float find_nearest_bead_distance(const bead_t *i, cell_t u_i);
    // Store the nearest bead distances for each bead in a JSON file
    void store_initial_bead_distances();

};

#include "../src/SimVolume.cpp"

#endif /*_SIM_VOLUME_H */
