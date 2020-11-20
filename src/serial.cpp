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

void SerialSim::setQueue(moodycamel::BlockingConcurrentQueue<DPDMessage> *queue) {
    _queue = queue;
}

/************** DPD Functions ***************/
// Initialise each cell
void SerialSim::init(DPDState *s) {
    // Generate first global random number
    s->grand = p_rand(&s->rngstate);
    s->grand = p_rand(&s->rngstate);
}

// Calculate forces of neighbour cell's beads acting on this cells beads
// void SerialSim::neighbour_forces(DPDState *local_state, DPDState *neighbour_state, float *cons, float *drag, float *rand, float *bond1, float *bond2, double *rands, double *rands_mag, uint64_t *total_rands, double *rands_variance_total, double *rands_avg) {
void SerialSim::neighbour_forces(DPDState *local_state, DPDState *neighbour_state) {
    // Get the local bead map
    uint32_t i = local_state->bslot;
    // For each local bead calculate its interaction with neighbouring beads
    while (i) {
        // Get the local bead
        uint8_t ci = get_next_slot(i);
        bead_t *local_bead = &local_state->bead_slot[ci];
        // Get neighbour bead map
        uint32_t j = neighbour_state->bslot;
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
          #ifdef SMALL_DT_EARLY
            // Vector3D<ptype> f = force_update(local_bead, &b, local_state->grand, local_state->inv_sqrt_dt, cons, drag, rand, bond1, bond2, rands, rands_mag, total_rands, rands_variance_total, rands_avg);
            Vector3D<ptype> f = force_update(local_bead, &b, local_state->grand, local_state->inv_sqrt_dt);
          #else
            Vector3D<ptype> f = force_update(local_bead, b, local_state->grand, inv_sqrt_dt);
          #endif
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

void SerialSim::migrate_bead(const bead_t *migrating_bead, const cell_t dest, const PDeviceId *neighbours) {
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
          #ifndef SMALL_DT_EARLY
            update_velocity(&n_s->bead_slot[ni], &n_s->old_velo[ni], dt);
          #else
            update_velocity(&n_s->bead_slot[ni], &n_s->old_velo[ni], n_s->dt);
          #endif
        #endif
            // We've migrated the bead, no need to keep looping
            return;
        }
    }
}
/************** Runtime functions ***************/
// Send a message from the thread to the host
void SerialSim::sendMessage(DPDMessage *msg) {
    while (!_queue->try_enqueue(*msg)) { };
}

// Host receive a message from the thread
DPDMessage SerialSim::receiveMessage() {
    DPDMessage msg;
    while (!_queue->try_dequeue(msg)) { };
    return msg;
}

// Run the simulator
void SerialSim::run() {
    // Initialise the system
    for (uint32_t c = 0; c < _cells.size(); c++) {
        DPDState *s = getCell(c);
        init(s);
    }

    // float cons[4] = {0.0, 0.0, 0.0, 0.0};
    // float drag[4] = {0.0, 0.0, 0.0, 0.0};
    // float rand[4] = {0.0, 0.0, 0.0, 0.0};
    // float bond1[4] = {0.0, 0.0, 0.0, 0.0};
    // float bond2[4] = {0.0, 0.0, 0.0, 0.0};
    // float forcex[4] = {0.0, 0.0, 0.0, 0.0};
    // float forcey[4] = {0.0, 0.0, 0.0, 0.0};
    // float forcez[4] = {0.0, 0.0, 0.0, 0.0};
    // float oldvelox[4] = {0.0, 0.0, 0.0, 0.0};
    // float oldveloy[4] = {0.0, 0.0, 0.0, 0.0};
    // float oldveloz[4] = {0.0, 0.0, 0.0, 0.0};
    // float beadaccx[4] = {0.0, 0.0, 0.0, 0.0};
    // float beadaccy[4] = {0.0, 0.0, 0.0, 0.0};
    // float beadaccz[4] = {0.0, 0.0, 0.0, 0.0};
    // float beadvelox[4] = {0.0, 0.0, 0.0, 0.0};
    // float beadveloy[4] = {0.0, 0.0, 0.0, 0.0};
    // float beadveloz[4] = {0.0, 0.0, 0.0, 0.0};

    // double rands_total_variance = 0.0;
    // double rands = 0.0;
    // double rands_mag = 0.0;
    // double rands_avg = 0.0;
    // uint64_t total_rands = 0;

    // FILE* f = fopen("../2147483805-forces.csv", "w+");
    // fclose(f);
    // FILE* g = fopen("../2147483806-forces.csv", "w+");
    // fclose(g);
    // FILE* h = fopen("../2147487592-forces.csv", "w+");
    // fclose(h);
    // FILE* i = fopen("../2147487593-forces.csv", "w+");
    // fclose(i);
    // FILE* j = fopen("../2147483805-verlet.csv", "w+");
    // fclose(j);
    // FILE* k = fopen("../2147483806-verlet.csv", "w+");
    // fclose(k);
    // FILE* l = fopen("../2147487592-verlet.csv", "w+");
    // fclose(l);
    // FILE* m = fopen("../2147487593-verlet.csv", "w+");
    // fclose(m);
    // FILE* n = fopen("../rand-avg-variance.csv", "w+");
    // fclose(n);
    // // FILE* n = fopen("../rand-avg.csv", "w+");
    // // fclose(n);

    // // File holding Rand stuff
    // std::string rng_str("../rand-avg.csv");
    // std::ifstream rng_in(rng_str);

    // Main loop. Continues until an end point is reached
    while(1) {
        // std::string line;
        // std::getline(rng_in, line);
        // std::stringstream ss(line);
        // std::string s;
        // std::vector<std::string> rngs;
        // // Loop through the line, split it into separate parts
        // while (std::getline(ss, s, ',')) {
        //     // Remove whitespaces
        //     boost::trim(s);
        //     // Add to vector
        //     rngs.push_back(s);
        // }
        // rands = std::stof(rngs.at(1));
        // rands_mag = std::stof(rngs.at(2));
        // total_rands = std::stol(rngs.at(3));
        // double rands_avg = std::stof(rngs.at(4));
        // double rands_mag_avg = std::stof(rngs.at(5));

        // UPDATE PHASE
        // For each cell
        for (uint32_t c = 0; c < _cells.size(); c++) {
            DPDState *s = getCell(c);
            // Calculate forces of beads within this cell
          #ifdef SMALL_DT_EARLY
            // local_calcs(s->inv_sqrt_dt, s->bslot, s->bead_slot, s->grand, s->force_slot, cons, drag, rand, bond1, bond2, &rands, &rands_mag, &total_rands, &rands_total_variance, &rands_avg);
            local_calcs(s->inv_sqrt_dt, s->bslot, s->bead_slot, s->grand, s->force_slot);
          #else
            // local_calcs(inv_sqrt_dt, s->bslot, s->bead_slot, s->grand, s->force_slot, cons, drag, rand, bond1, bond2);
            local_calcs(inv_sqrt_dt, s->bslot, s->bead_slot, s->grand, s->force_slot);
          #endif

            // Calculate forces acting on beads in this cell from beads in
            // neighbouring cells
            // For each neighbour
            for (PDeviceId n : s->neighbours) {
                DPDState *n_s = getCell(n);
                // neighbour_forces(s, n_s, cons, drag, rand, bond1, bond2, &rands, &rands_mag, &total_rands, &rands_total_variance, &rands_avg);
                neighbour_forces(s, n_s);
            }
        }

        // UPDATE COMPLETE
        // Increment timestep
        _timestep++;
        std::cout << "Timestep " << _timestep << "\r";
        // FILE* f = fopen("../2147483805-forces.csv", "a+");
        // fprintf(f, "%u, %f, %f, %f, %f, %f\n", _timestep, cons[0], drag[0], rand[0], bond1[0], bond2[0]);
        // fclose(f);
        // FILE* g = fopen("../2147483806-forces.csv", "a+");
        // fprintf(g, "%u, %f, %f, %f, %f, %f\n", _timestep, cons[1], drag[1], rand[1], bond1[1], bond2[1]);
        // fclose(g);
        // FILE* h = fopen("../2147487592-forces.csv", "a+");
        // fprintf(h, "%u, %f, %f, %f, %f, %f\n", _timestep, cons[2], drag[2], rand[2], bond1[2], bond2[2]);
        // fclose(h);
        // FILE* i = fopen("../2147487593-forces.csv", "a+");
        // fprintf(i, "%u, %f, %f, %f, %f, %f\n", _timestep, cons[3], drag[3], rand[3], bond1[3], bond2[3]);
        // fclose(i);
        // FILE* j = fopen("../2147483805-verlet.csv", "a+");
        // fprintf(j, "%u, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f\n", _timestep, forcex[0], forcey[0], forcez[0], oldvelox[0], oldveloy[0], oldveloz[0], beadaccx[0], beadaccy[0], beadaccz[0], beadvelox[0], beadveloy[0], beadveloz[0]);
        // fclose(j);
        // FILE* k = fopen("../2147483806-verlet.csv", "a+");
        // fprintf(k, "%u, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f\n", _timestep, forcex[1], forcey[1], forcez[1], oldvelox[1], oldveloy[1], oldveloz[1], beadaccx[1], beadaccy[1], beadaccz[1], beadvelox[1], beadveloy[1], beadveloz[1]);
        // fclose(k);
        // FILE* l = fopen("../2147487592-verlet.csv", "a+");
        // fprintf(l, "%u, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f\n", _timestep, forcex[2], forcey[2], forcez[2], oldvelox[2], oldveloy[2], oldveloz[2], beadaccx[2], beadaccy[2], beadaccz[2], beadvelox[2], beadveloy[2], beadveloz[2]);
        // fclose(l);
        // FILE* m = fopen("../2147487593-verlet.csv", "a+");
        // fprintf(m, "%u, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f\n", _timestep, forcex[3], forcey[3], forcez[3], oldvelox[3], oldveloy[3], oldveloz[3], beadaccx[3], beadaccy[3], beadaccz[3], beadvelox[3], beadveloy[3], beadveloz[3]);
        // fclose(m);

        // cons[0] = 0.0; cons[1] = 0.0; cons[2] = 0.0; cons[3] = 0.0;
        // drag[0] = 0.0; drag[1] = 0.0; drag[2] = 0.0; drag[3] = 0.0;
        // rand[0] = 0.0; rand[1] = 0.0; rand[2] = 0.0; rand[3] = 0.0;
        // bond1[0] = 0.0; bond1[1] = 0.0; bond1[2] = 0.0; bond1[3] = 0.0;
        // bond2[0] = 0.0; bond2[1] = 0.0; bond2[2] = 0.0; bond2[3] = 0.0;
        // forcex[0] = 0.0; forcex[1] = 0.0; forcex[2] = 0.0; forcex[3] = 0.0;
        // forcey[0] = 0.0; forcey[1] = 0.0; forcey[2] = 0.0; forcey[3] = 0.0;
        // forcez[0] = 0.0; forcez[1] = 0.0; forcez[2] = 0.0; forcez[3] = 0.0;
        // oldvelox[0] = 0.0; oldvelox[1] = 0.0; oldvelox[2] = 0.0; oldvelox[3] = 0.0;
        // oldveloy[0] = 0.0; oldveloy[1] = 0.0; oldveloy[2] = 0.0; oldveloy[3] = 0.0;
        // oldveloz[0] = 0.0; oldveloz[1] = 0.0; oldveloz[2] = 0.0; oldveloz[3] = 0.0;
        // beadaccx[0] = 0.0; beadaccx[1] = 0.0; beadaccx[2] = 0.0; beadaccx[3] = 0.0;
        // beadaccy[0] = 0.0; beadaccy[1] = 0.0; beadaccy[2] = 0.0; beadaccy[3] = 0.0;
        // beadaccz[0] = 0.0; beadaccz[1] = 0.0; beadaccz[2] = 0.0; beadaccz[3] = 0.0;
        // beadvelox[0] = 0.0; beadvelox[1] = 0.0; beadvelox[2] = 0.0; beadvelox[3] = 0.0;
        // beadveloy[0] = 0.0; beadveloy[1] = 0.0; beadveloy[2] = 0.0; beadveloy[3] = 0.0;
        // beadveloz[0] = 0.0; beadveloz[1] = 0.0; beadveloz[2] = 0.0; beadveloz[3] = 0.0;

        // FILE* n = fopen("../rand-avg-variance.csv", "a+");
        // // FILE* n = fopen("../rand-avg.csv", "a+");
        // double rands_vairance = rands_total_variance / (total_rands - 1);
        // fprintf(n, "%u, %f, %f, %lu, %f, %f, %f, %f\n", _timestep, rands, rands_mag, total_rands, rands_avg, rands_mag_avg, rands_total_variance, rands_vairance);
        // // double rands_avg = rands / total_rands;
        // // double rands_mag_avg = rands_mag / total_rands;
        // // fprintf(n, "%u, %f, %f, %lu, %f, %f\n", _timestep, rands, rands_mag, total_rands, rands_avg, rands_mag_avg);
        // fclose(n);

        // rands_total_variance = 0.0;
        // rands = 0.0;
        // rands_mag = 0.0;
        // total_rands = 0;

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
            uint32_t i = s->bslot;
            // For each bead in this cell
            while(i) {
                // Get the bead
                uint8_t ci = get_next_slot(i);
                // Perform velocity Verlet on this bead to update its
                // velocity, position and acceleration
            #if defined(SMALL_DT_EARLY) && defined(BETTER_VERLET)
                // velocity_Verlet(&s->bead_slot[ci], &s->force_slot[ci], &s->old_velo[ci], s->dt, forcex, forcey, forcez, oldvelox, oldveloy, oldveloz, beadaccx, beadaccy, beadaccz, beadvelox, beadveloy, beadveloz);
                velocity_Verlet(&s->bead_slot[ci], &s->force_slot[ci], &s->old_velo[ci], s->dt);
            #elif defined(SMALL_DT_EARLY)
                velocity_Verlet(&s->bead_slot[ci], &s->force_slot[ci], s->dt);
            #elif defined(BETTER_VERLET)
                velocity_Verlet(&s->bead_slot[ci], &s->force_slot[ci], &s->old_velo[ci], dt);
            #else
                velocity_Verlet(&s->bead_slot[ci], &s->force_slot[ci], dt);
            #endif

                // Now check if ant beads are migrating, and state this in the migrateslot
            #if defined(SMALL_DT_EARLY) && defined(BETTER_VERLET)
                migration(ci, &s->bead_slot[ci], _cell_size, s->loc, _N, &s->migrateslot, &s->migrate_loc[ci], s->dt, &s->old_velo[ci]);
            #elif defined(SMALL_DT_EARLY)
                migration(ci, &s->bead_slot[ci], _cell_size, s->loc, _N, &s->migrateslot, &s->migrate_loc[ci], s->dt);
            #elif defined(BETTER_VERLET)
                migration(ci, &s->bead_slot[ci], _cell_size, s->loc, _N, &s->migrateslot, &s->migrate_loc[ci], dt, &s->old_velo[ci]);
            #else
                migration(ci, &s->bead_slot[ci], _cell_size, s->loc, _N, &s->migrateslot, &s->migrate_loc[ci], dt);
            #endif

                // Clear this bead from the temporary bead map
                i = clear_slot(i, ci);
            }
        }

        // MIGRATION PHASE
        // For each cell
        for (uint32_t c = 0; c < _cells.size(); c++) {
            DPDState *s = getCell(c);
            // Get the migrate bit map
            uint32_t i = s->migrateslot;
            // For each migrating bead
            while (i) {
                // Get the bead
                uint8_t ci = get_next_slot(i);
                bead_t *migrating_bead = &s->bead_slot[ci];
                // Get its destination
                cell_t dest = s->migrate_loc[ci];
                // Find the correct neighbour and add this bead to its state
                migrate_bead(migrating_bead, dest, s->neighbours);
                // Clear the bead slot -- it no longer belongs to us
                s->bslot = clear_slot(s->bslot, ci);
                // Clear this bead from the migrate slot
                i = clear_slot(i, ci);
            }
            s->migrateslot = 0;
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
                uint32_t i = s->bslot;
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
