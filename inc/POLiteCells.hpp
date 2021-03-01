// Holds cell information and provides access to cell states.
// Provides an interface for PGraph

#ifndef __POLITECELLS_H
#define __POLITECELLS_H

#include "SimulationCells.hpp"

class POLiteCells : public SimulationCells<PGraph<DPDDevice, DPDState, None, DPDMessage> *> {

public:

    // Constructor
    POLiteCells(unsigned cells_per_dimension, ptype cell_length, uint32_t boxes_x, uint32_t boxes_y);

    // Destructor
    ~POLiteCells();

    // Simulation setup
    // Write the cell data to the POETS hardware
    void write(void *dest) override;

    // Getters and setters
    // Set the default for some of the DPD state
    void set_start_timestep(uint32_t start_timestep) override;

    void set_end_timestep(uint32_t end_timestep) override;

    DPDState * get_cell_state(PDeviceId id) override;

    DPDState * get_cell_state(cell_t loc) override;

protected:

    // Make these two devices neighbours
    void addNeighbour(PDeviceId a, PDeviceId b) override;

    // Set up the each cells state
    void initialise_cells() override;

};

#endif /* __POLITECELLS_H */
