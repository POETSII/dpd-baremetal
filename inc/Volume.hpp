// A class that contains a DPD volume

#ifndef _VOLUME_H
#define _VOLUME_H

#ifdef STATS
#define POLITE_DUMP_STATS
#define POLITE_COUNT_MSGS
#endif

#ifdef GALS
#include "gals.h"
#elif defined(SERIAL)
#include "serial.hpp"
#elif defined(RDF)
#include "RDF.hpp"
#else
#include "sync.h"
#endif
#ifndef SERIAL
#include "POLite.h"
#endif

#include <vector>
#include <map>

template<class S> // S is the type for this simulation i.e. fixap<C,F> or float
class Volume {
    public:

    // Constructors and destructors
    Volume(S volume_length, unsigned cells_per_dimension);
    ~Volume();

    // Setup
    // Add a bead to the volume
    cell_t add_bead(const bead_t* in);
    // Adds a bead to the volume in given cell space. Value of all positions must be less than cell length
    void add_bead_to_cell(const bead_t* in, const cell_t bead);

    // Debugging
    // Prints the number of beads assigned to each cell
    void print_occupancy();

    protected:

    // Length of one side of cubic volume in generic
    S volume_length;
    // Length of one side of cubic volume in number of cells
    unsigned cells_per_dimension;
    // Length of one side of cubic cell in generic
	S cell_length;

    // Total beads this volume contains
    uint32_t beads_added = 0;

    // The cells
  #if defined(SERIAL) || defined(RDF)
    // A vector where PDeviceId represents its index in the vector
    std::vector<DPDState> *cells;
  #else
	// POLite graph containing the cells
    // PDeviceId represents its device ID in the graph
	PGraph<DPDDevice, DPDState, None, DPDMessage> *cells;
  #endif

    // Maintain maps of ID's to locations (and vice versa) in the volume
    std::map<PDeviceId, cell_t> idToLoc;
    std::map<cell_t, PDeviceId> locToId;
};

#include "../src/Volume.cpp"

#endif /*_VOLUME_H */
