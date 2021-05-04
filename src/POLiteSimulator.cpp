// Implementation file for the host simulation volume class

#include "POLiteSimulator.hpp"

#ifndef __POLITESIMULATOR_IMPL
#define __POLITESIMULATOR_IMPL

POLiteSimulator::POLiteSimulator(const ptype volume_length, const unsigned cells_per_dimension, uint32_t start_timestep, uint32_t max_timestep,
                                 std::string state_dir, uint32_t boxes_x, uint32_t boxes_y)
                               : Simulator(volume_length, cells_per_dimension, start_timestep, max_timestep, state_dir) {
    this->volume = new POLiteVolume(volume_length, cells_per_dimension, boxes_x, boxes_y);

    std::cout << "Acquiring Hostlink...\n";
    // Acquire Hostlink so can communicate with POETS hardware
    hostLink = new HostLink(boxes_x, boxes_y);

    std::cout << "HostLink acquired.     \n";

    std::cout << "Running on " << boxes_x * boxes_y << " box";
    if ((boxes_x * boxes_y) != 1) {
        std::cout << "es";
    }
    std::cout << ".\n";

// #ifdef VISUALISE
//     std::cout << "Preparing server for external connections...\r";
//     _extern = new ExternalServer("_external.sock");
//     std::cout << "External server ready.\n";
// #endif

    POLiteCells *cells = (POLiteCells *)volume->get_cells();
    cells->set_start_timestep(start_timestep);
    cells->set_end_timestep(max_timestep);

    this->messenger = new POLiteMessenger(hostLink, state_dir, cells_per_dimension, max_timestep);

}

POLiteSimulator::POLiteSimulator(const ptype volume_length, const unsigned cells_per_dimension, uint32_t start_timestep, uint32_t max_timestep,
                                 std::string state_dir)
                               : POLiteSimulator(volume_length, cells_per_dimension, start_timestep, max_timestep, state_dir, 1, 1) {};

// Run the simulation
void POLiteSimulator::run() {
    this->messenger->set_number_of_beads(this->volume->get_number_of_beads());

    hostLink->boot("code.v", "data.v");
    hostLink->go();

    std::cout << "Running...\n";

#if defined(STATS)
    politeSaveStats(hostLink, "stats.txt");
#endif

    // Run the host messenger to deal with messages from the hardware
    this->messenger->run_wrapper();
}

//Runs a test, gets the bead outputs and returns this to the test file
void POLiteSimulator::test(void *result) {
    std::map<uint32_t, bead_t> *res = (std::map<uint32_t, bead_t> *)result;
    uint32_t total_cells = volume->get_number_of_cells();
    uint32_t total_beads_in = volume->get_number_of_beads();
    // Finish counter
    uint32_t finish = 0;

    hostLink->boot("code.v", "data.v");
    hostLink->go();

    // enter the main loop
    while(1) {
        PMessage<DPDMessage> pmsg;
        hostLink->recvMsg(&pmsg, sizeof(pmsg));
        DPDMessage msg = pmsg.payload;

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
            #ifdef SERIAL
                thread.join();
            #endif
                std::cout << "\n";
                return;
            }
        }
    }

}

void POLiteSimulator::write() {
    this->volume->write(hostLink);
}

#endif /* __POLITESIMULATOR_IMPL */
