// Implementation file for the host simulation universe class

#include "universe.hpp"

#ifndef __UNIVERSE_IMPL
#define __UNIVERSE_IMPL

// make two devices neighbours
template<class S>
void Universe<S>::addNeighbour(PDeviceId a, PDeviceId b){
     _g->addEdge(a,0,b);
}

// constructor
template<class S>
Universe<S>::Universe(S size, unsigned D){
    _size = size;
    _D = D;
    _unit_size = _size / S(D);
    _g = new PGraph<DPDDevice, DPDState, None, DPDMessage>();
    _hostLink = new HostLink();

    // create the devices
    for(uint16_t x=0; x<D; x++) {
       for(uint16_t y=0; y<D; y++) {
             for(uint16_t z=0; z<D; z++) {
                    PDeviceId id = _g->newDevice();
                    unit_t loc = {x, y, z};
                    _idToLoc[id] = loc;
                    _locToId[loc] = id;
             }
        }
    }

    // connect all the devices together appropriately
    // a toroidal space (cube with periodic boundaries)
    for(uint16_t x=0; x<_D; x++) {
       for(uint16_t y=0; y<_D; y++) {
          for(uint16_t z=0; z<_D; z++) {
              // this device  
	      unit_t c_loc = {x,y,z}; 
	      PDeviceId cId = _locToId[c_loc];

	      // calculate the neighbour positions
	      // (taking into account the periodic boundary)
              int x_neg, y_neg, z_neg;
              int x_pos, y_pos, z_pos;

              // assign the x offsets
              if(x==0) {
                x_neg = _D-1;
                x_pos = x+1;
              } else if (x == (_D-1)) {
                x_neg = x-1;
                x_pos = 0;
              } else {
                x_neg = x-1;
                x_pos = x+1;
              }

              // assign the y offsets
              if(y==0) {
                y_neg = _D-1;
                y_pos = y+1;
              } else if (y == (_D-1)) {
                y_neg = y-1;
                y_pos = 0;
              } else {
                y_neg = y-1;
                y_pos = y+1;
              }

              // assign the z offsets
              if(z==0) {
                z_neg = _D-1;
                z_pos = z+1;
              } else if (z == (_D-1)) {
                z_neg = z-1;
                z_pos = 0;
              } else {
                z_neg = z-1;
                z_pos = z+1;
              }

	      unit_t n_loc;
	      PDeviceId nId;
                 // z = -1
                   // { -1,-1,-1 },  { -1,0,-1 },  { -1, +1,-1 }
                      n_loc.x = x_neg; n_loc.y = y_neg; n_loc.z = z_neg;
                      nId = _locToId[n_loc];
		      addNeighbour(cId, nId);

                      n_loc.x = x_neg; n_loc.y = y; n_loc.z = z_neg;
                      nId = _locToId[n_loc];
		      addNeighbour(cId, nId);

                      n_loc.x = x_neg; n_loc.y = y_pos; n_loc.z = z_neg;
                      nId = _locToId[n_loc];
		      addNeighbour(cId, nId);

                   // { 0,-1, -1 },  { 0, 0,-1 },  { 0, +1, -1 }
                      n_loc.x = x; n_loc.y = y_neg; n_loc.z = z_neg;
                      nId = _locToId[n_loc];
		      addNeighbour(cId, nId);

                      n_loc.x = x; n_loc.y = y; n_loc.z = z_neg;
                      nId = _locToId[n_loc];
		      addNeighbour(cId, nId);

                      n_loc.x = x; n_loc.y = y_pos; n_loc.z = z_neg;
                      nId = _locToId[n_loc];
		      addNeighbour(cId, nId);

                   // { +1,-1,-1 },  { +1,0,-1 },  { +1, +1,-1 }
                      n_loc.x = x_pos; n_loc.y = y_neg; n_loc.z = z_neg;
                      nId = _locToId[n_loc];
		      addNeighbour(cId, nId);

                      n_loc.x = x_pos; n_loc.y = y; n_loc.z = z_neg;
                      nId = _locToId[n_loc];
		      addNeighbour(cId, nId);

                      n_loc.x = x_pos; n_loc.y = y_pos; n_loc.z = z_neg;
                      nId = _locToId[n_loc];
		      addNeighbour(cId, nId);

                 // z = 0
                   // { -1,-1,0 },  { -1,0,0 },  { -1, +1,0 }
                      n_loc.x = x_neg; n_loc.y = y_neg; n_loc.z = z;
                      nId = _locToId[n_loc];
		      addNeighbour(cId, nId);

                      n_loc.x = x_neg; n_loc.y = y; n_loc.z = z;
                      nId = _locToId[n_loc];
		      addNeighbour(cId, nId);

                      n_loc.x = x_neg; n_loc.y = y_pos; n_loc.z = z;
                      nId = _locToId[n_loc];
		      addNeighbour(cId, nId);

                   // { 0,-1, 0 },  { 0, 0, 0 },  { 0, +1, 0 }
                      n_loc.x = x; n_loc.y = y_neg; n_loc.z = z;
                      nId = _locToId[n_loc];
		      addNeighbour(cId, nId);

                      // skipping! one is not a neighbour of oneself
                      //n_loc.x = x; n_loc.y = y; n_loc.z = z;

                      n_loc.x = x; n_loc.y = y_pos; n_loc.z = z;
                      nId = _locToId[n_loc];
		      addNeighbour(cId, nId);

                   // { +1,-1, 0 },  { +1,0, 0 },  { +1, +1, 0 }
                      n_loc.x = x_pos; n_loc.y = y_neg; n_loc.z = z;
                      nId = _locToId[n_loc];
		      addNeighbour(cId, nId);

                      n_loc.x = x_pos; n_loc.y = y; n_loc.z = z;
                      nId = _locToId[n_loc];
		      addNeighbour(cId, nId);

                      n_loc.x = x_pos; n_loc.y = y_pos; n_loc.z = z;
                      nId = _locToId[n_loc];
		      addNeighbour(cId, nId);
		      
                 // z = +1
                   // { -1,-1,+1 },  { -1,0,+1},  { -1, +1,+1 }
                      n_loc.x = x_neg; n_loc.y = y_neg; n_loc.z = z_pos;
                      nId = _locToId[n_loc];
		      addNeighbour(cId, nId);

                      n_loc.x = x_neg; n_loc.y = y; n_loc.z = z_pos;
                      nId = _locToId[n_loc];
		      addNeighbour(cId, nId);

                      n_loc.x = x_neg; n_loc.y = y_pos; n_loc.z = z_pos;
                      nId = _locToId[n_loc];
		      addNeighbour(cId, nId);

                   // { 0,-1, +1 },  { 0, 0, +1 },  { 0, +1, +1 }
                      n_loc.x = x; n_loc.y = y_neg; n_loc.z = z_pos;
                      nId = _locToId[n_loc];
		      addNeighbour(cId, nId);

                      n_loc.x = x; n_loc.y = y; n_loc.z = z_pos;
                      nId = _locToId[n_loc];
		      addNeighbour(cId, nId);

                      n_loc.x = x; n_loc.y = y_pos; n_loc.z = z_pos;
                      nId = _locToId[n_loc];
		      addNeighbour(cId, nId);

                   // { +1,-1, +1 },  { +1,0, +1 },  { +1, +1, +1 }
                      n_loc.x = x_pos; n_loc.y = y_neg; n_loc.z = z_pos;
                      nId = _locToId[n_loc];
		      addNeighbour(cId, nId);

                      n_loc.x = x_pos; n_loc.y = y; n_loc.z = z_pos;
                      nId = _locToId[n_loc];
		      addNeighbour(cId, nId);

                      n_loc.x = x_pos; n_loc.y = y_pos; n_loc.z = z_pos;
                      nId = _locToId[n_loc];
		      addNeighbour(cId, nId);

          }
        }
     }
     // all the edges have been connected

    _g->map(); // map the graph into hardware calling the POLite placer

    // initialise all the devices with their position
    for(std::map<PDeviceId, unit_t>::iterator i = _idToLoc.begin(); i!=_idToLoc.end(); ++i) {
      PDeviceId cId = i->first;
      unit_t loc = i->second;
      _g->devices[cId]->state.loc.x = loc.x;
      _g->devices[cId]->state.loc.y = loc.y;
      _g->devices[cId]->state.loc.z = loc.z;
      _g->devices[cId]->state.unit_size = _unit_size;
    } 

}

// deconstructor
template<class S>
Universe<S>::~Universe(){
   delete _g;
}

// add a bead to the simulation universe
template<class S>
void Universe<S>::add(const bead_t *in) {
   bead_t b = *in;
   unit_pos_t x = floor(b.pos.x()/_unit_size);
   unit_pos_t y = floor(b.pos.y()/_unit_size);
   unit_pos_t z = floor(b.pos.z()/_unit_size);
   unit_t t = {x,y,z};

   // lookup the device
   PDeviceId b_su = _locToId[t];

   // check to make sure there is still enough room in the device
   if(_g->devices[b_su]->state.num_beads > max_beads_per_dev) {
      printf("Error: there is not enough space in device:%d for bead:%d\n", b_su, in->id);
      fflush(stdout);
      exit(EXIT_FAILURE);
   } else {
     // we can add the bead

     // make the postion of the bead relative
     b.pos.x(b.pos.x() - (S(float(t.x))*_unit_size)); 
     b.pos.y(b.pos.y() - (S(float(t.y))*_unit_size)); 
     b.pos.z(b.pos.z() - (S(float(t.z))*_unit_size)); 
     
     _g->devices[b_su]->state.beads[_g->devices[b_su]->state.num_beads++] = b; 
   }
}

// writes the universe into the POETS system
template<class S>
void Universe<S>::write() {
    _g->write(_hostLink);
}

// starts the simulation
template<class S>
void Universe<S>::run() {
    _hostLink->boot("code.v", "data.v");
    gettimeofday(&_start, NULL);
    _hostLink->go();

    // enter the main loop
    while(1) {
        PMessage<None, DPDMessage> msg;
        for(uint32_t i=0; i< _g->numDevices; i++) {
           _hostLink->recvMsg(&msg, sizeof(msg));
	   if(msg.payload.debug > 0.1)
	       printf("<%d,%d,%d> has dist_recorded %.4f local bead:<%.4f,%.4f,%.4f>\n", msg.payload.from.x, msg.payload.from.y, msg.payload.from.z, msg.payload.debug, msg.payload.beads[0].pos.x(), msg.payload.beads[0].pos.y(), msg.payload.beads[0].pos.z());
	}
	break; // exit the main loop
    }
    // get end time
    gettimeofday(&_finish, NULL);

    // Display time
    timersub(&_finish, &_start, &_diff);
    double duration = (double) _diff.tv_sec + (double) _diff.tv_usec / 1000000.0;
    printf("Time = %lf\n", duration);
}

#endif /* __UNIVERSE_IMPL */
