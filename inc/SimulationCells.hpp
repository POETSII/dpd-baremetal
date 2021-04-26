// Holds cell information and provides access to cell states.
// For any form of cell which will be used in a DPD simulation

#ifndef __SIMULATIONCELLS_H
#define __SIMULATIONCELLS_H

#include "Cells.hpp"

template<class C>
class SimulationCells : public Cells<C> {

public:

    // Constructor
    SimulationCells(unsigned cells_per_dimension, float cell_length) : Cells<C>(cells_per_dimension, cell_length) {};
    // Destructor
    ~SimulationCells() {};

    // Simulation setup
    // Write the cells data into the respective way for simulation
    virtual void write(void *dest) = 0;

    // Getters and setter
    virtual void set_start_timestep(uint32_t start_timestep) = 0;
    virtual void set_end_timestep(uint32_t end_timestep) = 0;

protected:

    // Make these two devices neighbours
    virtual void addNeighbour(PDeviceId a, PDeviceId b) {};

    // Connect edges
    void connect_cells(unsigned cells_per_dimension) {
        // A toroidal space (cube with periodic boundaries)
        for(uint16_t x = 0; x < cells_per_dimension; x++) {
            for(uint16_t y = 0; y < cells_per_dimension; y++) {
                for(uint16_t z = 0; z < cells_per_dimension; z++) {
                    // This device
                    cell_t c_loc = {x,y,z};

                    PDeviceId cId = this->locToId[c_loc];

                    // Calculate the neighbour positions
                    // (taking into account the periodic boundary).
                    int x_neg, y_neg, z_neg;
                    int x_pos, y_pos, z_pos;

                    // assign the x offsets
                    if (x == 0) {
                        x_neg = cells_per_dimension - 1;
                        x_pos = x + 1;
                    } else if (x == (cells_per_dimension - 1)) {
                        x_neg = x - 1;
                        x_pos = 0;
                    } else {
                        x_neg = x - 1;
                        x_pos = x + 1;
                    }

                    // assign the y offsets
                    if(y == 0) {
                        y_neg = cells_per_dimension - 1;
                        y_pos = y + 1;
                    } else if (y == (cells_per_dimension - 1)) {
                        y_neg = y - 1;
                        y_pos = 0;
                    } else {
                        y_neg = y - 1;
                        y_pos = y + 1;
                    }

                    // assign the z offsets
                    if(z == 0) {
                        z_neg = cells_per_dimension - 1;
                        z_pos = z + 1;
                    } else if (z == (cells_per_dimension - 1)) {
                        z_neg = z - 1;
                        z_pos = 0;
                    } else {
                        z_neg = z - 1;
                        z_pos = z + 1;
                    }

                    // Neighbour location and ID
                    cell_t n_loc;
                    PDeviceId nId;

                    // If we use send to self, a cell needs to be connected to itself
                #ifdef SEND_TO_SELF
                    addNeighbour(cId, cId);
                #elif defined(GALS) && !defined(ONE_BY_ONE)
                    // Non-improved GALS uses send to self by default
                    addNeighbour(cId, cId);
                #endif

                    // z = -1
                    // { -1,-1,-1 },  { -1,0,-1 },  { -1, +1,-1 }
                    n_loc.x = x_neg; n_loc.y = y_neg; n_loc.z = z_neg;
                    nId = this->locToId[n_loc];
                    addNeighbour(cId, nId);

                    n_loc.x = x_neg; n_loc.y = y; n_loc.z = z_neg;
                    nId = this->locToId[n_loc];
                    addNeighbour(cId, nId);

                    n_loc.x = x_neg; n_loc.y = y_pos; n_loc.z = z_neg;
                    nId = this->locToId[n_loc];
                    addNeighbour(cId, nId);

                    // { 0,-1, -1 },  { 0, 0,-1 },  { 0, +1, -1 }
                    n_loc.x = x; n_loc.y = y_neg; n_loc.z = z_neg;
                    nId = this->locToId[n_loc];
                    addNeighbour(cId, nId);

                    n_loc.x = x; n_loc.y = y; n_loc.z = z_neg;
                    nId = this->locToId[n_loc];
                    addNeighbour(cId, nId);

                    n_loc.x = x; n_loc.y = y_pos; n_loc.z = z_neg;
                    nId = this->locToId[n_loc];
                    addNeighbour(cId, nId);

                    // { +1,-1,-1 },  { +1,0,-1 },  { +1, +1,-1 }
                    n_loc.x = x_pos; n_loc.y = y_neg; n_loc.z = z_neg;
                    nId = this->locToId[n_loc];
                    addNeighbour(cId, nId);

                    n_loc.x = x_pos; n_loc.y = y; n_loc.z = z_neg;
                    nId = this->locToId[n_loc];
                    addNeighbour(cId, nId);

                    n_loc.x = x_pos; n_loc.y = y_pos; n_loc.z = z_neg;
                    nId = this->locToId[n_loc];
                    addNeighbour(cId, nId);

                    // z = 0
                    // { -1,-1,0 },  { -1,0,0 },  { -1, +1,0 }
                    n_loc.x = x_neg; n_loc.y = y_neg; n_loc.z = z;
                    nId = this->locToId[n_loc];
                    addNeighbour(cId, nId);

                    n_loc.x = x_neg; n_loc.y = y; n_loc.z = z;
                    nId = this->locToId[n_loc];
                    addNeighbour(cId, nId);

                    n_loc.x = x_neg; n_loc.y = y_pos; n_loc.z = z;
                    nId = this->locToId[n_loc];
                    addNeighbour(cId, nId);

                    // { 0,-1, 0 },  { 0, 0, 0 },  { 0, +1, 0 }
                    n_loc.x = x; n_loc.y = y_neg; n_loc.z = z;
                    nId = this->locToId[n_loc];
                    addNeighbour(cId, nId);

                    // skipping! one is not a neighbour of oneself
                    //n_loc.x = x; n_loc.y = y; n_loc.z = z;

                    n_loc.x = x; n_loc.y = y_pos; n_loc.z = z;
                    nId = this->locToId[n_loc];
                    addNeighbour(cId, nId);

                    // { +1,-1, 0 },  { +1,0, 0 },  { +1, +1, 0 }
                    n_loc.x = x_pos; n_loc.y = y_neg; n_loc.z = z;
                    nId = this->locToId[n_loc];
                    addNeighbour(cId, nId);

                    n_loc.x = x_pos; n_loc.y = y; n_loc.z = z;
                    nId = this->locToId[n_loc];
                    addNeighbour(cId, nId);

                    n_loc.x = x_pos; n_loc.y = y_pos; n_loc.z = z;
                    nId = this->locToId[n_loc];
                    addNeighbour(cId, nId);

                    // z = +1
                    // { -1,-1,+1 },  { -1,0,+1},  { -1, +1,+1 }
                    n_loc.x = x_neg; n_loc.y = y_neg; n_loc.z = z_pos;
                    nId = this->locToId[n_loc];
                    addNeighbour(cId, nId);

                    n_loc.x = x_neg; n_loc.y = y; n_loc.z = z_pos;
                    nId = this->locToId[n_loc];
                    addNeighbour(cId, nId);

                    n_loc.x = x_neg; n_loc.y = y_pos; n_loc.z = z_pos;
                    nId = this->locToId[n_loc];
                    addNeighbour(cId, nId);

                    // { 0,-1, +1 },  { 0, 0, +1 },  { 0, +1, +1 }
                    n_loc.x = x; n_loc.y = y_neg; n_loc.z = z_pos;
                    nId = this->locToId[n_loc];
                    addNeighbour(cId, nId);

                    n_loc.x = x; n_loc.y = y; n_loc.z = z_pos;
                    nId = this->locToId[n_loc];
                    addNeighbour(cId, nId);

                    n_loc.x = x; n_loc.y = y_pos; n_loc.z = z_pos;
                    nId = this->locToId[n_loc];
                    addNeighbour(cId, nId);

                    // { +1,-1, +1 },  { +1,0, +1 },  { +1, +1, +1 }
                    n_loc.x = x_pos; n_loc.y = y_neg; n_loc.z = z_pos;
                    nId = this->locToId[n_loc];
                    addNeighbour(cId, nId);

                    n_loc.x = x_pos; n_loc.y = y; n_loc.z = z_pos;
                    nId = this->locToId[n_loc];
                    addNeighbour(cId, nId);

                    n_loc.x = x_pos; n_loc.y = y_pos; n_loc.z = z_pos;
                    nId = this->locToId[n_loc];
                    addNeighbour(cId, nId);
                }
            }
        }
        // All the edges have been connected
    }

};

#endif /* __SIMULATIONCELLS_H */
