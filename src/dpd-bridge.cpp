// this program connects to the remote POETS dpd simulation and emits a json file and commands to the dpd-vis repository

#include "ExternalClient.hpp"
#include "Vector3D.hpp"
#include <stdio.h>
#include <stdint.h>
#include <experimental/filesystem>
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
    while(!dpd_pipe.tryRecv(&msg)) { }
    timestep = msg.timestep;
    out << "\t{\"id\":" << msg.bead.id<<", \"x\":"<<msg.bead.pos.x()<<", \"y\":"<<msg.bead.pos.y()<<", \"z\":"<<msg.bead.pos.z()<<", \"vx\":"<<msg.bead.velo.x()<<", \"vy\":"<<msg.bead.velo.y()<<", \"vz\":"<<msg.bead.velo.z()<<", \"type\":"<<msg.bead.type<<"}";
    out << ",\n";
     

    while(1) {
	    if(dpd_pipe.tryRecv(&msg)){
         if(timestep != msg.timestep) {
             // close up the JSON file and print the u command to stdout (dpd-vis weirdness)
             out.seekp(-2,std::ios::end);
             out << "  \n";
             out << "]}\n"; 
             out.close();
             // copy the file into state.json
             fs::copy("_state.json", "state.json", fs::copy_options::overwrite_existing);
             printf("u\n"); fflush(stdout);
             
             // setup the next state document  
             out.open("_state.json");
             out << "{ \n";
             out << "  \"beads\":[\n";
         }
         out << "\t{\"id\":" << msg.bead.id<<", \"x\":"<<msg.bead.pos.x()<<", \"y\":"<<msg.bead.pos.y()<<", \"z\":"<<msg.bead.pos.z()<<", \"vx\":"<<msg.bead.velo.x()<<", \"vy\":"<<msg.bead.velo.y()<<", \"vz\":"<<msg.bead.velo.z()<<", \"type\":"<<msg.bead.type<<"}";
         out << ",\n";
	    }

    }

}
