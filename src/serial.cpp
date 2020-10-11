// Dissipative particle dynamics simulation serial x86 version.
// This is aimed at simulating DPD systems using the same code as the POLite
// version but serially on an x86 machine. This way, it will produce the same
// results, but we are not restricted by message sizes or instruction space
// and so can add traces and debugging to determine any issues with the POETS
// DPD code.

#include "serial.hpp"

/************** Setup functions ***************/
// Create a new cell
// Return the ID of this cell
PDeviceId SerialSim::newCell() {
    DPDState s;
    _cells.push_back(s);
    return _num_cells++;
}
// Get a pointer to a cell
DPDState* SerialSim::getCell(PDeviceId id) {
    return &_cells.at(id);
}

// Add cell b as a neighbour to cell a
void SerialSim::addEdge(PDeviceId a, PDeviceId b) {
    // Get cell a's state
    DPDState *state_a = getCell(a);
    // In the next array slot put cell b's ID
    // Increment the number of neighbours a has
    state_a->neighbours[state_a->num_neighbours++] = b;
}

// Set the start timestep
void SerialSim::setTimestep(uint32_t timestep) {
    _timestep = timestep;
}

// Set the max timestep
void SerialSim::setMaxTimestep(uint32_t maxTimestep) {
    _max_timestep = maxTimestep;
}

// Set the cell size
void SerialSim::setN(uint32_t N) {
    _N = N;
}

void SerialSim::setCellSize(ptype cell_size) {
    _cell_size = cell_size;
}

/************** DPD Functions ***************/
// dt10's random number generator
uint32_t SerialSim::p_rand(DPDState *s) {
    uint32_t c = (s->rngstate)>>32, x=(s->rngstate)&0xFFFFFFFF;
    s->rngstate = x*((uint64_t)429488355U) + c;
    return x^c;
}

// Initialise each cell
void SerialSim::init(DPDState *s) {
        s->grand = rand();
        s->sentslot = s->bslot;
}

// Calculate forces of neighbour cell's beads acting on this cells beads
void SerialSim::neighbour_forces(DPDState *local_state, DPDState *neighbour_state) {
    // Get the local bead map
    uint32_t i = local_state->bslot;
    // For each local bead calculate its interaction with neighbouring beads
    while (i) {
        // Get the local bead
        uint8_t ci = get_next_slot(i);
        bead_t local_bead = local_state->bead_slot[ci];
        // Get neighbour bead map
        uint32_t j = neighbour_state->bslot;
        // For each neighbour bead
        while (j) {
            // Get neighbour bead
            uint8_t cj = get_next_slot(j);
            bead_t neighbour_bead = neighbour_state->bead_slot[cj];
            bead_t b;
            b.id = neighbour_bead.id;
            b.type = neighbour_bead.type;
            b.pos.set(neighbour_bead.pos.x(), neighbour_bead.pos.y(), neighbour_bead.pos.z());
            b.velo.set(neighbour_bead.velo.x(), neighbour_bead.velo.y(), neighbour_bead.velo.z());
            // from the device locaton get the adjustments to the bead positions
            int x_rel = period_bound_adj(neighbour_state->loc.x - local_state->loc.x);
            int y_rel = period_bound_adj(neighbour_state->loc.y - local_state->loc.y);
            int z_rel = period_bound_adj(neighbour_state->loc.z - local_state->loc.z);

            // relative position for this particle to this device
            b.pos.x(b.pos.x() + ptype(x_rel));
            b.pos.y(b.pos.y() + ptype(y_rel));
            b.pos.z(b.pos.z() + ptype(z_rel));
            // Calculate the forces acting on the local bead
          #ifdef SMALL_DT_EARLY
            Vector3D<ptype> f = force_update(&local_bead, &b, local_state->grand, s.inv_sqrt_dt);
          #else
            Vector3D<ptype> f = force_update(&local_bead, &b, local_state->grand, inv_sqrt_dt);
          #endif
            // Convert this force to fixed point to make it deterministic
            Vector3D<int32_t> x = f.floatToFixed();
            // Add this to the force accumulator for this bead
            local_state->force_slot[ci] = local_state->force_slot[cj] + x;
            // Clear the local bead map of the neighbour bead
            j = clear_slot(j, cj);
        }
        // Clear the local bead map for the local bead
        i = clear_slot(i, ci);
    }
}

void SerialSim::migrate_bead(const bead_t migrating_bead, const cell_t dest, const PDeviceId *neighbours) {
    // Find which neighbour this is going to
    for (uint8_t i = 0; i < NEIGHBOURS; i++) {
        // Get the neighbour state
        DPDState *n_s = getCell(neighbours[i]);
        // Check whether this cell is the destination of the migrating bead
        if (n_s->loc.x == dest.x && n_s->loc.y == dest.y && n_s->loc.z == dest.z) {
            // Get the neighbours next available bead slot
            uint8_t ni = get_next_free_slot(n_s->bslot);
            // Set this slot in the bead map
            n_s->bslot = set_slot(n_s->bslot, ni);
            // Welcome the new  bead
        #ifndef BETTER_VERLET
            n_s->bead_slot[ni].type = migrating_bead.type;
            n_s->bead_slot[ni].id = migrating_bead.id;
            n_s->bead_slot[ni].pos.set(migrating_bead.pos.x(), migrating_bead.pos.y(), migrating_bead.pos.z());
            n_s->bead_slot[ni].velo.set(migrating_bead.velo.x(), migrating_bead.velo.y(), migrating_bead.velo.z());
            n_s->force_slot[ni].set(0.0, 0.0, 0.0);
        #else
            n_s->bead_slot[ni].type = migrating_bead.type;
            n_s->bead_slot[ni].id = migrating_bead.id;
            n_s->bead_slot[ni].pos->set(migrating_bead.pos.x(), migrating_bead.pos.y(), migrating_bead.pos.z());
            n_s->bead_slot[ni].acc.set(migrating_bead.acc.x(), migrating_bead.acc.y(), migrating_bead.acc.z());
            n_s->force_slot[ni].set(0.0, 0.0, 0.0);

            // Store old velocity
            n_s->old_velo[ni].set(migrating_bead.velo.x(), migrating_bead.velo.y(), migrating_bead.velo.z());
            // Update velocity
          #ifndef SMALL_DT_EARLY
            update_velocity(&n_s->bead_slot[ni], &n_s->old_velo[ni], dt);
          #else
            update_velocity(&n_s->bead_slot[ni], &n_s->old_velo[ni], s->dt);
          #endif
        #endif
            // We've migrated the bead, no need to keep looping
            return;
        }
    }
}

/************** Runtime functions ***************/
// Run the simulator
void SerialSim::run() {
    #ifdef VISUALISE
        _emitcnt = emitperiod;
    #endif

    // Initialise the system
    for (DPDState s : _cells) {
        init(&s);
    }

    // Main loop. Continues until an end point is reached
    while(1) {
        // UPDATE PHASE
        // For each cell
        for (DPDState s : _cells) {
            // Calculate forces of beads within this cell
          #ifdef SMALL_DT_EARLY
            local_calcs(s.inv_sqrt_dt, s.bslot, s.bead_slot, s.grand, s.force_slot);
          #else
            local_calcs(inv_sqrt_dt, s.bslot, s.bead_slot, s.grand, s.force_slot);
          #endif

            // Calculate forces acting on beads in this cell from beads in
            // neighbouring cells
            // For each neighbour
            for (PDeviceId n : s.neighbours) {
                DPDState *n_s = getCell(n);
                neighbour_forces(&s, n_s);
            }
        }

        // UPDATE COMPLETE

        // Increment timestep
        _timestep++;
    #if defined(TIMER)
        // Timed run has ended
        if (_timestep >= _max_timestep) {
            // Send a message to the host to indicate the end of a run
            DPDMessage msg;
            msg.type = 0xAA; // Indicates end of run
            // Emit this (add it to the queue)
            _emit_message(msg);
            // Exit the simulation
            return;
        }
    #endif

        // Velocity verlet and finding beads which are to be migrated
        // For each cell
        for (DPDState s : _cells) {
        #ifdef SMALL_DT_EARLY
            // If the timestep is 1000, we can increase the dt value
            if (_timestep == 1000) {
                s.dt = normal_dt;
                s.inv_sqrt_dt = normal_inv_sqrt_dt;
            }
        #endif

            // Advance the random number
            s.grand = p_rand(&s);

            // Get the bead map for this cell
            uint32_t i = s.bslot;
            // For each bead in this cell
            while(i) {
                // Get the bead
                uint8_t ci = get_next_slot(i);
                // Perform velocity Verlet on this bead to update its
                // velocity, position and acceleration
            #if defined(SMALL_DT_EARLY) && defined(BETTER_VERLET)
                velocity_Verlet(&s.bead_slot[ci], &s.force_slot[ci], &s.old_velo[ci], s.dt);
            #elif defined(SMALL_DT_EARLY)
                velocity_Verlet(&s.bead_slot[ci], &s.force_slot[ci], s.dt);
            #elif defined(BETTER_VERLET)
                velocity_Verlet(&s.bead_slot[ci], &s.force_slot[ci], &s.old_velo[ci], dt);
            #else
                velocity_Verlet(&s.bead_slot[ci], &s.force_slot[ci], dt);
            #endif

                // Now check if ant beads are migrating, and state this in the migrateslot
            #if defined(SMALL_DT_EARLY) && defined(BETTER_VERLET)
                migration(ci, &s.bead_slot[ci], _cell_size, s.loc, _N, &s.migrateslot, &s.migrate_loc[ci], s.dt, &s.old_velo[ci]);
            #elif defined(SMALL_DT_EARLY)
                migration(ci, &s.bead_slot[ci], _cell_size, s.loc, _N, &s.migrateslot, &s.migrate_loc[ci], s.dt);
            #elif defined(BETTER_VERLET)
                migration(ci, &s.bead_slot[ci], _cell_size, s.loc, _N, &s.migrateslot, &s.migrate_loc[ci], dt, &s.old_velo[ci]);
            #else
                migration(ci, &s.bead_slot[ci], _cell_size, s.loc, _N, &s.migrateslot, &s.migrate_loc[ci], dt);
            #endif

                // Clear this bead from the temporary bead map
                i = clear_slot(i, ci);
            }
        }

        // MIGRATION PHASE
        // For each cell
        for (DPDState s : _cells) {
            // Get the migrate bit map
            uint32_t i = s.migrateslot;
            // For each migrating bead
            while (i) {
                // Get the bead
                uint8_t ci = get_next_slot(i);
                bead_t migrating_bead = s.bead_slot[ci];
                // Get its destination
                cell_t dest = s.migrate_loc[ci];
                // Find the correct neighbour and add this bead to its state
                migrate_bead(migrating_bead, dest, s.neighbours);
                // Clear the bead slot -- it no longer belongs to us
                s.bslot = clear_slot(s.bslot, ci);
                // Clear this bead from the migrate slot
                i = clear_slot(i, ci);
            }
        }

        // MIGRATION COMPLETE
        // Do we want to emit the beads?
        bool emit = false;
    #ifdef VISUALISE
        if (_emitcnt >= emitperiod) {
            emit = true;
        } else {
            _emitcnt++;
        }
    #elif defined(TESTING)
        if (s->timestep >= s->max_timestep) {
            emit = true;
        }
    #endif

    #if defined(VISUALISE) || defined(TESTING)
        // EMIT MODE
        if (emit) {
            // Do something
        }
    #endif

    }
    std::cout << "COMPLETE\n";
}

bool SerialSim::hasMessage() {
    _mutex.lock();
    bool r = !_queue.empty();
    _mutex.unlock();
    return r;
}

DPDMessage SerialSim::getMessage() {
    // Acquire lock
    _mutex.lock();
    // Get the message
    DPDMessage msg = _queue.front();
    // Remove this from the queue
    _queue.pop();
    // Return the message
    _mutex.unlock();
    return msg;
}

void SerialSim::_emit_message(DPDMessage msg) {
    _mutex.lock();
    _queue.push(msg);
    _mutex.unlock();
}
