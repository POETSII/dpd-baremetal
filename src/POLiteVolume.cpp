
#include "POLiteVolume.hpp"

#ifdef STATS
#define POLITE_DUMP_STATS
#define POLITE_COUNT_MSGS
#endif

#ifndef __POLITEVOLUME_IMPL
#define __POLITEVOLUME_IMPL

// Constructor
POLiteVolume::POLiteVolume(float volume_length, unsigned cells_per_dimension, uint32_t boxes_x, uint32_t boxes_y) : SimulationVolume<PGraph<DPDDevice, DPDState, None, DPDMessage> *>(volume_length, cells_per_dimension) {
    this->boxes_x = boxes_x;
    this->boxes_y = boxes_y;
    this->cells = new POLiteCells(cells_per_dimension, (float)(volume_length / cells_per_dimension), this->boxes_x, this->boxes_y);
}

POLiteVolume::POLiteVolume(float volume_length, unsigned cells_per_dimension) : POLiteVolume(volume_length, cells_per_dimension, 1, 1) {
}

// Write the volume
void POLiteVolume::write(void *dest) {
    POLiteCells *cells = (POLiteCells *)this->cells;
    cells->write(dest);
}

#endif /* __POLITEVOLUME_IMPL */
