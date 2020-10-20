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
#include <iostream>
#include "blockingconcurrentqueue.h"
#include <thread>

/********************* TYPEDEFS **************************/

typedef uint32_t PDeviceId;

/********************* CONSTANTS **************************/

// Number of neighbours
// If we change this to implement more of POLites operation it will come in
// handy to allow this to be defined as different values.
const uint8_t NEIGHBOURS = 26;

// Timestep and inverse sqrt of timestep
#ifndef SMALL_DT_EARLY
const ptype dt = 0.02;
// Inverse square root of dt - dt^(-1/2)
const ptype inv_sqrt_dt = 7.071067812;
#else
const ptype normal_dt = 0.02;
const ptype early_dt = 0.002;
// Inverse square root of dt - dt^(-1/2)
const ptype normal_inv_sqrt_dt = 7.071067812;
const ptype early_inv_sqrt_dt = 22.360679775;
#endif

#ifdef VISUALISE
const uint32_t emitperiod = 1;
#endif

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
    uint8_t num_neighbours = 0; // Holds how many neighbours this cell currently has
    cell_t loc; // the location of this cube
    uint32_t bslot = 0; // a bitmap of which bead slot is occupied
    uint32_t sentslot = 0; // a bitmap of which bead slot has not been sent from yet
    bead_t bead_slot[MAX_BEADS]; // at most we have five beads per device
    Vector3D<int32_t> force_slot[MAX_BEADS]; // at most 5 beads -- force for each bead
#ifdef BETTER_VERLET
    Vector3D<ptype> old_velo[MAX_BEADS]; // Store old velocites for verlet
#endif
    uint32_t migrateslot = 0; // a bitmask of which bead slot is being migrated in the next phase
    cell_t migrate_loc[MAX_BEADS]; // slots containing the destinations of where we want to send a bead to
    uint8_t mode = 0; // the mode that this device is in 0 = update; 1 = migration

    uint32_t grand = 0; // the global random number at this timestep
    uint64_t rngstate = 0; // the state of the random number generator

    uint32_t lost_beads = 0; // Beads lost due to the cell having a full bead_slot

#ifdef SMALL_DT_EARLY
    ptype dt = 0.0;
    ptype inv_sqrt_dt = 0.0;
#endif
};

/********************* CLASS DEFINITION **************************/

class SerialSim {

    public:

    // constructors and destructors
    // SerialSim();
    // ~SerialSim();

/************** Setup functions ***************/
    // Create a new cell
    // Return the ID of this cell
    PDeviceId newCell();
    // Get a pointer to a cell
    DPDState* getCell(PDeviceId id);
    // Add cell b as a neighbour to cell a
    void addEdge(PDeviceId a, PDeviceId b);
    // Set start timestep
    void setTimestep(uint32_t timestep);
    // Set max timestep
    void setMaxTimestep(uint32_t maxTimestep);
    // Set cell size
    void setN(uint32_t N);
    void setCellSize(ptype cell_size);
    void setQueue(moodycamel::BlockingConcurrentQueue<DPDMessage>* queue);

/************** DPD Functions ***************/
    uint32_t p_rand(DPDState *s);
    // Initialise each cell
    void init(DPDState *s);
    // Calculate forces of neighbour cell's beads acting on this cells beads
    void neighbour_forces(DPDState *local_state, DPDState *neighbour_state);
    // Migrate a bead to its given neighbour
    void migrate_bead(const bead_t migrating_bead, const cell_t dest, const PDeviceId neighbours[NEIGHBOURS]);
    // Get a message from the thread
    DPDMessage getMessage();

/************** Runtime functions ***************/
    // Run the simulator
    void run();

    private:

    std::vector<DPDState> _cells;
    uint32_t _num_cells = 0;
    uint32_t _timestep = 0;
    uint32_t _max_timestep = 0;
    uint32_t _N = 0;
    ptype _cell_size = 0;
    #ifdef VISUALISE
    uint32_t _emitcnt = emitperiod;
    #endif
    moodycamel::BlockingConcurrentQueue<DPDMessage> *_queue;

};

#endif // _SERIAL_SIM_H
