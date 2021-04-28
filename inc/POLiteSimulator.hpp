// A class that contains a DPD volume

#include "Simulator.hpp"

#ifndef _POLITESIMULATOR_H
#define _POLITESIMULATOR_H

#include "HostLink.h"

#include "ExternalServer.hpp"
#include "POLiteVolume.hpp"

#ifdef STATS
#define POLITE_DUMP_STATS
#define POLITE_COUNT_MSGS
#endif

class POLiteSimulator : public Simulator<POLiteVolume> {
    public:

    // Constructors and destructors
    POLiteSimulator(const ptype volume_length, const unsigned cells_per_dimension, uint32_t start_timestep, uint32_t max_timestep, std::string state_dir, uint32_t boxes_x, uint32_t boxes_y);
    POLiteSimulator(const ptype volume_length, const unsigned cells_per_dimension, uint32_t start_timestep, uint32_t max_timestep, std::string state_dir);
    ~POLiteSimulator() {}

    // Simulation control
    void write() override;
    void run() override;
    void test(void *result) override;

    protected:

    // For communicating with POETS
    HostLink *hostLink;

    // For sending bead data to externals
    ExternalServer *_extern;

};

// #include "../src/POLiteSimulator.cpp"

#endif /*_POLITESIMULATOR_H */
