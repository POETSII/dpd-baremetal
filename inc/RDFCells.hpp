// Holds cell information and provides access to cell states.
// For any form of cell which will be used in a DPD simulation

#ifndef __RDF_CELLS_H
#define __RDF_CELLS_H

#include <vector>

#include "Cells.hpp"

class RDFCells : public Cells<std::vector<DPDState>> {

public:

    // Constructor
    RDFCells(unsigned cells_per_dimension, float cell_length);
    // Destructor
    ~RDFCells();

    // Getters and setters

    // Get cell state from its device ID
    DPDState * get_cell_state(PDeviceId id) override;

    // Get cell state from its location
    DPDState * get_cell_state(cell_t loc) override;

protected:

    void initialise_cells() override;

};

#endif /* __RDF_CELLS_H */
