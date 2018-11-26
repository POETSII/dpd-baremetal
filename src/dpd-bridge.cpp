// this program connects to the remote POETS dpd simulation and emits a json file and commands to the dpd-vis repository

#include "ExternalClient.hpp"
#include <stdio.h>

int main(){
    ExternalClient dpd_pipe("_external.sock");

    pts_to_extern_t msg;
    while(1) {
	if(dpd_pipe.tryRecv(&msg)){
             printf("recvd a message timestep=%d\n", msg.timestep);
	}

    }

}
