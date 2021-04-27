// Holds cell information and provides access to cell states.
// This is specific to a Serial simulation which holds states in a vector

#ifndef __SERIALCELLS_H
#define __SERIALCELLS_H

#include "SimulationCells.hpp"

class SerialCells : public SimulationCells<std::vector<DPDState> *> {

public:

    // Constructor
    SerialCells(unsigned cells_per_dimension, float cell_length);
    // Destructor
    ~SerialCells();

    // Simulation setup
    // Dummy - Writing to the simulator isn't necessary
    void write(void *dest) override;

    // Getters and setters
    // Set the default for some of the DPD state
    void set_start_timestep(uint32_t start_timestep) override;
    void set_end_timestep(uint32_t end_timestep) override;

    // Access to cell data
    DPDState *get_cell_state(PDeviceId id);
    uint32_t get_timestep();
    void increment_timestep();

    uint8_t get_cell_bslot(cell_t loc) override;
    uint8_t get_device_bslot(PDeviceId loc) override;
    const bead_t * get_bead_from_cell_slot(cell_t loc, uint8_t slot) override;
    const bead_t * get_bead_from_device_slot(PDeviceId id, uint8_t slot) override;

    // Adding a bead to the cell
    void place_bead_in_cell(bead_t *b, cell_t loc) override;
    void place_bead_in_device(bead_t *b, PDeviceId id) override;

    // Condition functions
    bool reached_max_timestep();

protected:

    // Add a new device
    PDeviceId newDevice();

    // Make these two devices neighbours
    void addNeighbour(PDeviceId a, PDeviceId b) override;

    // Set up the each cells state
    void initialise_cells() override;

    // Number of cells inserted (used for PDeviceID addressing cells)
    uint32_t number_of_cells = 0;
    // Variables that can be global in a serial simulator
    uint32_t timestep = 0;
    uint32_t max_timestep = 10000;
    uint8_t error = 0; // If this is not 0 it ends early. Can be used to identify which error is wrong
    #ifdef VISUALISE
    uint32_t emitcnt = 0;
    #endif

};

#include "../src/SerialCells.cpp"

#endif /* __SERIALCELLS_H */
