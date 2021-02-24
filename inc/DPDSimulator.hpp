// A class that contains a DPD volume

#include "Executor.hpp"
#include "SimVolume.hpp"

#ifndef _DPDSIMULATOR_H
#define _DPDSIMULATOR_H

class DPDSimulator : public Executor<SimVolume<float>> {
    public:

    // Constructors and destructors
    DPDSimulator(const ptype volume_length, const unsigned cells_per_dimension, uint32_t start_timestep, uint32_t max_timestep);
    ~DPDSimulator() {}

    // Write the volume into the simulator memory
    virtual void write() = 0;

    protected:

    uint32_t start_timestep;
    uint32_t max_timestep;

    // Runtime variables - So we can run until a given time is elapsed
    uint32_t runtime_hours, runtime_minutes, runtime_seconds;
    void calculate_runtime();

};

// #include "../src/DPDSimulator.cpp"

#endif /*_DPDSIMULATOR_H */
