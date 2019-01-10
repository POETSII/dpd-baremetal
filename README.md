# dpd-baremetal
_A tinsel-based version of the DPD application_

| real-time example | slowed down |
| ----------------- | ----------- |
| ![example](gifs/dpd-baremetal-oil-and-water-realtime.gif) | ![example](gifs/dpd-baremetal-oil-and-water-slowed-start.gif) |

An oil-and-water demo (ran on Byron with 1000 threads).
 
__dpd-baremetal__ consists of two main parts:
* _[the application]_ the simulation application that is running on the _remote_ POETS box.
* _[the client]_ a _local_ web-based GUI that displays the real-time output from the application and allows simulation playback and storage.

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
On the POETS box you can start the application with the following:
```bash
cd bin; ./run
```

You then need to start the client side before `running...` appears on the POETS box application. This can be done by typing the following on the client machine:
```bash
make client_run
```

Once the application is running the web-interface can be launched from the client machine by opening `http://localhost:3000`. From here the user can watch a live output of the simulation and play the simulation back from the start at a faster framerate. 

## implmentation details

C urrently this application is mainly built around POLite -- although the ultimate goal is to modify POLite to create a more application specific code base. The current implementation also makes use of the latest Tinsel idle-detection features to make globally synchronus transition changes between the update, migration, and emit phases of the computation. The plan is ultimately to play with the synchronisation and see how it effect performance.
