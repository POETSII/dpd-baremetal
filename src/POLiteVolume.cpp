
#include "POLiteVolume.hpp"

#ifdef STATS
#define POLITE_DUMP_STATS
#define POLITE_COUNT_MSGS
#endif

#ifndef __POLITEVOLUME_IMPL
#define __POLITEVOLUME_IMPL

// Constructor
template<class S>
POLiteVolume<S>::POLiteVolume(S volume_length, unsigned cells_per_dimension) : SimulationVolume<S, PGraph<DPDDevice, DPDState, None, DPDMessage> *>(volume_length, cells_per_dimension) {
    this->boxes_x = 1;
    this->boxes_y = 1;
    this->cells = new POLiteCells(cells_per_dimension, (float)(volume_length / cells_per_dimension), this->boxes_x, this->boxes_y);
}

// Write the volume
template<class S>
void POLiteVolume<S>::write(void *dest) {
    POLiteCells *cells = (POLiteCells *)this->cells;
    cells->write(dest);
}

#endif /* __POLITEVOLUME_IMPL */
