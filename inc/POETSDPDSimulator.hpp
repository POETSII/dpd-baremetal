// A class that contains a DPD volume

#include "DPDSimulator.hpp"

#ifndef _POETSDPDSIMULATOR_H
#define _POETSDPDSIMULATOR_H

#include "HostLink.h"

#include "ExternalServer.hpp"
#include "SimVolume.hpp"

#ifdef STATS
#define POLITE_DUMP_STATS
#define POLITE_COUNT_MSGS
#endif

class POETSDPDSimulator : public DPDSimulator {
    public:

    // Constructors and destructors
    POETSDPDSimulator(const ptype volume_length, const unsigned cells_per_dimension, uint32_t start_timestep, uint32_t max_timestep);
    ~POETSDPDSimulator() {}

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

// #include "../src/POETSDPDSimulator.cpp"

#endif /*_POETSDPDSIMULATOR_H */
