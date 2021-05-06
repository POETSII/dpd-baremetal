// Dissipative particle dynamics simulation serial x86 version.
// This is aimed at simulating DPD systems using the same code as the POLite
// version but serially on an x86 machine. This way, it will produce the same
// results, but we are not restricted by message sizes or instruction space
// and so can add traces and debugging to determine any issues with the POETS
// DPD code.

#include "SerialSimulator.hpp"
#include "SerialMessenger.hpp"

#ifndef _SERIAL_SIM_IMPL
#define _SERIAL_SIM_IMPL

/************** Constructor functions ***************/
SerialSimulator::SerialSimulator(const ptype volume_length, const unsigned cells_per_dimension, uint32_t start_timestep, uint32_t max_timestep, std::string state_dir) : Simulator(volume_length, cells_per_dimension, start_timestep, max_timestep, state_dir) {
    // Generate the volume
    this->volume = new SerialVolume(volume_length, cells_per_dimension);

// #ifdef VISUALISE
//     std::cout << "Preparing server for external connections...\r";
//     _extern = new ExternalServer("_external.sock");
//     std::cout << "External server ready.\n";
// #endif

    // Get the cells and set their start and end timestep
    SerialCells *cells = (SerialCells *)volume->get_cells();
    cells->set_start_timestep(start_timestep);
    cells->set_end_timestep(max_timestep);

    // This thread will deal with messages from the simulator
    this->messenger = new SerialMessenger(&queue, state_dir, cells_per_dimension, max_timestep);

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
    // Get the neighbour bead map
    uint16_t i = neighbour_state->bslot;
    // For each neighbour bead calculate its affect on local beads
    while (i) {
        // Get the neighbour bead
        uint8_t ci = get_next_slot(i);
        bead_t *neighbour_bead = &neighbour_state->bead_slot[ci];

        bead_t b = get_relative_bead(neighbour_bead, &local_state->loc, &neighbour_state->loc);

    #ifndef SINGLE_FORCE_LOOP
        uint16_t j = local_state->bslot;
        // For each local bead
        while (j) {
            // Get local bead
            uint8_t cj = get_next_slot(j);
            bead_t *local_bead = &local_state->bead_slot[cj];

            // Calculate the forces acting on the local bead
            Vector3D<ptype> f = force_update(local_bead, &b, local_state);
            // Convert this force to fixed point to make it deterministic
        #ifndef FLOAT_ONLY
            Vector3D<int32_t> x = f.floatToFixed();
            // Add this to the force accumulator for this bead
            local_state->force_slot[cj] = local_state->force_slot[cj] + x;
        #else
            local_state->force_slot[cj] = local_state->force_slot[cj] + f;
        #endif
            // Clear the local bead map of the neighbour bead
            j = clear_slot(j, cj);
        }
    #else
        calc_bead_force_on_beads(&b, local_state->bslot, local_state);
    #endif
        // Clear the local bead map for the local bead
        i = clear_slot(i, ci);
    }
}

void SerialSimulator::migrate_bead(const bead_t *migrating_bead, const cell_t dest, const std::vector<PDeviceId> neighbours) {
    // Find which neighbour this is going to
    for (uint8_t i = 0; i < NEIGHBOURS; i++) {
        // Get the neighbour state
        DPDState *n_s = ((SerialCells *)this->volume->get_cells())->get_cell_state(neighbours.at(i));
        // Check whether this cell is the destination of the migrating bead
        if (n_s->loc.x == dest.x && n_s->loc.y == dest.y && n_s->loc.z == dest.z) {
            // Get the neighbours next available bead slot
            uint8_t ni = get_next_free_slot(n_s->bslot);
            if (ni == 0xFF) {
                std::cerr << "ERROR: No free slot for bead to migrate to. Terminating.\n";
                exit(0xFF);
            }
            // Set this slot in the bead map
            uint16_t before_slot = n_s->bslot;
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
    while (!queue.try_enqueue(*msg)) { };
}

// Host receive a message from the thread
DPDMessage SerialSimulator::receiveMessage() {
    DPDMessage msg;
    while (!queue.try_dequeue(msg)) { };
    return msg;
}

void SerialSimulator::write() {
    //Dummy function, there is nothing to write with the serial simulator
}

// Run the simulator
void SerialSimulator::run() {
    // Set the number of beads in the host messenger so it can use this to determine termination
    this->messenger->set_number_of_beads(this->volume->get_number_of_beads());

    // Spawn a thread that handles the messages from this
  #ifndef TESTING
    std::thread messaging_thread = std::thread(&SerialMessenger::run_wrapper, this->messenger);

    std::cout << "Running...\n";
  #endif

    uint32_t total_beads = 0;
    // Initialise the system
    for (uint32_t c = 0; c < this->volume->get_number_of_cells(); c++) {
        DPDState *s = ((SerialCells *)this->volume->get_cells())->get_cell_state(c);
        init(s);
        total_beads += get_num_beads(s->bslot);
    }


    // Main loop. Continues until an end point is reached
    while(1) {
        // UPDATE PHASE
        // For each cell
        for (uint32_t c = 0; c < this->volume->get_number_of_cells(); c++) {
            DPDState *s = ((SerialCells *)this->volume->get_cells())->get_cell_state(c);

        uint16_t bead_map = s->bslot; //All the beads in this cell
    #if defined(SINGLE_FORCE_LOOP) || defined(REDUCE_LOCAL_CALCS)
        // Loop through the local beads, finding each ones force acting
        // on the other local beads.
        while(bead_map) {
            // The bead (index) we want to calculate forces on
            uint8_t ci = get_next_slot(bead_map);
            bead_map = clear_slot(bead_map, ci);
        #ifdef REDUCE_LOCAL_CALCS
            // Clear ci from the bead map, store it in another variable.
            // This variable now holds all the beads that haven't had their
            // force calculated yet, but forces are equal and opposite so we
            // can add the force to all beads in calc_map and subtract the
            // forces from ci.
           #ifndef SINGLE_FORCE_LOOP
            local_calcs(ci, calc_map, s);
           #else
            calc_bead_force_on_beads(&s->bead_slot[ci], bead_map, s, ci);
           #endif
        #else
           #ifdef SINGLE_FORCE_LOOP
            calc_bead_force_on_beads(&s->bead_slot[ci], s->bslot, s);
           #endif
        #endif
        }
    #else
        // This will take state and do local calculations automatically
        local_calcs(s);
    #endif

            // Calculate forces acting on beads in this cell from beads in
            // neighbouring cells
            // For each neighbour
            for (PDeviceId n : s->neighbours) {
                DPDState *n_s = ((SerialCells *)this->volume->get_cells())->get_cell_state(n);
                neighbour_forces(s, n_s);
            }
        }

        // UPDATE COMPLETE
        // Increment timestep
        ((SerialCells *)this->volume->get_cells())->increment_timestep();
        // Print to the screen (because we can do this in a serial simulator)
        std::cout << "Timestep " << ((SerialCells *)this->volume->get_cells())->get_timestep() << "\r";
        fflush(stdout);

    #ifdef TIMER
        // Timed run has ended
        if (((SerialCells *)this->volume->get_cells())->reached_max_timestep()) {
            // Send a message to the host to indicate the end of a run
            DPDMessage msg;
            msg.timestep = ((SerialCells *)this->volume->get_cells())->get_timestep();
            msg.type = 0xAA; // Indicates end of run
            // Emit this (add it to the queue)
            sendMessage(&msg);
            // Exit the simulation
            break;
        }
    #endif

        // Velocity verlet and finding beads which are to be migrated
        // For each cell
        for (uint32_t c = 0; c < this->volume->get_number_of_cells(); c++) {
            DPDState *s = ((SerialCells *)this->volume->get_cells())->get_cell_state(c);

        #ifdef SMALL_DT_EARLY
            // If the timestep is 1000, we can increase the dt value
            if (((SerialCells *)this->volume->get_cells())->get_timestep() == 1000) {
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
            for (uint32_t c = 0; c < this->volume->get_number_of_cells(); c++) {
                // Get the cell state
                DPDState *s = ((SerialCells *)this->volume->get_cells())->get_cell_state(c);
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
        if (((SerialCells *)this->volume->get_cells())->emitting()) {
            emit = true;
        } else {
            ((SerialCells *)this->volume->get_cells())->increment_emitcnt();
        }
    #elif defined(TESTING)
        if (((SerialCells *)this->volume->get_cells())->reached_max_timestep()) {
            emit = true;
        }
    #endif

    #if defined(VISUALISE) || defined(TESTING)
        // EMIT MODE
        if (emit) {
            // For each cell
            for (uint32_t c = 0; c < this->volume->get_number_of_cells(); c++) {
                DPDState *s = ((SerialCells *)this->volume->get_cells())->get_cell_state(c);
                uint16_t i = s->bslot;
                while (i) {
                    uint8_t ci = get_next_slot(i);
                    bead_t *bead = &s->bead_slot[ci];
                    DPDMessage msg;
                    msg.type = 0;
                    msg.timestep = ((SerialCells *)this->volume->get_cells())->get_timestep();
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
            if (((SerialCells *)this->volume->get_cells())->reached_max_timestep()) {
                break;
            }
        #endif
        }
    #endif
    }
    std::cout << "COMPLETE         \n";
  #ifndef TESTING
    messaging_thread.join();
  #endif
    return;
}

void SerialSimulator::test(void *result) {
    std::map<uint32_t, bead_t> *res = (std::map<uint32_t, bead_t> *)result;
    uint32_t total_cells = volume->get_number_of_cells();
    uint32_t total_beads_in = volume->get_number_of_beads();
    // Finish counter
    uint32_t finish = 0;

    // Start the simulation
    std::thread simulation_thread = std::thread(&SerialSimulator::run, this);

    // enter the main loop
    while(1) {
        DPDMessage msg = receiveMessage();

        if (msg.type == 0xE0) {
            std::cout << "ERROR: A cell was too full at timestep " << msg.timestep << "\n";
            std::cout << "Cell " << msg.from.x << ", " << msg.from.y << ", " << msg.from.z << "\n";
            std::cout << "Num beads = " << msg.beads[0].id << "\n";
            exit(1);
        }

        if (msg.type != 0xAB) {
            bead_t b;
            b.id = msg.beads[0].id;
            b.type = msg.beads[0].type;
            b.pos.set(msg.beads[0].pos.x() + msg.from.x, msg.beads[0].pos.y() + msg.from.y, msg.beads[0].pos.z() + msg.from.z);
            (*res)[b.id] = b;
            std::cout << "Finished = " << finish << "/" << total_cells << " ";
            std::cout << "Beads = " << res->size() << "/" << total_beads_in << "\r";
        }

        if (msg.type == 0xAA || msg.type == 0xAB) {
            finish++;
            std::cout << "Finished = " << finish << "/" << total_cells << " ";
            std::cout << "Beads = " << res->size() << "/" << total_beads_in << "\r";
            if (finish >= total_cells && res->size() >= total_beads_in) {
                std::cout << "\n";
                simulation_thread.join();
                return;
            }
        }
    }

}

#endif /*_SERIAL_SIM_IMPL */
