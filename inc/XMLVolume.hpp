// A class that contains the simulation volume.
// used to generate and manage DPD simulation volumes.

#ifndef _XMLVOLUME_H
#define _XMLVOLUME_H

#include "SimulationVolume.hpp"
#include "XMLCells.hpp"

class XMLVolume : public SimulationVolume<std::vector<DPDState>> {

public:

    // Constructors and destructors
    XMLVolume(const float volume_length, const unsigned cells_per_dimension);

    void write(void *dest) override;

};

// #include "../src/XMLVolume.cpp"

#endif /*_XMLVOLUME_H */
