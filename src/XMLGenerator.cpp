// Implementation file for the host simulation volume class

#include "XMLGenerator.hpp"

#ifndef __XMLGENERATOR_IMPL
#define __XMLGENERATOR_IMPL

XMLGenerator::XMLGenerator(const ptype volume_length, const unsigned cells_per_dimension, uint32_t start_timestep, uint32_t max_timestep) : Simulator<XMLVolume<float>>(volume_length, cells_per_dimension, start_timestep, max_timestep) {

}

#endif /* __XMLGENERATOR_IMPL */
