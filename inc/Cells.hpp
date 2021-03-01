// Holds cell information and provides access to cell states.

#ifndef __CELLS_H
#define __CELLS_H

#include <map>

#include "dpd.hpp"

// Include the correct POLite device code.
#ifdef GALS
#include "gals.h"
#elif defined(SERIAL)
#include "serial.hpp"
#elif defined(RDF)
#else
#include "sync.h"
#endif

#if !defined(SERIAL) && !defined(RDF)
#include "POLite.h"
#else
typedef uint32_t PDeviceId;
#endif

template<class C>
class Cells {

public:

    // Constructor
    Cells(unsigned cells_per_dimension, float cell_length);
    // Destructor
    ~Cells();

    // Getters and Setters

    std::map<PDeviceId, cell_t> * get_idToLoc();

    std::map<cell_t, PDeviceId> * get_locToId();

    unsigned get_cells_per_dimension();

    ptype get_cell_length();

    virtual DPDState * get_cell_state(PDeviceId id) = 0;
    virtual DPDState * get_cell_state(cell_t loc) = 0;

    cell_t get_cell_loc(PDeviceId id);
    PDeviceId get_device_id(cell_t loc);

protected:

    // The cells themselves
    C cells;

    // Length of one side of cubic volume in number of cells
    unsigned cells_per_dimension;
    // Length in each dimension of one cell
    ptype cell_length;

    // Maintain maps of ID's to locations (and vice versa) in the volume
    std::map<PDeviceId, cell_t> idToLoc;
    std::map<cell_t, PDeviceId> locToId;

    virtual void initialise_cells() = 0;

};

#include "../src/Cells.cpp"

#endif /* __CELLS_H */
