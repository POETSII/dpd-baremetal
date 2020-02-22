// A class that contains the simulation universe
// used to manage the host side dpd-application

#ifndef _SIM_UNIVERSE_H
#define _SIM_UNIVERSE_H

#ifndef GALS
#include "dpd.h"
#else
#include "dpdGALS.h"
#endif
#include "POLite.h"
#include "HostLink.h"
#include <sys/time.h>
#include <map>
#include "ExternalServer.hpp"

const uint8_t max_beads_per_dev = 7;

#if defined(OUTPUT_MAPPING) || defined(MESSAGE_COUNTER)
    typedef struct _FPGALinks {
        std::vector<std::vector<uint64_t>> x;
        std::vector<std::vector<uint64_t>> y;
    } FPGALinks;

#endif

template<class S> // S is the type for this simulation i.e. fixap<C,F> or float
class Universe {
    public:

    // constructors and destructors
    Universe(S size, unsigned D, uint32_t max_time);
    ~Universe();

    // setup
    unit_t add(const bead_t* in); // adds a bead to the universe
    void add(const unit_t bead, const bead_t* in); // adds a bead to the universe in given cell space. Value of all positions must be less than unit length
    bool space(const bead_t* in); // checks to see if this bead can be added to the universe
    bool space(const bead_t* a, const bead_t *b); // checks to see if this pair of beads can be added to the universe
    void addNeighbour(PDeviceId a, PDeviceId b); // make these two devices neighbours

#if defined(OUTPUT_MAPPING) || defined(MESSAGE_COUNTER)
    void clearLinks(FPGALinks* links);
    void followEdge(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, FPGALinks* links);
    void updateLinkInfo(PThreadId cellAddr, unit_t cellLoc, FPGALinks* links);
    uint16_t locOffset(const uint16_t current, const int16_t offset, const float vol_max);
#endif

#ifdef OUTPUT_MAPPING
    void outputMapping(); // Print mapping as JSON
#endif

#ifdef MESSAGE_COUNTER
    void calculateMessagesPerLink(std::map<unit_t, uint32_t> cell_messages);
#endif

    // simulation control
    void write(); // writes the simulation env onto the POETS system
    PThreadId get_thread_from_loc(unit_t loc); // Use unit_t location to acquire thread id
    void run(uint32_t max_time); // runs the simulation
    std::map<uint32_t, DPDMessage> test(); // Runs a test, gets the bead outputs and returns this to the test file

    // bead slot management
    uint8_t clear_slot(uint8_t slotlist, uint8_t pos);
    uint8_t set_slot(uint8_t slotlist, uint8_t pos);
    bool is_slot_set(uint8_t slotlist, uint8_t pos);

    uint8_t get_next_slot(uint8_t slotlist);
    uint8_t get_next_free_slot(uint8_t slotlist);

    void print_slot(uint8_t slotlist);
    uint8_t get_num_beads(uint8_t slotlist);

    // debugging
    void print_occupancy(); // prints the number of beads assigned to each devices

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

	// for the external connection
        ExternalServer *_extern;

	// measuring performance
	struct timeval _start, _finish, _diff;

    // Box mesh dimensions
    uint32_t _boxesX, _boxesY;
    // Board mesh dimensions
    uint32_t _boardsX, _boardsY;

    FPGALinks _link_messages, _link_edges;
};

#include "../src/universe.cpp"

#endif /*_SIM_UNIVERSE_H */
