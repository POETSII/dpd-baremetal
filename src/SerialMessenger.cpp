// Implementation file for Host messenger.
// This is what handles messaging coming from the simulation engine.

#include "SerialMessenger.hpp"

#ifndef __SERIAL_MESSENGER_IMPL
#define __SERIAL_MESSENGER_IMPL

SerialMessenger::SerialMessenger(moodycamel::BlockingConcurrentQueue<DPDMessage> *queue, std::string state_dir,
                                 uint32_t cells_per_dimension, uint32_t max_timestep)
                              : HostMessenger(queue, state_dir, cells_per_dimension, max_timestep) {}

void SerialMessenger::set_number_of_beads(uint32_t number_of_beads) {
    this->number_of_beads = number_of_beads;
}

DPDMessage SerialMessenger::await_message() {
    DPDMessage msg;
    queue->wait_dequeue(msg);
    return msg;
}

void SerialMessenger::run_wrapper() {
    run();
}

#endif //__SERIAL_MESSENGER_IMPL
