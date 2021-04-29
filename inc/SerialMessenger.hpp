// This handles messages from the serial simulator

#ifndef _SERIAL_MESSENGER_H
#define _SERIAL_MESSENGER_H

#include "blockingconcurrentqueue.h"

#include "HostMessenger.hpp"

class SerialMessenger : HostMessenger<moodycamel::BlockingConcurrentQueue<DPDMessage>> {

public:
    // Constructor
    SerialMessenger(moodycamel::BlockingConcurrentQueue<DPDMessage> *queue, std::string state_dir, uint32_t cells_per_dimension, uint32_t max_timestep);

    // Setters
    void set_number_of_beads(uint32_t number_of_beads) override;

    // Blocking dequeue from the simulator
    DPDMessage await_message() override;

    // Wrapper to be able to run run()
    void run_wrapper();

};

#endif //_SERIAL_MESSENGER_H
