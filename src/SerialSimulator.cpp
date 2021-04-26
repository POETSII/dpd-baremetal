// Dissipative particle dynamics simulation serial x86 version.
// This is aimed at simulating DPD systems using the same code as the POLite
// version but serially on an x86 machine. This way, it will produce the same
// results, but we are not restricted by message sizes or instruction space
// and so can add traces and debugging to determine any issues with the POETS
// DPD code.

#include "SerialSimulator.hpp"

/************** Setup functions ***************/
// Create a new cell
// Return the ID of this cell
PDeviceId SerialSimulator::newCell() {
    DPDState s;
    _cells.push_back(s);
    return _num_cells++;
}

// Get a pointer to a cell
DPDState* SerialSimulator::getCell(PDeviceId id) {
    return &_cells.at(id);
}

// Add cell b as a neighbour to cell a
void SerialSimulator::addEdge(PDeviceId a, PDeviceId b) {
    // Get cell a's state
    DPDState *state_a = getCell(a);
    // In the next array slot put cell b's ID
    // Increment the number of neighbours a has
    state_a->neighbours[state_a->num_neighbours++] = b;
}

// Set the start timestep
void SerialSimulator::setTimestep(uint32_t timestep) {
    _timestep = timestep;
}

// Set the max timestep
void SerialSimulator::setMaxTimestep(uint32_t maxTimestep) {
    _max_timestep = maxTimestep;
}

// Set the cell size
void SerialSimulator::setCellsPerDimension(uint32_t cells_per_dimension) {
    _cells_per_dimension = cells_per_dimension;
}

void SerialSimulator::setQueue(moodycamel::BlockingConcurrentQueue<DPDMessage> *queue) {
    _queue = queue;
}

/************** DPD Functions ***************/
// Initialise each cell
void SerialSimulator::init(DPDState *s) {
    // Generate first global random number
    s->grand = p_rand(&s->rngstate);
    s->grand = p_rand(&s->rngstate);
}

// Calculate forces of neighbour cell's beads acting on this cells beads
void SerialSimulator::neighbour_forces(DPDState *local_state, DPDState *neighbour_state) {
    // Get the local bead map
    uint16_t i = local_state->bslot;
    // For each local bead calculate its interaction with neighbouring beads
    while (i) {
        // Get the local bead
        uint8_t ci = get_next_slot(i);
        bead_t *local_bead = &local_state->bead_slot[ci];
        // Get neighbour bead map
        uint16_t j = neighbour_state->bslot;
        // For each neighbour bead
        while (j) {
            // Get neighbour bead
            uint8_t cj = get_next_slot(j);
            bead_t *neighbour_bead = &neighbour_state->bead_slot[cj];
            bead_t b;
            b.id = neighbour_bead->id;
            b.type = neighbour_bead->type;
            b.pos.set(neighbour_bead->pos.x(), neighbour_bead->pos.y(), neighbour_bead->pos.z());
            b.velo.set(neighbour_bead->velo.x(), neighbour_bead->velo.y(), neighbour_bead->velo.z());
            // from the device locaton get the adjustments to the bead positions
            int x_rel = period_bound_adj(neighbour_state->loc.x - local_state->loc.x);
            int y_rel = period_bound_adj(neighbour_state->loc.y - local_state->loc.y);
            int z_rel = period_bound_adj(neighbour_state->loc.z - local_state->loc.z);

            // relative position for this particle to this device
            b.pos.x(b.pos.x() + ptype(x_rel));
            b.pos.y(b.pos.y() + ptype(y_rel));
            b.pos.z(b.pos.z() + ptype(z_rel));
            // Calculate the forces acting on the local bead
            Vector3D<ptype> f = force_update(local_bead, &b, local_state);
            // Convert this force to fixed point to make it deterministic
        #ifndef FLOAT_ONLY
            Vector3D<int32_t> x = f.floatToFixed();
            // Add this to the force accumulator for this bead
            local_state->force_slot[ci] = local_state->force_slot[ci] + x;
        #else
            local_state->force_slot[ci] = local_state->force_slot[ci] + f;
        #endif
            // Clear the local bead map of the neighbour bead
            j = clear_slot(j, cj);
        }
        // Clear the local bead map for the local bead
        i = clear_slot(i, ci);
    }
}

void SerialSimulator::migrate_bead(const bead_t *migrating_bead, const cell_t dest, const PDeviceId *neighbours) {
    // Find which neighbour this is going to
    for (uint8_t i = 0; i < NEIGHBOURS; i++) {
        // Get the neighbour state
        DPDState *n_s = getCell(neighbours[i]);
        // Check whether this cell is the destination of the migrating bead
        if (n_s->loc.x == dest.x && n_s->loc.y == dest.y && n_s->loc.z == dest.z) {
            // Get the neighbours next available bead slot
            uint8_t ni = get_next_free_slot(n_s->bslot);
            if (ni == 0xFF) {
                std::cerr << "ERROR: No free slot for bead to migrate to. Terminating.\n";
                exit(0xFF);
            }
            // Set this slot in the bead map
            n_s->bslot = set_slot(n_s->bslot, ni);
            // Welcome the new  bead
        #ifndef BETTER_VERLET
            n_s->bead_slot[ni].type = migrating_bead->type;
            n_s->bead_slot[ni].id = migrating_bead->id;
            n_s->bead_slot[ni].pos.set(migrating_bead->pos.x(), migrating_bead->pos.y(), migrating_bead->pos.z());
            n_s->bead_slot[ni].velo.set(migrating_bead->velo.x(), migrating_bead->velo.y(), migrating_bead->velo.z());
            n_s->force_slot[ni].set(0.0, 0.0, 0.0);
        #else
            n_s->bead_slot[ni].type = migrating_bead->type;
            n_s->bead_slot[ni].id = migrating_bead->id;
            n_s->bead_slot[ni].pos.set(migrating_bead->pos.x(), migrating_bead->pos.y(), migrating_bead->pos.z());
            n_s->bead_slot[ni].acc.set(migrating_bead->acc.x(), migrating_bead->acc.y(), migrating_bead->acc.z());
            n_s->force_slot[ni].set(0.0, 0.0, 0.0);

            // Store old velocity
            n_s->old_velo[ni].set(migrating_bead->velo.x(), migrating_bead->velo.y(), migrating_bead->velo.z());
            // Update velocity
            update_velocity(ni, n_s);
        #endif
            // We've migrated the bead, no need to keep looping
            return;
        }
    }
}
/************** Runtime functions ***************/
// Send a message from the thread to the host
void SerialSimulator::sendMessage(DPDMessage *msg) {
    while (!_queue->try_enqueue(*msg)) { };
}

// Host receive a message from the thread
DPDMessage SerialSimulator::receiveMessage() {
    DPDMessage msg;
    while (!_queue->try_dequeue(msg)) { };
    return msg;
}

// Run the simulator
void SerialSimulator::run() {
    // Initialise the system
    for (uint32_t c = 0; c < _cells.size(); c++) {
        DPDState *s = getCell(c);
        init(s);
    }

    // Main loop. Continues until an end point is reached
    while(1) {
        // UPDATE PHASE
        // For each cell
        for (uint32_t c = 0; c < _cells.size(); c++) {
            DPDState *s = getCell(c);

        uint16_t bead_map = s->bslot; //All the beads in this cell
        while(bead_map) {
            // The bead (index) we want to calculate forces on
            uint8_t ci = get_next_slot(bead_map);

        #ifdef REDUCE_LOCAL_CALCS
            // Clear ci from the bead map, store it in another variable.
            // This variable now holds all the beads that haven't had their
            // force calculated yet, but forces are equal and opposite so we
            // can add the force to all beads in calc_map and subtract the
            // forces from ci.
            uint16_t calc_map = clear_slot(bead_map, ci);
           #ifndef SINGLE_FORCE_LOOP
            local_calcs(ci, calc_map, s);
           #else
            calc_bead_force_on_beads(&s->bead_slot[ci], calc_map, s, ci);
           #endif
        #else
           #ifndef SINGLE_FORCE_LOOP
            local_calcs(ci, bead_map, s);
           #else
            calc_bead_force_on_beads(&s->bead_slot[ci], bead_map, s);
           #endif
        #endif

            bead_map = clear_slot(bead_map, ci);
        }


            // Calculate forces acting on beads in this cell from beads in
            // neighbouring cells
            // For each neighbour
            for (PDeviceId n : s->neighbours) {
                DPDState *n_s = getCell(n);
                neighbour_forces(s, n_s);
            }
        }

        // UPDATE COMPLETE
        // Increment timestep
        _timestep++;
        std::cout << "Timestep " << _timestep << "\r";

    #ifdef TIMER
        // Timed run has ended
        if (_timestep >= _max_timestep) {
            // Send a message to the host to indicate the end of a run
            DPDMessage msg;
            msg.timestep = _timestep;
            msg.type = 0xAA; // Indicates end of run
            // Emit this (add it to the queue)
            sendMessage(&msg);
            // Exit the simulation
            return;
        }
    #endif

        // Velocity verlet and finding beads which are to be migrated
        // For each cell
        for (uint32_t c = 0; c < _cells.size(); c++) {
            DPDState *s = getCell(c);
        #ifdef SMALL_DT_EARLY
            // If the timestep is 1000, we can increase the dt value
            if (_timestep == 1000) {
                s->dt = normal_dt;
                s->inv_sqrt_dt = normal_inv_sqrt_dt;
            }
        #endif

            // Advance the random number
            s->grand = p_rand(&s->rngstate);

            // Get the bead map for this cell
            uint16_t i = s->bslot;
            // For each bead in this cell
            while(i) {
                // Get the bead
                uint8_t ci = get_next_slot(i);
                // Perform velocity Verlet on this bead to update its
                // velocity, position and acceleration
                velocity_Verlet(ci, s);

                // Now check if ant beads are migrating, and state this in the migrateslot
                migration(ci, s);

                // Clear this bead from the temporary bead map
                i = clear_slot(i, ci);
            }
        }

        // MIGRATION PHASE
        // Each cell must be passed over one at a time or we risk overflowing the bead array
        bool beads_to_migrate = true;
        // While there are beads to migrate
        while (beads_to_migrate) {
            // Set this to false for each pass
            beads_to_migrate = false;
            // For each cell
            for (uint32_t c = 0; c < _cells.size(); c++) {
                // Get the cell state
                DPDState *s = getCell(c);
                // For each migrating bead
                if (s->migrateslot) {
                    beads_to_migrate = true;
                    // Get the bead
                    uint8_t ci = get_next_slot(s->migrateslot);
                    bead_t *migrating_bead = &s->bead_slot[ci];
                    // Get its destination
                    cell_t dest = s->migrate_loc[ci];
                    // Find the correct neighbour and add this bead to its state
                    migrate_bead(migrating_bead, dest, s->neighbours);
                    // Clear the bead slot -- it no longer belongs to us
                    s->bslot = clear_slot(s->bslot, ci);
                    // Clear this bead from the migrate slot
                    s->migrateslot = clear_slot(s->migrateslot, ci);
                }
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
        if (_timestep >= _max_timestep) {
            emit = true;
        }
    #endif

    #if defined(VISUALISE) || defined(TESTING)
        // EMIT MODE
        if (emit) {
            // For each cell
            for (uint32_t c = 0; c < _cells.size(); c++) {
                DPDState *s = getCell(c);
                uint16_t i = s->bslot;
                while (i) {
                    uint8_t ci = get_next_slot(i);
                    bead_t *bead = &s->bead_slot[ci];
                    DPDMessage msg;
                    msg.type = 0;
                    msg.timestep = _timestep;
                    msg.from = s->loc;
                    msg.beads[0].id = bead->id;
                    msg.beads[0].type = bead->type;
                    msg.beads[0].pos.set(bead->pos.x(), bead->pos.y(), bead->pos.z());
                    msg.beads[0].velo.set(bead->velo.x(), bead->velo.y(), bead->velo.z());
                    sendMessage(&msg);

                    i = clear_slot(i, ci);
                }
            #ifdef TESTING
                DPDMessage msg;
                msg.type = 0xAA;
                sendMessage(&msg);
            #endif
            }
        #ifdef TESTING
            break;
        #elif defined(VISUALISE)
            if (_timestep >= _max_timestep) {
                break;
            }
        #endif
        }
    #endif
    }
    std::cout << "COMPLETE         \n";
}