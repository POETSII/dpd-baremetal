// Holds cell information and provides access to cell states.

#ifndef __CELLS_H
#define __CELLS_H

#ifdef STATS
#define POLITE_DUMP_STATS
#define POLITE_COUNT_MSGS
#endif

#include <map>

#ifndef XML
#include "dpd.hpp"
#else
#include "DPDStructs.hpp"
#endif

// // Include the correct POLite device code.
// #ifndef XML
// #ifdef GALS
// #include "gals.h"
// #elif defined(SERIAL)
// #include "SerialSimulator.hpp"
// #elif defined(RDF)
// #else
// #include "sync.h"
// #endif
// #endif // XML

#if !defined(SERIAL) && !defined(RDF) && !defined(XML)
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

    cell_t get_cell_loc(PDeviceId id);
    PDeviceId get_device_id(cell_t loc);

    // Access to cell data
    virtual uint8_t get_cell_bslot(cell_t loc) = 0;
    virtual uint8_t get_device_bslot(PDeviceId id) = 0;
    virtual const bead_t * get_bead_from_cell_slot(cell_t loc, uint8_t slot) = 0;
    virtual const bead_t * get_bead_from_device_slot(PDeviceId id, uint8_t slot) = 0;

    // Adding a bead to the cell
    virtual void place_bead_in_cell(bead_t *b, cell_t loc) = 0;
    virtual void place_bead_in_device(bead_t *b, PDeviceId id) = 0;

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
