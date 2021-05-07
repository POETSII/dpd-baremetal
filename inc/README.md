# Inc

This directory contains header files for the DPD application. Some are in a
class structure, others are standalone. This will describe the class structure
and the purpose of each file.

## DPD header files

Several of the files in this directory are used to implement the operations
necessary for the simulations to run on Tinsel cores. Some are used to store
constants which are only changed base on the example simulation running, some
are for shared code to make development easier. We will briefly discuss their
purpose here. These are all implemented in these files, there is none in `src/`.

### DPDConstants.hpp

Various examples of DPD simulations use different constants based on what each
bead type represents. Oil and water features three bead types which repel each
other in different strengths, Vesicle self-assembly has 4 types which interact
with different strengths to those types of the oil and water simulation.

This houses the constants, some of which are common to all examples, and some of
which are different for each example. These are stored here for easy reference
and simplicity in changing or adding to them.

### DPDStructs.hpp

`DPDState` and `DPDMessage` are structs which are different and/or removed
depending on the simulator being used and the macros which were used when
compiling it. We store these in a separate file to keep the code in the
dpd implementation files clear, as many of the functions are similar or the
same regardless of the simulator, macro or example.

Each simulator has its own struct with some similarities, but for clarity they
have been defined separately (separated by macro flags) so if working
specifically with one of these, it's easier to decipher the variables in the
struct for that type.

### dpd.hpp

This is the most important file as it is where the key DPD operations are
performed.

The synchronous, GALS and serial simulators all aim to produce the same or
similar results. This is achieved by them performing the same calculations.
Rather than these be in the separate simulator implementation files, we define
the key operations as functions in `dpd.hpp`. The individual simulators all
import this file and can use any of the functions in the same way.

This helps with debugging when there is an issue with the calculation of forces
or migrations, as it is likely if one simulator is showing an issue, this will
affect another simulator too.

### Vector3D.hpp

This provides a Vector class which can be used to store 3 dimensional
directional data, such as position, velocity or force. This is used heavily as
DPD is simulated in a 3D volume. It provides operators for addition, subtraction
etc and some functions such as finding the magnitude of a vector and calculating
the dot-product.

### utils.hpp

RISC-V cores, such as those that POETS threads run on, does not have a
native square root function, a vital function in DPD. We therefore had to
implement one ourselves.

This is a simple file providing the prototypes functions of some square root
functions. `inv_sqrt()` is not used in POETS DPD, instead we use `newt_sqrt()`
which provides a resulting square root which we can balance between being more
accurate at a loss of performance, or perform better for less accurate square
roots.

## POLite files

These inherit from `POLite/PDevice`, which provides an API for using Tinsel
cores/threads, namely sending and receiving messages and handling an idle event.
Each is used to implement one of the POLite simulators.

### sync.h

This provides the synchronous simulator, all implemented in this header file. It
inherits the functions of `init()`, `send()`, `recv()`, `step()` and `finish()`.
These are used to handle the events that can occur in a POETS application,
sending or receiving a message, or handling when the system becomes idle or the
application terminates.

Send is used to share beads with neighbours in the update phase, and migrate
beads to neighbours in the migrate phase.

Recv is used to handle beads which neighbours have sent, to calculate the forces
acting on this cells beads if an update message, or to add a bead to its state
if its a migrating bead.

Step runs when the hardware detects that all threads are idle. At this point,
cells will calculate the change in velocity and position for the next timestep,
and determine if any beads need to migrate.

Finish is called when the application wishes to terminate, such as when the
given number of timesteps is reached.

### gals.h

This is the Globally Aysynchronous Locally Synchronous (GALS) implementation of
the DPD simulator. It does not use `step()` or `finish()`, instead it uses `send
()` and `recv()` more fully. Messages are sent encoded with information for
neighbours to use, which helps them determine if they are synchronised with
their neighbours or not.

Send is still used to send beads in the update and migrate phases, but is also
used to send "update complete" and "migrate complete" messages.

Receiving a bead is handled the same as in `sync.h` but receving a
"phase" complete message is used to update counters used to determine if a
neighbour has moved onto the next phase.

More details about the operation of the GALS simulator is provided in
[docs/gals-simulator.md](docs/gals-simulator.md).

## Simulator/Volume/Cells class structure

A class structure exists to reduce duplicate code. Many functions that occur for
different simulators or features can be shared between them, with some
inheritance to provide a method of accessing different data structures that are
used for different simulators or features.

The classes can be helpful for viewing a sort of "api" for how to use the
different classes. The implementation files in `src/` are obviously more clear
on the specific way functions work, but for simple how-to-use information, these
can provide good detail.

### Cells.hpp

Cells is a class which provides a wrapper for the set of cells which make
up a volume. The base class, `Cells<C>` has a template, `C` which is the
type of the set of cells.

The fields contains some details about the cells, such as the number in the
length of the volume, and maps for accessing the cells with either their device
ID or their location within the volume. It also contains a pointer the cell set
`C`.

Its member functions provides access to the fields, and virtual functions which
provide the access to the individual cells, for manipulating their state. These
virtual functions are implemented by inheriting classes in order to provide
access via the method suited by the template class `C`.

This class simply holds the cells and provides access to their state for
placing beads and setting variables, it has no information about how the cells
are set up and used during execution.

#### RDFCells.hpp

Radial Distribution Function (RDF) is a function which measures the probability
of particles in a volume being at certain distances from one another. This can
be used to detect patterns, such as structures forming, or separations of
particle types.

`RDFCells` inherits from `Cells<C>`, using a `std::vector<DPDState>` as the set
which holds the cells. RDF is calculated on a per-timestep basis on an x86
machine, and when comparing bead distances in cells, it simply reads from both
cell's state, via the std::vector. It does not need to send messages, and thus
does not need to "connect" the cells to their neighbours.

It overrides virtual functions from the parent `Cells<C>`, providing access to
the states of individual cells and beads.

#### SimulationCells.hpp

Cells may be needed simply to house a fixed set of beads, and `Cells<C>`
provides the template for this purpose. However, cells when used in an
execution, such as a simulator, need to be prepared differently, so that they
can communicate and update their state regularly.

`SimulationCells<C>` inherits from `Cells<C>` and provides the necessary
functions for cells that are to be part of a simulation. These cells need to be
connected to neighbours, and provides a function to do so, something not
necessary for base `Cells<C>` as these are to be used simply to place bead.
Neighbours are needed in order to share and migrate beads with the relevant
neighbouring cells.

A virtual function `addNeighbour(PDeviceId a, PDeviceId b)` is provided to be
implemented by an inheriting class, so that it can connect two cells in the
method appropriate to the template Cell set `C`.

It also provides functions to initialise cells before the simulation is run,
and functions to set the maximum and minimum timestep in a cell, somthing only
needed by a simulation so it knows how many timesteps to run for.

#### POLiteCells.hpp

`POLiteCells` inherits from `SimulationCells<C>`, using a pointer to a
`PGraph<DPDDevice, DPDState, None, DPDMessage>` instead of the template `C`.
POLite uses `PGraph` to generate and map devices, and we can use a pointer to
this data structure to manipulate the state of cells, for example when adding
beads.

The member functions override all virtual classes from the parent classes,
`SimulationCells<C>` and `Cells<C>`, providing the access to the `PGraph`
structure holding the cells before they are written to the hardware for
simulation.

#### SerialCells.hpp

`SerialCells` also inherits from `SimulationCells<C>`, using a pointer to
`std::vector<DPDState>`, the set housing the cells. Serial simulations are naive
and do not use the POETS hardware or messaging of any form, and thus do not need
mapping for efficient communications.

Unlike the POLite simulators, the serial simulators can have global state, such
as the current and maximum timesteps, and the overridden functions which in
POLite would be stored in each cell, instead store this data in the private
fields.

New member functions are added for easy access to the cell states by the serial
simulator, and virtual functions from parent classes are overridden to allow
manipulation of the cells in a `std::vector`.

#### XMLCells.hpp

`XMLCells` also a pointer to `std::vector<DPDState>` for its cell set, however
the `DPDState` struct is different, holding more variables for use in a
simulation run by the Orchestrator from an XML graph instance, a structure to
house the application, devices and connectivity for the Orchestrator to parse
and generate objects to be run on POETS hardware.

This provides similar functions for access to the `std::vector` as SerialCells,
as well as overrides the functions for connecting and initialising cells in the
method necessary for the resulting simulator.

Member functions are also provided which will return data from cell states used
to generate an XML for eventual ingestion by the Orchestrator.

### Volume.hpp

The volume class is designed to hold the cells, and hold global information
about the volume, such as its size and the number of beads. `Volume<C>` uses the
`C` template as the type of set that the cells are held in. The Cells class
structure provides the necessary access to the state of the cells, depending on
the type of `C`.

The base volume class provides the member functions to add beads, either from a
global position, where it finds the relevant cell and adds it to its state, or
to a specific cell.

#### RDFVolume.hpp

`RDFVolume` inherits from `Volume<C>`, providing `std::vector<DPDState>` as the
set housing the cells. It is simply a wrapper class to provide this template for
`Volume<C>`. When constructed, its primary function is to generate an instance
of `RDFCells`.

The member functions for `Volume<C>` are all implemented to add beads to the
volume, and `RDFCells` inherits these. The functions provided by the instance of
`RDFCells` allow `Volume<C>` to place beads in the correct cells state.

#### SimulationVolume.hpp

Inheriting from `Volume<C>`, this class `SimulationVolume<C>` continues the use
of a template `C` so that depending on the simulator in use, it can use a
different set to store the cells. This uses the implementations of the member
functions of `Volume<C>`, but provides some new ones to guard against issues
that can affect a simulator.

The POETS hardware has a fixed memory capacity, and every device which runs on a
POETS thread has limited state space therefore. We have to limit the state
space, and we can do this by limiting the number of beads a cell can hold at
any one time.

Beads are initially placed randomly for a simulation, but this can lead to a
scenario where cells have more than they are capable of storing.
To guard against this, `SimulationVolume<C>` provides functions which check if a
bead is capable of being added to a cell before it is added.

It also provides a function
`float find_nearest_bead_distance(const bead_t *i, cell_t u_i)` which limits the
minimum distance between beads when placed randomly. Two beads placed initialy
very close can have massive forces acting on them, which can lead to beads being
lost. Limiting the initial distance between beads helps to reduce the early
forces, and stop beads being lost.

#### POLiteVolume.hpp

`POLiteVolume` serves to inherits `SimulationVolume<C>` and provide
`PGraph<DPDDevice, DPDState, None, DPDMessage>` as the set holding the cells.
When constructed, it constructs this `PGraph` and the `POLiteCells`, and reuses
the functions of the parent class. It can also take in variables which give the
dimensions of the POETS cluster to use, allowing the choice at runtime of how
many boxes the simulation should use.

The parent class has all the necessary functions for handling the volume, and
`POLiteCells` provides the functions for manipulating cell states stored in the
`PGraph`.

#### SerialVolume.hpp

Similar to `POLiteVolume`, this mainly serves to construct a cell type, this
time `SerialCells`, replacing the template `C` with `std::vector<DPDState`.
This class provides the functions for adding beads to the cells, and
`SimulationVolume` handles the checking of space for bead and adding them to the
volume.

#### XMLVolume.hpp

The Orchestrator uses the same programming model as POLite, but provides a wider
feature range. However, it also uses the same hardware and thus is subject to
the same limits on state space. `XMLVolume` inherits from `SimulationVolume<C>`
to use the functions to check for space before adding a bead. The cell type
for this is `XMLCells`, which uses for the template class, `C`,
`std::vector<DPDState>`.

### HostMessenger.hpp

Whether using a POLite simulator (GALS or synchronous), or the serial simulator,
there is data that needs to come from the simulator, either in the form of beads
to be stored for visualisation or analysis, or indicating that a timed
simulation has ended.

`HostMessenger<Q>` provides the functions to handle the messages from the
simulators for whatever their use may be. How these messages get from the
simulator to the host is determined by the simulator, so the template `Q` is
used to indicate that some form of blocking *queue* is necessary for this to
await messages and handle them as they are received.

### POLiteMessenger.hpp

Inheriting from `HostMessenger<Q>`, using `HostLink` as the queue to receive
messages. It provides a function which blocks while awaiting a messages from
the POETS hardware via `HostLink`. `HostMessenger` does not care where the
message comes from, only how it handles it.

### SerialMessenger.hpp

This inherits from `HostMessenger<Q>`, using a blocking queue (see
[inc/concurrentqueue](inc/concurrentqueue)) as the channel to receive messages
from the serial simulator.

When the serial simulator starts, it spawns a `SerialMessenger` thread which
runs concurrently, handling messages generated by the `SerialSimulator` queue.
`SerialMessenger` provides functions that `HostMessenger<Q>` uses to receive a
message, blocking until it does.

### Executor.hpp

The `Executor<V>` has the volume template `V`, and is designed for a hierarchy
of classes which *execute* something on/with a volume, such as simulat**or**s
and generat**or**s. This provides access to the volume it holds, and virtual
functions `run()` and `test()`, which perform the operations on the volume.

#### RDFCalculator.hpp

This class inherits from `Executor<V>`, with the volume being an `RDFVolume`.
This is the function that actively calculates the RDF values. The volume will
hold the state at a given timestep, and the `RDFCalculator` will iterate through
the beads in this volume, determining the RDF value for beads of different
combinations of types.

It is designed to be used with [src/RDF.cpp](src/RDF.cpp), which takes a set of
states, generates the volumes and spawns `RDFCalculator` threads to calculate
the RDF, running as many as it can in parallel. The results are stored and can
be used to generate graphs.

#### Simulator.hpp

`Simulator<V>` is used to provide the simulators (Sync, GALS which are POLite
simulators and serial) with common fields. This continues to use the template
`V` as the different simulators use different volume types. It does not
implement `run()` or `test()` as these are specific to the simulator being used.

#### POLiteSimulator.hpp

This uses `POLiteVolume`, and (in the corresponding cpp file in `src/`)
implements `run()` and `test()` from `Executor<V>`. With a constructed volume,
this writes the cells to the hardware memory, and then starts the simulation. It
then uses `POLiteMessenger` which is built in the constructor, to handle
messages which come from the hardware at run time.

#### SerialSimulator.hpp

`SerialSimulator` holds a `SerialVolume` and executes the serial simulation on
the cells in this volume. `run()` and `test()` will serially run the simulation
and pass messages to the relevant place, `run()` to a separate thread running
with `SerialMessenger` handling the messages, `test()` spawns another thread to
run the simulation while awaiting messages to store and return for comparison to
expected output.

#### XMLGenerator.hpp

This inherits from `Simulator<V>` as it used used to generate an XML file
containing the application graph of DPD to be simulated by the Orchestrator on
POETS hardware. The volume and the cells prepare the state of this simulation,
and `run()` in this class simply takes this and generates an XML.

It uses one of the graph types in `xml-graph-types`, and then writes the device
instances and edge instances.

### BeadMap.hpp

This holds several functions used for operating on bitmaps. Bitmaps are used
heavily to represent which of an array of beads is occupied with a bead. These
will find a free slot in a bitmap, set or clear a slot in a bitmap and so on.

## concurrentqueue/

For the serial simulator and RDF calculate, it was necessary to have some
communication channel available which could be shared by two threads
concurrently. Rather than spend time implementing one, I found one on github
and decided to integrate it so I could get back on with working on DPD.

## Externals

Early on in DPDs development, we had a set up where a simulation could emit its
bead states to the host, which would then send them to a client PC elsewhere,
which we called **externals**, which could render them into a visualisation.

This hasn't been used for a while, as
[VMD](https://www.ks.uiuc.edu/Research/vmd/) produces much nicer visualisations
and videos, however this legacy code has been kept in case we need to use it
for future real-time visualisations, perhaps using virual-reality or some
other software.

## externals-common.h

A header file containing the struct of an external message, something similar to
that of a DPDMessage.

## ExternalServer.hpp

The header for a server class which handles the messages being sent from the
POETS machine to the client. The POETS host will pass messages to this, which
will then send them along the predefined channel.

## ExternalClient.hpp

The header for a client class which runs on the client PC, which receives
messages from the POETS machine running the simulation, and can use them to
visualise the simulation or perform real-time analysis.
