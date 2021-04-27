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
#include "SerialVolume.hpp"
#include "utils.hpp"

/********************* CLASS DEFINITION **************************/

class SerialSimulator : public Simulator<SerialVolume> {

    public:

    SerialSimulator(const ptype volume_length, const unsigned cells_per_dimension, uint32_t start_timestep, uint32_t max_timestep);
    ~SerialSimulator() {}

/************** Runtime functions ***************/
    // Simulation control
    void write() override;
    void run() override;
    void test(void *result) override;

    private:

/************** Setup functions ***************/
    void setQueue(moodycamel::BlockingConcurrentQueue<DPDMessage>* queue);

/************** DPD Functions ***************/
    // Initialise each cell
    void init(DPDState *s);
    // Calculate forces of neighbour cell's beads acting on this cells beads
    void neighbour_forces(DPDState *local_state, DPDState *neighbour_state);
    // Migrate a bead to its given neighbour
    void migrate_bead(const bead_t *migrating_bead, const cell_t dest, const std::vector<PDeviceId> neighbours);

    // Host receive a message from the thread
    DPDMessage receiveMessage();

    // Send a message from thread to host
    void sendMessage(DPDMessage *msg);

    moodycamel::BlockingConcurrentQueue<DPDMessage> *_queue;

};

// #include "../src/SerialSimulator.cpp"

#endif // _SERIAL_SIM_H
