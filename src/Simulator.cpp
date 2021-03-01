// Implementation file for the host simulation volume class

#include "Simulator.hpp"

#ifndef __SIMULATOR_IMPL
#define __SIMULATOR_IMPL

template <class V>
Simulator<V>::Simulator(const float volume_length, const unsigned cells_per_dimension, uint32_t start_timestep, uint32_t max_timestep) : Executor<V>() {
    this->start_timestep = start_timestep;
    this->max_timestep = max_timestep;

    std::cout << "Preparing simulator\n";
    std::cout << "Start timestep = " << start_timestep << "\n";
    if (max_timestep >= start_timestep) {
        std::cout << "Last timestep  = " << max_timestep << "\n";
    }
}

template <class V>
void Simulator<V>::calculate_runtime() {
    // Hours to minutes
    runtime_minutes += runtime_hours * 60; // Hours * 60 minutes per hour
    // Minutes to seconds
    runtime_seconds += runtime_minutes * 60; // Minutes * 60 seconds per minute;
}

#endif /* __SIMULATOR_IMPL */
