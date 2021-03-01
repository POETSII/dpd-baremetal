
#include "POLiteVolume.hpp"

#ifdef STATS
#define POLITE_DUMP_STATS
#define POLITE_COUNT_MSGS
#endif

#ifndef __POLITEVOLUME_IMPL
#define __POLITEVOLUME_IMPL

// Constructor
POLiteVolume::POLiteVolume(float volume_length, unsigned cells_per_dimension) : SimulationVolume<PGraph<DPDDevice, DPDState, None, DPDMessage> *>(volume_length, cells_per_dimension) {
    this->boxes_x = 1;
    this->boxes_y = 1;
    this->cells = new POLiteCells(cells_per_dimension, (float)(volume_length / cells_per_dimension), this->boxes_x, this->boxes_y);
}

// Write the volume
void POLiteVolume::write(void *dest) {
    POLiteCells *cells = (POLiteCells *)this->cells;
    cells->write(dest);
}

#endif /* __POLITEVOLUME_IMPL */
