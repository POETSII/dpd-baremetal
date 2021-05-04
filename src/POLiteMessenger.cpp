// Implementation file for Host messenger.
// This is what handles messaging coming from the simulation engine.

#include "POLiteMessenger.hpp"

#ifndef __POLite_MESSENGER_IMPL
#define __POLite_MESSENGER_IMPL

POLiteMessenger::POLiteMessenger(HostLink *queue, std::string state_dir,
                                 uint32_t cells_per_dimension, uint32_t max_timestep)
                              : HostMessenger(queue, state_dir, cells_per_dimension, max_timestep) {}

void POLiteMessenger::set_number_of_beads(uint32_t number_of_beads) {
    this->number_of_beads = number_of_beads;
}

DPDMessage POLiteMessenger::await_message() {
    PMessage<DPDMessage> pmsg;
    queue->recvMsg(&pmsg, sizeof(pmsg));
    return pmsg.payload;
}

void POLiteMessenger::run_wrapper() {
    run();
}

#endif //__POLite_MESSENGER_IMPL
