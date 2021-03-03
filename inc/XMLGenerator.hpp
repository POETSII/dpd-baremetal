// A class that contains a DPD volume and is used to generate an XML for use by
// the Orchestrator

#include "Simulator.hpp"
#include "XMLVolume.hpp"

#ifndef _XMLGENERATOR_H
#define _XMLGENERATOR_H

#include <sstream>
#include <iomanip>
#include <fstream>

class XMLGenerator : public Simulator<XMLVolume> {
    public:

    // Constructors and destructors
    XMLGenerator(const float volume_length, const unsigned cells_per_dimension, uint32_t start_timestep, uint32_t max_timestep, std::string out_path);
    ~XMLGenerator() {};

    // Functions which must be implemented
    // Generate the XML
    void run() override;
    // Generate the XML, put it in result for testing
    void test(void * result) override;
    // Write the generated XML to a file
    void write() override;

protected:
    // Path to the graph type
    std::string graph_type;
    // Holds the xml
    std::string xml;
    // Holds the output filepath
    std::string out_path;

    // Generator functions for the graph instance
    std::string generate_graph_instance();
    std::string generate_device_instances();
    std::string generate_device_instance(cell_t loc);
    std::string generate_edge_instances();
    std::string generate_input_edge_instances_for_cell(cell_t loc);

};

#endif /*_XMLGENERATOR_H */
