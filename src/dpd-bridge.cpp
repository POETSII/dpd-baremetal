// this program connects to the remote POETS dpd simulation and emits a json file and commands to the dpd-vis repository

#include "ExternalClient.hpp"
#include "Vector3D.hpp"
#include <stdio.h>
#include <stdint.h>
#include <experimental/filesystem>
#include "dpd.h"
namespace fs = std::experimental::filesystem;

int main(){
    ExternalClient dpd_pipe("_external.sock");

    uint32_t timestep=0;
    std::ofstream out;
    out.open("_state.json");
    pts_to_extern_t msg;

    // do this for the first one while to figure out the starting timestep
    out << "{ \n";
    out << "  \"beads\":[\n";
    // std::cerr << "{ \n";
    // std::cerr << "  \"beads\":[\n";
    while(!dpd_pipe.tryRecv(&msg)) { }
    timestep = msg.timestep;

    float x_off = msg.from.x;// * (problem_size/(float)N);
    float y_off = msg.from.y;// * (problem_size/(float)N);
    float z_off = msg.from.z;// * (problem_size/(float)N);

    out << "\t{\"id\":" << msg.bead.id <<", \"x\":"<< msg.bead.pos.x() + x_off <<", \"y\":"<<msg.bead.pos.y() + y_off<<", \"z\":"<<msg.bead.pos.z() + z_off<<", \"vx\":"<<msg.bead.velo.x()<<", \"vy\":"<<msg.bead.velo.y()<<", \"vz\":"<<msg.bead.velo.z()<<", \"type\":"<< (uint32_t) msg.bead.type <<"}";
    out << ",\n";
    // std::cerr << "\t{\"id\":" << msg.bead.id <<", \"x\":"<< msg.bead.pos.x() + x_off <<", \"y\":"<<msg.bead.pos.y() + y_off<<", \"z\":"<<msg.bead.pos.z() + z_off<<", \"vx\":"<<msg.bead.velo.x()<<", \"vy\":"<<msg.bead.velo.y()<<", \"vz\":"<<msg.bead.velo.z()<<", \"type\":"<< (uint32_t) msg.bead.type <<"}";
    // std::cerr << ",\n";

    // for rate limiting the output
    clock_t last_emit = clock();

    while(1) {
	    if(dpd_pipe.tryRecv(&msg)){
         if(timestep != msg.timestep) {
             timestep = msg.timestep;
             // close up the JSON file and print the u command to stdout (dpd-vis weirdness)
             out.seekp(-2,std::ios::end);
             out << "  \n";
             out << "]}\n";
             // std::cerr << "  \n";
             // std::cerr << "]}\n";
             out.close();
             // copy the file into state.json
             fs::copy("_state.json", "state.json", fs::copy_options::overwrite_existing);

             // Add code here to rate limit the output of the data
             if ((float(clock() - last_emit) / CLOCKS_PER_SEC) > 0.2) {
                  printf("u\n"); fflush(stdout);
                  last_emit = clock();
             }

             // setup the next state document
             out.open("_state.json");
             out << "{ \n";
             out << "  \"beads\":[\n";
             // std::cerr << "{ \n";
             // std::cerr << "  \"beads\":[\n";
         }

         x_off = msg.from.x; // * (problem_size/(float)N);
         y_off = msg.from.y; // * (problem_size/(float)N);
         z_off = msg.from.z; // * (problem_size/(float)N);

         out << "\t{\"id\":" << msg.bead.id<<", \"x\":"<<msg.bead.pos.x() + x_off<<", \"y\":"<<msg.bead.pos.y() + y_off<<", \"z\":"<<msg.bead.pos.z() + z_off<<", \"vx\":"<<msg.bead.velo.x()<<", \"vy\":"<<msg.bead.velo.y()<<", \"vz\":"<<msg.bead.velo.z()<<", \"type\":"<<(uint32_t)msg.bead.type<<"}";
         out << ",\n";
         // std::cerr << "\t{\"id\":" << msg.bead.id<<", \"x\":"<<msg.bead.pos.x() + x_off<<", \"y\":"<<msg.bead.pos.y() + y_off<<", \"z\":"<<msg.bead.pos.z() + z_off<<", \"vx\":"<<msg.bead.velo.x()<<", \"vy\":"<<msg.bead.velo.y()<<", \"vz\":"<<msg.bead.velo.z()<<", \"type\":"<<(uint32_t)msg.bead.type<<"}";
         // std::cerr << ",\n";
	    }

    }

}
