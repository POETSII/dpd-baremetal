// A class that contains the simulation universe
// used to manage the host side dpd-application

#include "dpd.h"
#include "POLite.h"
#include "HostLink.h"
#include <sys/time.h>
#include <map>

#ifndef _SIM_UNIVERSE_H
#define _SIM_UNIVERSE_H

const uint8_t max_beads_per_dev = 5;

template<class S> // S is the type for this simulation i.e. fixap<C,F> or float
class Universe {
    public:
    // constructors and destructors
    Universe(S size, unsigned D);
    ~Universe();

    // setup
    void add(const bead_t* in); // adds a bead to the universe
    void addNeighbour(PDeviceId a, PDeviceId b); // make these two devices neighbours

    // simulation control
    void write(); // writes the simulation env onto the POETS system
    void run(); // runs the simulation 

    private:
        S _size;
        unsigned _D;
	S _unit_size;

	// POLite related stuff
	PGraph<DPDDevice, DPDState, None, DPDMessage> * _g; // the graph
        HostLink *_hostLink; // the hostlink

        // maintain a map of ID's to locations in the space
        std::map<PDeviceId, unit_t> _idToLoc;
        std::map<unit_t, PDeviceId> _locToId;

	// measuring performance
	struct timeval _start, _finish, _diff;
};

#include "../src/universe.cpp"

#endif /*_SIM_UNIVERSE_H */
