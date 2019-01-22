# dpd-baremetal
_A tinsel-based (POLite) version of the DPD application_

Based around the equations set out in the DL_MESO manual [[1](http://www.cse.scitech.ac.uk/ccg/software/DL_MESO/MANUAL/USRMAN.pdf)].

| real-time example | slowed down |
| ----------------- | ----------- |
| ![example](gifs/dpd-baremetal-oil-and-water-realtime.gif) | ![example](gifs/dpd-baremetal-oil-and-water-slowed-start.gif) |
| ![example](gifs/dpd-baremetal-oil-and-water-3D-realtime.gif) | ![example](gifs/dpd-baremetal-oil-and-water-3D-slowed-down.gif) |

An oil-and-water demo (ran on Byron: 2D = 100 threads, 3D = 1000 threads -- any larger and the current visualiser breaks..).

__current performance__  = 1.5 Million Bead Steps/second (MPS/second), this is about the same range as a single GPU implementation in the literature [[2](https://www.sciencedirect.com/science/article/pii/S0010465514002203)]. However, it should be noted that this is our first-cut at a hardware implementation for this application and there is likely plenty of room for improvement.

[1] http://www.cse.scitech.ac.uk/ccg/software/DL_MESO/MANUAL/USRMAN.pdf <br />
[2] https://www.sciencedirect.com/science/article/pii/S0010465514002203 

--------------------------------------------------------------

__dpd-baremetal__ consists of two main parts:
* _[the application]_ the simulation application that is running on the _remote_ POETS box.
* _[the client]_ a _local_ web-based GUI that displays the real-time output from the application and allows simulation playback and storage.

Both of these two separate parts communicate over ssh, in a similar fashion to [ImPOLite](https://github.com/POETSII/ImPOLite) and [poets-ecosystem](https://github.com/POETSII/poets-ecosystem).

--------------------------------------------------------------

## setup

#### install requirments

Both client and application
* `libboost`
* `socat`

--------------------------------------------------------------

#### building

To build on the local client-side:
```bash
 make bridge 
```

To build on the remote application side (POETS box):
```bash
make
```

To connect the client and the application the user must have ssh key-based authetification access to the POETS box where the application is running and must edit the following lines of `dpd-baremetal/Makefile`:

```bash
  # ~~~~~~~~~~~~~~~ Client side setup ~~~~~~~~~~~~~~~~~~~~~~~~~
  LOCAL_SOCKET=./_external.sock
  REMOTE_FULL=sf306@byron.cl.cam.ac.uk
  REMOTE_SOCKET=/home/sf306/dpd-baremetal/bin/_external.sock
```
Where:
* `LOCAL_SOCKET` is the name of the socket for the socket on the client side that is connected to the remote socket using `socat`.
* `REMOTE_FULL` is the username of the user and the address of the POETS box where the application will be executed.
* `REMOTE_SOCKET` is the name of the socket on the POETS box that is created by the application


## running the default example

The default application is a 2D oil-and-water example (gif at the top of this page). To run this application code needs to be executed on both the client-side and the application side in a specific order -- as there is not currently startup synchronisation like there is in `ImPOLite` or `poets-ecosystem` ... yet. 

#### step 1: _launch the remote application_
On the POETS box you can start the application with the following:
```bash
cd bin; ./run
```

#### step 2: _launch the local client_
Before `running...` appears on the POETS box application. The web interface needs to be lauched on the client-side by typing:
```bash
make client_run
```

#### step 3: _open the web-interface_

Once the application is running the web-interface can be launched from the client machine by opening `http://localhost:3000`. From here the user can watch a live output of the simulation and play the simulation back from the start at a faster framerate. 

## implmentation details

Currently this application is built using POLite and makes extensive use of the newer hardware and POLite features, in particular the idle-detection feature from [tinsel-0.5](https://github.com/POETSII/tinsel/releases/tag/v0.5). 

The simulation universe is a cube that is decomposed into many smaller sub-cubes represented by devices. Each sub-cube is connected in a toroidal fashion to it's neighbours via edges (i.e. the edges of the cube wraparound, making the boundaries periodic). 

The simulation has three modes of operation which is switches between throughout the simulation:

* __[UPDATE]__ Every sub-cube sends its entire state -- bead position and velocity -- to all of its neighbours. Each message contains the details for a single bead, meaning that multiple messages may need to be sent to each neighbour. As it receives beads it performs force update calculations for all the beads it owns. Once the system has gone idle we know that every sub-cube has shared it state and we can calculate the inter-bead interactions for each sub-cube.

* __[MIGRATION]__ In the idle handler after an update we have the new force acting on each bead and we can perform a velocity verlet integration to calculate the new position of each bead. Any bead that leaves the current sub-cube is sent as a message to its intended destination and removed from the current cube. When a sub-cube receives a message in the __MIGRATION__ stage it knows that it should add this bead to its state. 

* __[EMIT]__ After `emitperiod` __MIGRATION__ stages there is an emit phase where the position and velocity of every bead is sent to the remote web-based client. (In the current implementation for real-time output it is important that this is not set too low otherwise the web-interface gets overwhelmed, for small sized problems of <20K beads a value of between 25 - 50 works well.) 


