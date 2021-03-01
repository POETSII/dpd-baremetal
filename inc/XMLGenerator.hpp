// A class that contains a DPD volume and is used to generate an XML for use by
// the Orchestrator

#include "Simulator.hpp"
#include "XMLVolume.hpp"

#ifndef _XMLGENERATOR_H
#define _XMLGENERATOR_H

class XMLGenerator : public Simulator<XMLVolume<float>> {
    public:

    // Constructors and destructors
    XMLGenerator(const ptype volume_length, const unsigned cells_per_dimension, uint32_t start_timestep, uint32_t max_timestep);
    ~XMLGenerator() {}

    protected:

};

#endif /*_XMLGENERATOR_H */
