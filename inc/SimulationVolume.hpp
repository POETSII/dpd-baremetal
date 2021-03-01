// A class that contains the simulation volume.
// used to generate and manage DPD simulation volumes.

#ifndef _SIM_VOLUME_H
#define _SIM_VOLUME_H

#include "Volume.hpp"
#include "SimulationCells.hpp"

template<class S, class C>
class SimulationVolume : public Volume<S, C> {

public:

    // Constructors and destructors
    SimulationVolume(S volume_length, unsigned cells_per_dimension);
    ~SimulationVolume();

    // Simulation setup
    // Write the volume data into the respective way for simulation
    virtual void write(void *dest) = 0;

    // Volume setup
    // Checks to see if this bead can be added to the volume
    bool space_for_bead(const bead_t* in);
    // Checks to see if this pair of beads can be added to the volume
    bool space_for_bead_pair(const bead_t* a, const bead_t *b);
    // Gets single dimension neighbour based on n which is -1, 0 or 1
    uint16_t get_neighbour_cell_dimension(cell_pos_t c, int16_t n);
    // Gets device ID for neighbouring cell. d_x, d_y and d_z are between -1 and 1 and used for to find the 26 neighbours
    PDeviceId get_neighbour_cell_id(cell_t u_i, int16_t d_x, int16_t d_y, int16_t d_z);
    // Find the distance between the given bead and its nearest bead
    float find_nearest_bead_distance(const bead_t *i, cell_t u_i);
    // Store the nearest bead distances for each bead in a JSON file
    void store_initial_bead_distances();

};

#include "../src/SimulationVolume.cpp"

#endif /*_SIM_VOLUME_H */
