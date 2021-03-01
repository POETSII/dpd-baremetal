// A class that contains the simulation volume.
// used to generate and manage DPD simulation volumes.

#ifndef _RDF_VOLUME_H
#define _RDF_VOLUME_H

#include "Volume.hpp"
#include "RDFCells.hpp"

class RDFVolume : public Volume<double, std::vector<DPDState>> {

public:

    // Constructors and destructors
    RDFVolume(volume volume_length, unsigned cells_per_dimension);

};

#endif /*_RDF_VOLUME_H */
