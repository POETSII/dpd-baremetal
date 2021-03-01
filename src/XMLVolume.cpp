// Implementation file for the host SimVolume class

#include "XMLVolume.hpp"

#ifdef STATS
#define XML_DUMP_STATS
#define XML_COUNT_MSGS
#endif

#ifndef __XMLVOLUME_IMPL
#define __XMLVOLUME_IMPL

// constructor
XMLVolume::XMLVolume(const float volume_length, const unsigned cells_per_dimension) : SimulationVolume<std::vector<DPDState>>(volume_length, cells_per_dimension) {
    this->cells = new XMLCells(volume_length, float(volume_length / cells_per_dimension));
}

void XMLVolume::write(void *dest) {
    FILE* f = (FILE*) dest;
}

#endif /* __XMLVOLUME_IMPL */
