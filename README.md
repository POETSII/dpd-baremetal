# dpd-baremetal
_A POLite version of the DPD application_

## Documentation

This README is designed to give an overview of the repo, and get you testing
and running stuff quickly.

More details are provided in the `docs` directory of how the POETS DPD algorithm
actually works, and how macro flags and makefile recipes work to produce
simulations that achieve a result but with differing performance.

Some directories have a README explaining what its containing files do. At the
end of this README, a brief explanation of what each of the directories in the
root contains is provided.

## Setup

#### Install requirments

- `libboost`
- `socat`

#### Cloning

To clone this correctly, use:

```bash
git clone --recursive git@github.com/POETSII/dpd-baremetal
```

To run the simulations on a POETS box, this needs `tinsel` in the submodules
directory.

## Testing

It's always a good idea to run a test suite after first cloning, and then after
making changes during development.

`tests/` has a regression test suite that runs everything. It does take a
while, but it will test all the different macro flags. More information on what
is tested can be found in the README of the `tests/` directory.

If you are on a POETS box, you can run tests on the Tinsel hardware:

```bash
cd tests/
./test-all.sh
```

This will run a script which tests everything. Again, it may take a long time,
and occasionally the boxes will fail a startup self-check. In this case, either
comment-out the completed tests from the test-all script and run again, or run
the whole thing again.

If you are on a non-tinsel x86 machine, you can instead run:

```bash
cd tests/
./test-serial.sh
```

This will test the DPD calculation code and all features that apply to it, but
will not test any synchronisation or messaging based code. Enough to get
started with at least.

If all complete successfully, it will inform you, and you can start running
and developing.

## Running a POETS DPD simulation

To compile, we need to select a **simulator**, an **operation** and an
**example**.

### Simulators

Included in this repo are three simulators:

- A POLite based synchronous simualator - `sync`
- A POLite based GALS simulator - `gals`
- An x86 naive serial simulator - `serial`

The POLite simulators need POETS boxes with Tinsel hardware and have
comparative performance and produce the same results (given the same macros),
and are capable of using up to 8 boxes.

The serial simulator is slower, produced different (but similar) results to the
POLite simulators but allows you to run and test things on any machine, and is
handy for development, allowing you to add traces and use debug tools.

### Operations

Operations include are:

#### `visual`

Store the state of all beads at a given frequency for use in
making videos of the simulation, or analysis. `inc/DPDConstants.hpp` has a
variable `emitperiod` which sets the frequency (in timesteps) to store the
state.

The x86 host machine will store the bead states in `polite-dpd-states/` or
`serial-dpd-states/` depending on the simulator used. These will be in JSON
files, one file per timestep. These files can then be used to generate a PDB
file for use with [VMD](https://www.ks.uiuc.edu/Research/vmd/) which
visualises the data, and can generate videos, or in analysis with other tools.

#### `timed`

Run the simulation for a number of timesteps (provided at runtime)
and record the length of time it takes. **Does not store the state** as this is
slow.

A timed simulation will report the runtime to stdout, and store it in a file
`mega_results.csv`. This can be used for scripting, along with the provided
`mega-tests.sh` file.

#### `stats` **Does not work with serial simulator**

Run the simulation for a given number of timesteps (provided at runtime) and then end, POLite will store
some statistic about number of messages sent/received, CPU usage etc. **Does not
store the state**.

A stats simulation will store the data reported by POLite in `bin/stats.txt`.
This can then be used with `make print-stats` to print the statistics to stdout.

### Examples

The `examples/` directory contains a few files which are used to build a volume,
add beads and run a simulation. More information can be found in the README of
that directory. A summary of the options:

- Oil and Water: `oilwater` - 2 types of oil and water placed in a volume, which
are expected to separate, forming a blob of oil.
- Vesicle self-assembly: `vesicle` - Polymer chains are placed in a volume mostly
full of water, and the polymers should merge and form hollow vesicles.
- Oil and Water with gravity: `gravity` - 2 types of oil and water are placed in
a volume with walls at the top and bottom. Water is subject to gravity, and
the blob of oil initially placed should rise to the top as the water sinks.
- Corner example `corners` - 2 beads are placed in a volume. This aims to show
the workings of the periodic boundary conditions, and one of the beads should
be pushed out of a corner and re-appear back in the opposite corner.

## Compiling

When you have selected a simulator, an operation and an example, we can now
compile the binaries to run this. For this example, let's use
`gals`, `timed` and `vesicle`. To compile this, in the root directory, run:

```bash
make gals-timed-vesicle
```

The binaries should compile, and the executable `run` will be in `bin/`.

The compilation can take macro flags during compilation to use different
available features. Each has their benefits and shortcomings, and more
information on the flags is provided in [docs/macro-flags.md]
(docs/macro-flags.md). However, for each simulator, operation and example
combination, there is a `fastest` and `smallest` makefile recipe.

`fastest` provides the simulator with the best performance.
`smallest` provides the simulator with the smallest total instruction count.

To use these for a given simulation example, simply append `-fastest` or
`-smallest` to the makefile command, e.g.:

```bash
make sync-visual-oilwater-smallest
```

Providing these options allows for some breathing-room when developing. The
`fastest` is the ideal performance we want, but when prototyping new features,
the `smallest` version may allow us to fit everything inside the limited POLite
instruction space. Once the feature is tested and working, then it can be
optimised to fit within the instruction space, and used with `fastest` to
profile and find how this affects the simulator performance.

### DRAM

Large volumes may not fully fit in SRAM. In this case, when running with the
above, the simulation will terminate before running and indicate that SRAM is
full.

In this case, return to the root directory of dpd-baremetal and run the
following:

```bash
make clean
EXTERNAL_FLAGS=-DDRAM make visual-gals-vesicle-fastest
```

This will build the same simulation binaries, but this time direct the POLite
mapper to map the cells to DRAM instead. This will allow much larger
simulations to be run, at a loss of performance (longer runtimes). Running
this is the same as before.

`EXTERNAL_FLAGS` can be applied before any make command to add additional flags
to a makefile recipe.

## Running a simulation

When a simulation has compiled, the `bin/` directory with have a `run`
executable. To run this, `cd` into `bin/` and run:

```bash
./run v
```

Where v is an integer >= 3, the length of one dimension in terms of cells. The
simulator always work on cube volumes.

#### Box arrangement

By default, a POETS DPD simulation will run on 1 box (6 FPGAs, 6144
Tinsel threads). There is capability to run this on up to 8 boxes, in a 2x4
arrangement, depending on which box you run from. See
[poets-cloud](https://github.com/POETSII/poets-cloud) for more details on which
boxes can run what arrangement.

To select box arrangement at runtime, POETS DPD has command line options. Once
the binaries are compiled, run:

```bash
./run v --boxes-x x --boxes-y y
```

`x` is the number of boxes in the x-dimension and `y` is the number of boxes in the y
dimension.

Using more boxes means there are more available threads, so larger simulations
will run faster.

NOTE: These arguments will not work with the serial simulator. That is designed
to run on one thread on one machine.

### Number of timesteps

The `timed` operation reports the runtime for a simulation to complete a given
number of timesteps. By default, this is 10,000 timesteps, however it can be set
for any of the simulators with the `--time t` argument.

Once these have compiled, run the simulation:
```bash
./run v --time t
```

`t` is an integer for the number of timesteps to run.

-------------------------------------------------------------------------------

## Directory information

### docs

Here you will find documents explaining in more detail how DPD and the POETS DPD
algorithm work, how the different versions (sync, GALS and serial) of the
simulator work, and a list of the available macro flags, and details on what
they do.

### examples

This directory contains cpp files for generating a DPD simulation. They take in
arguments for length of one side of the volume and build it, add in the beads
according to percentages of types. The README in this directory has more
information on the available examples.

### gifs

Holds images used to show how the simulator operates in the documentation.

### inc

All header files for the code are stored here. For Volume, Simulator and Cells
it provides a good place to look at the API for using these. It also contains
all the common code for the DPD simulations, how forces and Verlet are
calculated and so on. The code that inherits from POLite to create vertices
(cells) is stored here.

The README in this directory provides more detailed information for each of the
files.

### scripts

Contains some handy python scripts, which convert bead states into something
usable by something else, generate graphs from states or other data, and even a
script that compares two sets of states and finds where they diverge. The README
of this directory provides more information.

### single-poets-thread

A version of the simulator that uses 1 Tinsel thread to run a small simulation.
It was designed as a development tool for testing custom instructions.

### socat

This contains the socat script, used for sending bead data to a client machine
for the purposes of live visualisation. This feature has been deprecated
somewhat, as we now use [VMD](https://www.ks.uiuc.edu/Research/vmd/) primarily for generating visualisation videos, as
they are better quality. The `vmd-files` directory README has more information.

### src

This is the main source code directory. They implement the classes found in
`inc/`. `inc/` has all the code for DPD calculations, the structs necessary and
the constants, as well as the code for implementing POLite based vertices (the
cells).

The README in this directory has more information on what each file does.

### submodules

This contains the repos for `tinsel` and `dpd-vis`. `tinsel` is the engine which
drives the POETS boxes, and contains POLite, the thin-layer API on which this
POETS DPD simulator is implemented, and facilitates all the necessary
communication between host x86 machine and the POETS boxes.

`dpd-vis` is the way we used to visualise DPD simulations. It allowed for
real-time visualisation and some playback, but has been deprecated somewhat, as
we now store states, convert them to PDB and use
[VMD](https://www.ks.uiuc.edu/Research/vmd/) as it generates better videos.

### tests

This contains scripts to perform regression testing. It is useful to test every
feature, old and new, in case any combination of old/new features somehow
improves performance. It also contains CSV files holding input and expected
outputs bead states, which is what is tested against. The README in this
directory provides more information on tests.

### utils

This contains some scripts used by the RISCV compiler.

### vmd-files

To generate videos, you need to use
[VMD](https://www.ks.uiuc.edu/Research/vmd/). This contains some scripts to help
with this. See the README in this directroy for more information.

### xml-generators

Source code for taking an xml-graph type and generating the DeviceInstances with
the initial state containing beads, and the EdgeInstances connecting the
devices.

### xml-graph-types

Graph types for an XML version of DPD aimed for used with The Orchestrator.

### Makefile

This is quite long, but contains the necessary recipes to generate a simulation
that can use any valid combination of macro flags. The macro flags are explained
more in `docs`.

### config.json

This is used on a client x86 machine when built to use `dpd-vis` to visualise a
simulation at runtime.
