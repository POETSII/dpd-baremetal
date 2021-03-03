// Holds cell information and provides access to cell states.
// Provides an interface for PGraph

#ifndef __XML_CELLS_H
#define __XML_CELLS_H

#include <vector>

#include "SimulationCells.hpp"

class XMLCells : public SimulationCells<std::vector<DPDState>> {

public:

    // Constructor
    XMLCells(unsigned cells_per_dimension, ptype cell_length);

    // Destructor
    ~XMLCells();

    // Simulation setup
    // Write the cell data to the POETS hardware
    void write(void *dest) override;

    // Getters and setters
    // Set the default for some of the DPD state
    void set_start_timestep(uint32_t start_timestep) override;

    void set_end_timestep(uint32_t end_timestep) override;

    DPDState * get_cell_state(cell_t loc);

    DPDProperties * get_cell_properties(cell_t loc);

    GraphProperties * get_graph_properties();

    std::vector<PDeviceId> * get_cell_neighbours(cell_t loc);

    // Access to cell data
    uint8_t get_cell_bslot(cell_t loc) override;
    uint8_t get_device_bslot(PDeviceId id) override;
    const bead_t * get_bead_from_cell_slot(cell_t loc, uint8_t slot) override;
    const bead_t * get_bead_from_device_slot(PDeviceId id, uint8_t slot) override;

    // Adding a bead to the cell
    void place_bead_in_cell_slot(bead_t *b, cell_t loc, uint8_t slot) override;
    void place_bead_in_device_slot(bead_t *b, PDeviceId id, uint8_t slot) override;

protected:

    GraphProperties graph_properties;
    std::vector<DPDProperties> cell_properties;

    std::map<PDeviceId, std::vector<PDeviceId>> neighbours;

    // Make these two devices neighbours
    void addNeighbour(PDeviceId a, PDeviceId b) override;

    // Set up the each cells state
    void initialise_cells() override;

};

#endif /* __XML_CELLS_H */
