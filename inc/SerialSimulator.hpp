// Dissipative particle dynamics simulation serial x86 version.
// This is aimed at simulating DPD systems using the same code as the POLite
// version but serially on an x86 machine. This way, it will produce similar
// results, but we are not restricted by message sizes or instruction space
// and so can add traces and debugging to determine any issues with the POETS
// DPD code.

#ifndef _SERIAL_SIM_H
#define _SERIAL_SIM_H

#include <vector>
#include <iostream>
#include "blockingconcurrentqueue.h"
#include <thread>
#include <map>

#include "dpd.hpp"
#include "BeadMap.hpp"
#include "Simulator.hpp"
#include "SimulationVolume.hpp"

/********************* CLASS DEFINITION **************************/

class SerialSimulator : public Simulator<SimulationVolume<std::vector<DPDState>>> {

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
