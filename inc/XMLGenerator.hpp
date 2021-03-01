// A class that contains a DPD volume and is used to generate an XML for use by
// the Orchestrator

#include "Simulator.hpp"
#include "XMLVolume.hpp"

#ifndef _XMLGENERATOR_H
#define _XMLGENERATOR_H

class XMLGenerator : public Simulator<XMLVolume> {
    public:

    // Constructors and destructors
    XMLGenerator(const float volume_length, const unsigned cells_per_dimension, uint32_t start_timestep, uint32_t max_timestep);
    ~XMLGenerator() {};

    // Functions which must be implemented
    // Generate the XML
    void run() override;
    // Generate the XML, put it in result for testing
    void test(void * result) override;
    // Write the generated XML to a file
    void write() override;

protected:
    // Holds the xml
    std::string xml;

};

#endif /*_XMLGENERATOR_H */
