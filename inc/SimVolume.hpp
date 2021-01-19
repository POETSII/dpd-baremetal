// A class that contains the simulation volume.
// used to generate and manage DPD simulation volumes.

#ifndef _SIM_UNIVERSE_H
#define _SIM_UNIVERSE_H

#ifndef SERIAL
#include "HostLink.h"
#endif
#include <sys/time.h>
#include <map>
#include "ExternalServer.hpp"
#include <thread>
#include "Volume.hpp"

#if defined(OUTPUT_MAPPING) || defined(MESSAGE_COUNTER)
    typedef struct _FPGALinks {
        std::vector<std::vector<uint64_t>> x;
        std::vector<std::vector<uint64_t>> y;
        std::vector<std::vector<uint64_t>> intra;
    } FPGALinks;

#endif

template<class S> // S is the type for this simulation i.e. fixap<C,F> or float
class SimVolume : public Volume<S> {
    public:

    // Constructors and destructors
    SimVolume(S volume_length, unsigned cells_per_dimension, uint32_t start_time, uint32_t max_time);
    ~SimVolume();

    // Setup
    // Checks to see if this bead can be added to the volume
    bool space_for_bead(const bead_t* in);
    // Checks to see if this pair of beads can be added to the volume
    bool space_for_bead_pair(const bead_t* a, const bead_t *b);

#if defined(OUTPUT_MAPPING) || defined(MESSAGE_COUNTER)
    void clearLinks(FPGALinks* links);
    void followEdge(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, FPGALinks* links);
    void updateLinkInfo(PThreadId cellAddr, cell_t cellLoc, FPGALinks* links);
    uint16_t locOffset(const uint16_t current, const int16_t offset, const float vol_max);
#endif

#ifdef OUTPUT_MAPPING
    void outputMapping(); // Print mapping as JSON
#endif

#ifdef MESSAGE_COUNTER
    void calculateMessagesPerLink(std::map<cell_t, uint32_t> cell_messages);
#endif

    // Setup
    // Make these two devices neighbours
    void addNeighbour(PDeviceId a, PDeviceId b);

    // Simulation control
    // Writes the simulation volume onto the POETS system
    // void write();
    // Runs the simulation
    // void run();
    // Runs a test, gets the bead positions and returns them for comparison
    // std::map<uint32_t, DPDMessage> test();

    // Debugging
    // Gets single dimension neighbour based on n which is -1, 0 or 1
    uint16_t get_neighbour_cell_dimension(cell_pos_t c, int16_t n);
    // Gets device ID for neighbouring cell. d_x, d_y and d_z are between -1 and 1 and used for to find the 26 neighbours
    PDeviceId get_neighbour_cell_id(cell_t u_i, int16_t d_x, int16_t d_y, int16_t d_z);
    // Find the distance between the given bead and its nearest bead
    float find_nearest_bead_distance(const bead_t *i, cell_t u_i);
    // Store the nearest bead distances for each bead in a JSON file
    void store_initial_bead_distances();

    protected:
    // First timestep to run. 0 if starting a new simulation. n if restarting a simulation
    // uint32_t start_timestep = 0;
    // // Last timestep to run. Not always used if simulation is to run indefinitely
    // uint32_t max_timestep = 0;

    // Runtime variables
    // These are used so the simulation finishes after a given period of time.
    // When the simulation emits its state, these are checked, and only then
    // will the simulation terminate. This ensures we have complete states when
    // the simulation ends.
    // uint32_t _runtime_hours = 0;
    // uint32_t _runtime_minutes = 0;
    // uint32_t _runtime_seconds = 0;
    // timeval is used to get seconds of runtime, so calculate_runtime takes
    // the above variables and converts them into seconds
    // (stored in _runtime_seconds).
    // void calculate_runtime();

	// For the external connection
    // ExternalServer *_extern;

	// Measuring performance
	// struct timeval _start, _finish, _diff;

    // Box mesh dimensions
    // uint32_t _boxesX, _boxesY;
    // Board mesh dimensions
    // uint32_t _boardsX, _boardsY;

#if defined(MESSAGE_COUNTER) || defined(OUTPUT_MAPPING)
    FPGALinks _link_messages, _link_edges;
#endif

};

#include "../src/SimVolume.cpp"

#endif /*_SIM_VOLUME_H */
