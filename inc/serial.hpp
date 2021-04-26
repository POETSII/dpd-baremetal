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
#include <map>

#include <boost/algorithm/string.hpp>
#include <iomanip>
#include <fstream>
#include <sstream>

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
// const ptype early_dt = 0.002;
const ptype early_dt = 0.005;
// Inverse square root of dt - dt^(-1/2)
const ptype normal_inv_sqrt_dt = 7.071067812;
// const ptype early_inv_sqrt_dt = 22.360679775;
const ptype early_inv_sqrt_dt = 14.142135624;
#endif

#ifdef VISUALISE
const uint32_t emitperiod = 1;
#endif

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
    void setCellsPerDimension(uint32_t _cells_per_dimension);
    void setCellLength(ptype cell_length);
    void setQueue(moodycamel::BlockingConcurrentQueue<DPDMessage>* queue);

/************** DPD Functions ***************/
    // Initialise each cell
    void init(DPDState *s);
    // Calculate forces of neighbour cell's beads acting on this cells beads
    void neighbour_forces(DPDState *local_state, DPDState *neighbour_state);
    // Migrate a bead to its given neighbour
    void migrate_bead(const bead_t *migrating_bead, const cell_t dest, const PDeviceId neighbours[NEIGHBOURS]);
    // Host receive a message from the thread
    DPDMessage receiveMessage();

/************** Runtime functions ***************/
    // Run the simulator
    void run();

    private:

    std::vector<DPDState> _cells;
    uint32_t _num_cells = 0;
    uint32_t _timestep = 0;
    uint32_t _max_timestep = 0;
    uint32_t _cells_per_dimension = 0;
    ptype _cell_size = 0;
    #ifdef VISUALISE
    uint32_t _emitcnt = emitperiod;
    #endif
    moodycamel::BlockingConcurrentQueue<DPDMessage> *_queue;

    // Send a message from thread to host
    void sendMessage(DPDMessage *msg);

};

#endif // _SERIAL_SIM_H
