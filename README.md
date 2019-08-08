# dpd-baremetal
_A tinsel-based (POLite) version of the DPD application_

Based around the equations set out in the DL_MESO manual [[1](http://www.cse.scitech.ac.uk/ccg/software/DL_MESO/MANUAL/USRMAN.pdf)].

| real-time example | slowed down |
| ----------------- | ----------- |
| ![example](gifs/dpd-baremetal-oil-and-water-realtime.gif) | ![example](gifs/dpd-baremetal-oil-and-water-slowed-start.gif) |
| ![example](gifs/dpd-baremetal-oil-and-water-3D-realtime.gif) | ![example](gifs/dpd-baremetal-oil-and-water-3D-slowed-down.gif) |

An oil-and-water demo (ran on Byron: 2D = 100 threads, 3D = 1000 threads -- any larger and the current visualiser breaks..).

[1] http://www.cse.scitech.ac.uk/ccg/software/DL_MESO/MANUAL/USRMAN.pdf <br />
[2] https://www.sciencedirect.com/science/article/pii/S0010465514002203

--------------------------------------------------------------

__dpd-baremetal__ consists of two main parts:
* _[the application]_ the simulation application that is running on the _remote_ POETS box.
* _[the client]_ a _local_ web-based GUI that displays the real-time output from the application and allows simulation playback and storage.

Both of these two separate parts communicate over ssh, in a similar fashion to [ImPOLite](https://github.com/POETSII/ImPOLite) and [poets-ecosystem](https://github.com/POETSII/poets-ecosystem).

--------------------------------------------------------------

## implmentation details

Currently this application is built using POLite and makes extensive use of the newer hardware and POLite features, in particular the idle-detection feature from [tinsel-0.5](https://github.com/POETSII/tinsel/releases/tag/v0.5).

The simulation universe is a cube that is decomposed into many smaller sub-cubes represented by devices. Each sub-cube is connected in a toroidal fashion to it's neighbours via edges (i.e. the edges of the cube wraparound, making the boundaries periodic).

The simulation has three modes of operation which is switches between throughout the simulation:

* __[UPDATE]__ Every sub-cube sends its entire state -- bead position and velocity -- to all of its neighbours. Each message contains the details for a single bead, meaning that multiple messages may need to be sent to each neighbour. As it receives beads it performs force update calculations for all the beads it owns. Once the system has gone idle we know that every sub-cube has shared it state and we can calculate the inter-bead interactions for each sub-cube.

* __[MIGRATION]__ In the idle handler after an update we have the new force acting on each bead and we can perform a velocity verlet integration to calculate the new position of each bead. Any bead that leaves the current sub-cube is sent as a message to its intended destination and removed from the current cube. When a sub-cube receives a message in the __MIGRATION__ stage it knows that it should add this bead to its state.

* __[EMIT]__ After `emitperiod` __MIGRATION__ stages there is an emit phase where the position and velocity of every bead is sent to the remote web-based client. (In the current implementation for real-time output it is important that this is not set too low otherwise the web-interface gets overwhelmed, for small sized problems of <20K beads a value of between 25 - 50 works well.)

## Setup

#### Install requirments

Both client and application
* `libboost`
* `socat`

--------------------------------------------------------------

#### Building for visual simulation

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

#### Step 1: _launch the remote application_
On the POETS box you can start the application with the following:
```bash
cd bin; ./run
```

#### Step 2: _launch the local client_
Before `running...` appears on the POETS box application. The web interface needs to be lauched on the client-side by typing:
```bash
make client_run
```

#### Step 3: _open the web-interface_

Once the application is running the web-interface can be launched from the client machine by opening `http://localhost:3000`. From here the user can watch a live output of the simulation and play the simulation back from the start at a faster framerate.

## Other make options

The Makefile has several other options for running the DPD application, which
can provide interesting information for how the DPD application performs.

### `make test`

This option builds a test to run a DPD simulation in an 18x18x18 universe, from
a set of beads which have been pre-set (stored in `tests/beads_in_18.csv`).
This simulation runs for 1000 timesteps, and outputs the state of all beads at
the end of this run. The positions of these beads are then checked against the
expected output from the pre-set input beads (stored in
`tests/beads_out_18.csv`). If these match, the application works as expected.
Otherwise, there is a problem in the application, meaning the calculations are
not performing as expected, or potentially, beads are being lost.

#### Execution:

```
    make test
    cd bin
    ./test
```

The test will print out the positions of each bead, comparing them to the
expected output, and finally print an overall `PASS` or `FAIL` to indicate the
success of the test.

### `make test-bonds`

This option builds a test to run a DPD simulation in a 25x25x25 universe, from
a set of beads which have been pre-set, and includes some polymer chains; beads
which are bonded (stored in `tests/beads_bonds_in_25.csv`).
This simulation runs for 1000 timesteps, and outputs the state of all beads at
the end of this run. The positions of these beads are then checked against the
expected output from the pre-set input beads (stored in
`tests/beads_bonds_out_25.csv`). If these match, the application works as
expected. Otherwise, there is a problem in the application, meaning the calculations are
not performing as expected, or potentially, beads are being lost.

#### Execution:

```
    make test
    cd bin
    ./test
```

The test will print out the positions of each bead, comparing them to the
expected output, and finally print an overall `PASS` or `FAIL` to indicate the
success of the test.

### `make timed-run`

This option sets a simulation up to accurately time the execution time of a
given simulation size from timestep 0 to timestep 999, 1000 timesteps total.
The number of beads is fixed at 4 beads per cell.

#### Execution:

```
    make timed-run
    cd bin
    ./run n
```

Where n is any number greater than or equal to 3. This will run a simulation
with a volume of `n x n x n`. Once complete, the simulation will output a time
for the 1000 timestep simulation in seconds.

### `make stats-run`

This make option will build a form of the application which includes some
performance counters. This is useful for providing information on the numbers
of messages sent/received in the simulation, the cache hit rate and the
utilisation of each CPU.

#### Execution:

```
    make stats-run
    cd bin
    ./run n
```

Replace `n` by any number for a simulation volume of size `n x n x n`. The number
of beads per cell is fixed at 4.

This simulation will the run for 1000 timesteps, at which point it will
terminate. To view the performance information, while in the directory
`bin` run:

```
    make print-stats
```

### Run options

There are some command line options for run when generated by some `make`
options. These give options for various modes of running the simulation.

`./run <Simulation length> [--bonds] [--print-number-of-beads] [--help]`

`Simulation length` - The only required argument is the simulation length, which
must be an integer greater than or equal to three. This is the length of one
side of the volume, which is a cube.

`--bonds` - Optional. When generating the simulation volume, and inserting the
beads within the volume, should this include some polymer chains, bonding some
beads.

`--print-number-of-beads` - Optional testing option. When printing the run time
of the simulation to the results file, `timing-results.csv`, should the number
of beads which were in the simulation be printed first. This was added purely
for automatic performance testing purposes.

`--help` - Print a help text, explaining the above.
