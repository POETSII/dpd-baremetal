// Implementation file for the host SimulationVolume class

#include "SerialVolume.hpp"

#ifndef __SERIAL_VOLUME_IMPL
#define __SERIAL_VOLUME_IMPL

// Constructor
SerialVolume::SerialVolume(float volume_length, unsigned cells_per_dimension) : SimulationVolume<std::vector<DPDState> *>(volume_length, cells_per_dimension) {
    this->cells = new SerialCells(cells_per_dimension, (float)(volume_length / cells_per_dimension));
}

// Dummy - There is no writing for a serial simulator
void SerialVolume::write(void *dest) {
    // SerialCells *cells = (SerialCells *)this->cells;
    // cells->write(dest);
}

#endif /* __SERIAL_VOLUME_IMPL */
