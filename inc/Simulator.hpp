// A class that contains a DPD volume

#include <iostream>

#include "Executor.hpp"

#ifndef _SIMULATOR_H
#define _SIMULATOR_H

template <class V>
class Simulator : public Executor<V> {
    public:

    // Constructors and destructors
    Simulator(const float volume_length, const unsigned cells_per_dimension, uint32_t start_timestep, uint32_t max_timestep);
    ~Simulator() {}

    // Write the volume into the simulator memory
    virtual void write() = 0;

    protected:

    uint32_t start_timestep;
    uint32_t max_timestep;

    // Runtime variables - So we can run until a given time is elapsed
    uint32_t runtime_hours, runtime_minutes, runtime_seconds;
    void calculate_runtime();

};

#include "../src/Simulator.cpp"

#endif /*_SIMULATOR_H */
