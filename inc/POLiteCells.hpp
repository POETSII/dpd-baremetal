// Holds cell information and provides access to cell states.
// Provides an interface for PGraph

#ifndef __POLITECELLS_H
#define __POLITECELLS_H

#include "SimulationCells.hpp"
#ifdef GALS
 #include "gals.h"
#else
 #include "sync.h"
#endif

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

    // Access to cell data
    uint8_t get_cell_bslot(cell_t loc) override;
    uint8_t get_device_bslot(PDeviceId loc) override;
    const bead_t * get_bead_from_cell_slot(cell_t loc, uint8_t slot) override;
    const bead_t * get_bead_from_device_slot(PDeviceId id, uint8_t slot) override;

    // Adding a bead to the cell
    void place_bead_in_cell(bead_t *b, cell_t loc) override;
    void place_bead_in_device(bead_t *b, PDeviceId id) override;

protected:

    // Make these two devices neighbours
    void addNeighbour(PDeviceId a, PDeviceId b) override;

    // Set up the each cells state
    void initialise_cells() override;

};

#endif /* __POLITECELLS_H */
