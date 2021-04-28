// Implementation file for the host simulation volume class

#include "Simulator.hpp"

#ifndef __SIMULATOR_IMPL
#define __SIMULATOR_IMPL

template <class V>
Simulator<V>::Simulator(const float volume_length, const unsigned cells_per_dimension, uint32_t start_timestep, uint32_t max_timestep, std::string state_dir) : Executor<V>() {
    this->start_timestep = start_timestep;
    this->max_timestep = max_timestep;

    std::cout << "Preparing simulator\n";
    std::cout << "Start timestep = " << start_timestep << "\n";
    if (max_timestep >= start_timestep) {
        std::cout << "Last timestep  = " << max_timestep << "\n";
    }

    this->state_dir = state_dir;

}

template <class V>
uint32_t Simulator<V>::get_max_timestep() {
    return this->max_timestep;
}

#endif /* __SIMULATOR_IMPL */
