// A class that contains the simulation universe
// used to manage the host side dpd-application

#ifndef _SIM_PARSE_UNIVERSE_H
#define _SIM_PARSE_UNIVERSE_H

#include "parsedDPD.h"
#include "POLite.h"
#include "HostLink.h"
#include <sys/time.h>
#include <map>
#include "ExternalServer.hpp"
#include "DPDSimulation.hpp"

const uint8_t max_beads_per_dev = 15;

template<class S> // S is the type for this simulation i.e. fixap<C,F> or float
class Universe {
    public:
    // constructors and destructors
    Universe(DPDSimulation sim);
    ~Universe();

    // setup
    unit_t addBead(const bead_t *in); // adds a bead to the universe
    bool spaceForBead(const bead_t *in); // checks to see if this bead can be added to the universe
    void addNeighbour(PDeviceId a, PDeviceId b); // make these two devices neighbours

    // simulation control
    void write(); // writes the simulation env onto the POETS system
    PThreadId get_thread_from_loc(unit_t loc); // Use unit_t location to acquire thread id
    void run(); // runs the simulation
    std::map<uint32_t, DPDMessage> test(); // Runs a test, gets the bead outputs and returns this to the test file

    // bead slot management
    uint32_t clear_slot(uint32_t slotlist, uint8_t pos);
    uint32_t set_slot(uint32_t slotlist, uint8_t pos);
    bool is_slot_set(uint32_t slotlist, uint8_t pos);

    uint32_t get_next_slot(uint32_t slotlist);
    uint32_t get_next_free_slot(uint32_t slotlist);

    void print_slot(uint32_t slotlist);
    uint8_t get_num_beads(uint32_t slotlist);

    // debugging
    void print_occupancy(); // prints the number of beads assigned to each devices

    private:
        // Volume dimensions
        Vector3D<S> _volume;
        // Cell dimensions
        Vector3D<S> _cell;
        // Total number of cells
        uint _numCells;
     //        S _size;
     //        unsigned _D;


	// POLite related stuff
	PGraph<DPDDevice, DPDState, None, DPDMessage> * _g; // the graph
        HostLink *_hostLink; // the hostlink

        // maintain a map of ID's to locations in the space
        std::map<PDeviceId, unit_t> _idToLoc;
        std::map<unit_t, PDeviceId> _locToId;

	// for the external connection
    #ifdef VISUALISE
        ExternalServer *_extern;
    #endif

    // Box mesh dimensions
    uint32_t _boxesX, _boxesY;

    uint32_t _max_time;

    void createDevices();
    void connectDevices();
    uint16_t locOffset(const uint16_t current, const int16_t offset, const float max);
    void initialiseCells(DPDSimulation sim);
    void addBeads(DPDSimulation sim);
    std::vector<bead_t> expandPolymer(Polymer_structure p, DPDSimulation sim);
};

#include "../src/parseUniverse.cpp"

#endif /*_SIM_PARSE_UNIVERSE_H */
