// Implementation file for the host simulation volume class

#include "XMLGenerator.hpp"

#ifndef __XMLGENERATOR_IMPL
#define __XMLGENERATOR_IMPL

XMLGenerator::XMLGenerator(const float volume_length, const unsigned cells_per_dimension, uint32_t start_timestep, uint32_t max_timestep) : Simulator<XMLVolume>(volume_length, cells_per_dimension, start_timestep, max_timestep) {
    this->volume = new XMLVolume(volume_length, cells_per_dimension);
}

// Generate the XML
void XMLGenerator::run() {

}

// Generate the XML, put it in result for testing
void XMLGenerator::test(void *result) {

}

// Write the generated XML to a file
void XMLGenerator::write() {

}

#endif /* __XMLGENERATOR_IMPL */
