// A class that contains the simulation volume for a serial simulation.

#ifndef _SERIAL_VOLUME_H
#define _SERIAL_VOLUME_H

#include "SimulationVolume.hpp"
#include "SerialCells.hpp"

class SerialVolume : public SimulationVolume<std::vector<DPDState> *> {

public:

    // Constructors and destructors
    SerialVolume(const float volume_length, const unsigned cells_per_dimension);

    // Simulation setup
    // Dummy - There is no writing for a serial simulator
    void write(void *dest) override;

};

#include "../src/SerialVolume.cpp"

#endif /*_SERIAL_VOLUME_H */
