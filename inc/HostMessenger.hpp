// This class is designed to handle messages from the simulation engine at
// runtime.

#ifndef _HOST_MESSENGER_H
#define _HOST_MESSENGER_H

#include <map>
#include <iostream>
#include <sys/time.h>

#include "DPDStructs.hpp"

template<class Q>
class HostMessenger {

public:
    // Constructor
    HostMessenger(Q *queue, std::string state_dir, uint32_t cells_per_dimension, uint32_t max_timestep);

    // Setters
    virtual void set_number_of_beads(uint32_t number_of_beads) = 0;

    // Functions for different functions of message
    void check_error(DPDMessage msg); // Check if the message indactes an error

#ifdef TIMER
    unsigned timer_message(DPDMessage msg); // Message expected to end a performance test
#endif
#ifdef STATS
    unsigned stats_collection(DPDMessage msg); // Expected when a simulation has finished gathering POLite stats
#endif
#ifdef VISUALISE
    void emit_message(DPDMessage msg); // A message containing bead information to be stored for visualisation/analysis
#endif

    virtual DPDMessage await_message() = 0;

    // Run the messenger (loops continuously until the simulation end is detected)
    void run();

protected:

    // Where the messages are read from
    Q *queue;
    // The directory where simulation states are stored
    std::string state_dir;

    // Used for determining end conditions
    uint32_t devices_received_from = 0;
    // Current timestep
    uint32_t timestep = 0;

    // Fields storing info based on the simulation
    uint32_t cells_per_dimension;
    uint32_t number_of_cells;
    uint32_t number_of_beads;
    uint32_t max_timestep;

    // Fields for different messaging types

    // Timer storage
    struct timeval start, finish, elapsedTime;

#ifdef VISUALISE
    // The number of beads printed to a file per timestep
    std::map<uint32_t, uint32_t> bead_print_map;
    // If this is the first bead in the file
    bool first = true;
#endif

};

#include "../src/HostMessenger.cpp"

#endif // _HOST_MESSENGER_H
