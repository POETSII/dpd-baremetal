// A class that contains the simulation volume.
// used to generate and manage DPD simulation volumes.

#ifndef _POLITEVOLUME_H
#define _POLITEVOLUME_H

#include "SimulationVolume.hpp"
#include "POLiteCells.hpp"

class POLiteVolume : public SimulationVolume<PGraph<DPDDevice, DPDState, None, DPDMessage> *> {

public:

    // Constructors and destructors
    POLiteVolume(float volume_length, unsigned cells_per_dimension);

    // Simulation setup
    // Write the volume data onto the POETS hardware
    void write(void *dest) override;

};

// #include "../src/POLiteVolume.cpp"

#endif /*_POLITEVOLUME_H */
