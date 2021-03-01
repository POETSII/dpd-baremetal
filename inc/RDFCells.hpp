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
    bool get_cell_done(cell_t loc);
    void set_cell_done(cell_t loc);

    // Manipulating cell state
    uint8_t get_cell_bslot(cell_t loc) override;
    const bead_t * get_bead_from_cell_slot(cell_t loc, uint8_t slot) override;

    // Adding a bead
    void place_bead_in_cell_slot(bead_t *b, cell_t loc, uint8_t slot) override;

protected:

    void initialise_cells() override;

};

#endif /* __RDF_CELLS_H */
