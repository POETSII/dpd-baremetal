
#include "Vector3D.hpp"
#include "BeadMap.hpp"

typedef float ptype;

typedef uint8_t bead_class_t; // the type of the bead, we are not expecting too many

typedef uint32_t bead_id_t; // the ID for the bead

typedef uint16_t cell_pos_t;

typedef struct _bead_t {
    bead_id_t id; // 4 bytes
    bead_class_t type; // 1 byte
    Vector3D<ptype> pos; // 12 bytes
    Vector3D<ptype> velo; // 12 bytes
#ifdef BETTER_VERLET
    Vector3D<ptype> acc; // 12 bytes
#endif
} bead_t; // 29 bytes - 41 with BETTER_VERLET

// defines the cell location
struct cell_t {
    cell_pos_t x;
    cell_pos_t y;
    cell_pos_t z;

    #ifndef TINSEL // below is only needed for the host code

    // So that we can use the co-ordinate of the cell as a key
    bool operator<(const cell_t& coord) const {
        if(x < coord.x) return true;
        if(x > coord.x) return false;
        //x == coord.x
        if(y < coord.y) return true;
        if(y > coord.y) return false;
        //x == coord.x && y == coord.y
        if(z < coord.z) return true;
        if(z > coord.z) return false;
        //*this == coord
        return false;
    }

    #endif /* TINSEL */

}; // 6 bytes

#ifdef GALS
struct DPDMessage {
    uint8_t mode;
    uint8_t type;
    uint32_t timestep; // The timestep this message is from
    uint8_t total_beads; // Used for sending cycle counts
    cell_t from; // The cell that this message is from - 6 bytes
    bead_t beads[1]; // The beads payload from this unit - 29 bytes - 42 with BETTER_VERLET
}; // 42 bytes - 54 bytes with BETTER_VERLET

struct DPDState {
    float cell_length; // The size of this spatial unit in one dimension
    uint8_t cells_per_dimension; // The length of one side of the volume in number of cells
    cell_t loc; // The location of this cell
    uint16_t bslot; // A bitmap of which bead slot is occupied
    uint16_t sentslot; // A bitmap of which bead slot has not been sent from yet
    uint16_t newBeadMap;
    bead_t bead_slot[MAX_BEADS]; // at most we have five beads per device
#ifdef FLOAT_ONLY
    Vector3D<float> force_slot[MAX_BEADS];
#else
    Vector3D<int32_t> force_slot[MAX_BEADS]; // force for each bead
#endif
#ifdef BETTER_VERLET
    Vector3D<ptype> old_velo[MAX_BEADS]; // Store old velocites for verlet
#endif
    uint16_t migrateslot; // a bitmask of which bead slot is being migrated in the next phase
    cell_t migrate_loc[MAX_BEADS]; // slots containing the destinations of where we want to send a bead to
    uint8_t mode; // the mode that this device is in.
#ifdef VISUALISE
    uint32_t emitcnt; // a counter to kept track of updates between emitting the state
#endif
    uint32_t timestep; // the current timestep that we are on
    uint32_t grand; // the global random number at this timestep
    uint64_t rngstate; // the state of the random number generator

    // uint32_t lost_beads;
    uint32_t max_timestep;

    uint8_t updates_received;
    uint8_t update_completes_received;
    uint8_t migrations_received;
    uint8_t migration_completes_received;
    uint8_t emit_complete_sent;
    uint8_t emit_completes_received;
    uint8_t updates_sent;
    uint8_t migrates_sent;
    int32_t total_update_beads;
    int32_t total_migration_beads;

#ifdef SMALL_DT_EARLY
    ptype dt;
    ptype inv_sqrt_dt;
#endif

#ifdef MESSAGE_MANAGEMENT
    int8_t msgs_to_recv; // Number of messages expected from neighbours. Will only send when all neighbours have sent at least one message
    uint8_t nbs_complete; // Neighbours which are not expected to send any more. Works in tandem with the above
#endif
    uint8_t error; // Error code to be returned to the host
};

#elif defined(SERIAL)
struct DPDMessage {
    uint8_t type;
    uint32_t timestep; // the timestep this message is from
    cell_t from; // the unit that this message is from - 6 bytes
    bead_t beads[1]; // the beads payload from this unit - 29 bytes - 41 with BETTER_VERLET
}; // 40 Bytes - 52 Bytes with BETTER_VERLET

struct DPDState {
    PDeviceId neighbours[NEIGHBOURS]; // Holds a list of the neighbours of this cell
    uint8_t num_neighbours = 0; // Holds how many neighbours this cell currently has
    cell_t loc; // the location of this cube
    uint16_t bslot = 0; // a bitmap of which bead slot is occupied
    uint16_t sentslot = 0; // a bitmap of which bead slot has not been sent from yet
    bead_t bead_slot[MAX_BEADS]; // at most we have five beads per device
#ifdef FLOAT_ONLY
    Vector3D<float> force_slot[MAX_BEADS];
#else
    Vector3D<int32_t> force_slot[MAX_BEADS]; // force for each bead
#endif
#ifdef BETTER_VERLET
    Vector3D<ptype> old_velo[MAX_BEADS]; // Store old velocites for verlet
#endif
    uint16_t migrateslot = 0; // a bitmask of which bead slot is being migrated in the next phase
    cell_t migrate_loc[MAX_BEADS]; // slots containing the destinations of where we want to send a bead to
    uint8_t mode = 0; // the mode that this device is in 0 = update; 1 = migration

    uint32_t grand = 0; // the global random number at this timestep
    uint64_t rngstate = 0; // the state of the random number generator

    uint32_t lost_beads = 0; // Beads lost due to the cell having a full bead_slot

#ifdef SMALL_DT_EARLY
    ptype dt = 0.0;
    ptype inv_sqrt_dt = 0.0;
#endif
};

#elif defined(RDF)

struct DPDState {
    cell_t loc = {0, 0, 0}; // the location of this cube
    uint16_t bslot = 0; // a bitmap of which bead slot is occupied
    bead_t bead_slot[MAX_BEADS]; // at most we have five beads per device
};

#else // SYNC
struct DPDMessage {
    uint8_t type;
    uint32_t timestep; // the timestep this message is from
    cell_t from; // the unit that this message is from - 6 bytes
    bead_t beads[1]; // the beads payload from this unit - 29 bytes - 41 with BETTER_VERLET
}; // 40 Bytes - 52 with BETTER_VERLET

struct DPDState {
    float cell_length; // the size of this spatial unit in one dimension
    uint8_t cells_per_dimension;
    cell_t loc; // the location of this cube
    uint16_t bslot; // a bitmap of which bead slot is occupied
    uint16_t sentslot; // a bitmap of which bead slot has not been sent from yet
    bead_t bead_slot[MAX_BEADS]; // at most we have five beads per device
#ifdef FLOAT_ONLY
    Vector3D<float> force_slot[MAX_BEADS];
#else
    Vector3D<int32_t> force_slot[MAX_BEADS]; // force for each bead
#endif
#ifdef BETTER_VERLET
    Vector3D<ptype> old_velo[MAX_BEADS]; // Store old velocites for verlet
#endif
    uint16_t migrateslot; // a bitmask of which bead slot is being migrated in the next phase
    cell_t migrate_loc[MAX_BEADS]; // slots containing the destinations of where we want to send a bead to
    uint8_t mode; // the mode that this device is in 0 = update; 1 = migration
#ifdef VISUALISE
    uint32_t emitcnt; // a counter to kept track of updates between emitting the state
#endif
    uint32_t timestep; // the current timestep that we are on
    uint32_t grand; // the global random number at this timestep
    uint64_t rngstate; // the state of the random number generator
    uint32_t max_timestep; // Maximum timestep for this run

#ifdef SMALL_DT_EARLY
    ptype dt;
    ptype inv_sqrt_dt;
#endif

#ifdef MESSAGE_MANAGEMENT
    int8_t msgs_to_recv; // Number of messages expected from neighbours. Will only send when all neighbours have sent at least one message
    uint8_t nbs_complete; // Neighbours which are not expected to send any more. Works in tandem with the above
#endif

#ifdef MESSAGE_COUNTER
    uint32_t message_counter;
#endif

    uint8_t error;
};
#endif
