
#include "RDFVolume.hpp"

#ifndef __RDF_VOLUME_IMPL
#define __RDF_VOLUME_IMPL

// Constructor
RDFVolume::RDFVolume(float volume_length, unsigned cells_per_dimension) : Volume<double, std::vector<DPDState>>(volume_length, cells_per_dimension) {
    this->cells = new RDFCells(cells_per_dimension, (float)(volume_length / cells_per_dimension));
}

#endif /* __RDF_VOLUME_IMPL */
