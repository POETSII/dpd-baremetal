// Dissipative particle dynamics simulation serial x86 version.
// This is aimed at simulating DPD systems using the same code as the POLite
// version but serially on an x86 machine. This way, it will produce the same
// results, but we are not restricted by message sizes or instruction space
// and so can add traces and debugging to determine any issues with the POETS
// DPD code.

#ifndef _SERIAL_SIM_H
#define _SERIAL_SIM_H

#include "dpd.hpp"
#include "BeadMap.hpp"
#include <vector>

/********************* TYPEDEFS **************************/

typedef uint32_t PDeviceId;

/********************* CONSTANTS **************************/

const uint8_t NEIGHBOURS = 26;

/********************* STRUCTS ***************************/

// Format of message
struct DPDMessage {
    uint8_t type;
    uint32_t timestep; // the timestep this message is from
    cell_t from; // the unit that this message is from
    bead_t beads[1]; // the beads payload from this unit
}; // 50 Bytes

// the state of the DPD Device
struct DPDState {
    PDeviceId neighbours[NEIGHBOURS]; // Holds a list of the neighbours of this cell
    uint8_t num_neighbours; // Holds how many neighbours this cell currently has
    float unit_size; // the size of this spatial unit in one dimension
    uint8_t N;
    cell_t loc; // the location of this cube
    uint32_t bslot; // a bitmap of which bead slot is occupied
    uint32_t sentslot; // a bitmap of which bead slot has not been sent from yet
    bead_t bead_slot[MAX_BEADS]; // at most we have five beads per device
    Vector3D<int32_t> force_slot[MAX_BEADS]; // at most 5 beads -- force for each bead
#ifdef BETTER_VERLET
    Vector3D<ptype> old_velo[MAX_BEADS]; // Store old velocites for verlet
#endif
    uint32_t migrateslot; // a bitmask of which bead slot is being migrated in the next phase
    cell_t migrate_loc[MAX_BEADS]; // slots containing the destinations of where we want to send a bead to
    uint8_t mode; // the mode that this device is in 0 = update; 1 = migration
#ifdef VISUALISE
    uint32_t emitcnt; // a counter to kept track of updates between emitting the state
#endif
    uint32_t timestep; // the current timestep that we are on
    uint32_t grand; // the global random number at this timestep
    uint64_t rngstate; // the state of the random number generator

    uint32_t lost_beads; // Beads lost due to the cell having a full bead_slot
    uint32_t max_timestep; // Maximum timestep for this run

#ifdef SMALL_DT_EARLY
    ptype dt;
    ptype inv_sqrt_dt;
#endif
};

/********************* CLASS DEFINITION **************************/

class SerialSim {
    public:

    // constructors and destructors
    // SerialSim();
    // ~SerialSim();

    // Create a new cell
    // Return the ID of this cell
    PDeviceId newCell();
    // Get a pointer to a cell
    DPDState* getCell(PDeviceId id);
    // Add cell b as a neighbour to cell a
    void addEdge(PDeviceId a, PDeviceId b);

    private:

    std::vector<DPDState> _cells;
    uint32_t _num_cells = 0;

};

#endif // _SERIAL_SIM_H
