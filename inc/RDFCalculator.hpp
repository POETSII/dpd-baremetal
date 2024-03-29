// Class definition of the RDFCalculator.
// This can be run alongside a simulation, or on its own, as threads
// This will take a volume at a certain state, and calculate the RDF for all
// types.

#include <unistd.h>

#include "Executor.hpp"
#include "RDFVolume.hpp"
#include "blockingconcurrentqueue.h"

#ifndef _RDFCALCULATOR_H
#define _RDFCALCULATOR_H

enum Progress { waiting, running, finished };

struct RDFMessage {
    uint32_t timestep;
    Progress progress;
    double percent;
    int core;
};

class RDFCalculator : public Executor<RDFVolume> {
    public:

    // Constructors and destructors
    RDFCalculator(double volume_length, unsigned cells_per_dimension, uint32_t timestep, uint8_t number_density, uint8_t number_bead_types, std::vector<std::vector<std::vector<double>>> *results, moodycamel::BlockingConcurrentQueue<RDFMessage> *message_queue);
    ~RDFCalculator() { };

    // Calculator control
    void run() override;
    void test(void *result) override;

    // Communication with main thread
    void send_message(RDFMessage msg);

    // Getters and setters
    uint32_t get_timestep();

    protected:

    uint32_t timestep;
    uint8_t number_density;
    uint8_t number_bead_types;
    std::vector<std::vector<std::vector<double>>> *results;
    double rmax;
    double min_r;
    double max_r;
    double dr;

    moodycamel::BlockingConcurrentQueue<RDFMessage> *message_queue;

    cell_t getNeighbourLoc(cell_t c, uint16_t n_x, uint16_t n_y, uint16_t n_z);
    int16_t period_bound_adj(int16_t dim);

};

// #include "../src/RDFCalculator.cpp"

#endif /*_RDFCALCULATOR_H */
