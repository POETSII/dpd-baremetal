// Implementation file for the host simulation volume class

#include "DPDSimulator.hpp"

#ifndef __DPDSIMULATOR_IMPL
#define __DPDSIMULATOR_IMPL

DPDSimulator::DPDSimulator(const ptype volume_length, const unsigned cells_per_dimension, uint32_t start_timestep, uint32_t max_timestep) : Executor<SimVolume<float>>(volume_length, cells_per_dimension) {
    this->start_timestep = start_timestep;
    this->max_timestep = max_timestep;

    std::cout << "Preparing simulator\n";
    std::cout << "Start timestep = " << start_timestep << "\n";
    if (max_timestep >= start_timestep) {
        std::cout << "Last timestep  = " << max_timestep << "\n";
    }
}

void DPDSimulator::calculate_runtime() {
    // Hours to minutes
    runtime_minutes += runtime_hours * 60; // Hours * 60 minutes per hour
    // Minutes to seconds
    runtime_seconds += runtime_minutes * 60; // Minutes * 60 seconds per minute;
}

#endif /* __DPDSIMULATOR_IMPL */
