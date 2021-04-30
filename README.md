# dpd-baremetal
_A POLite version of the DPD application_

## Documentation

This README is designed to give an overview of how to run something quickly.
More details are provided in the `docs` directory of how the POETS DPD algorithm
actually works, and how macro flags and makefile recipes work to produce
simulations that achieve a result but with differing performance.

Each directory has a README explaining what its containing files do. At the end
of this README, a brief explanation of what each of the directories in the root
contains is provided.

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

To run the tests:

```bash
cd tests/
./test-all
```

This will run a script which tests everything. Again, it may take a long time,
and occasionally the boxes will fail a startup self-check. In this case, either
comment-out the completed tests from the test-all script and run again, or run
the whole thing again.

If all complete successfully, it will inform you, and you can start running
other things and developing.

## Running a POETS DPD simulation

There are a few methods of running a simulation, each of which the
basics will be explained here.

### Visual DPD Simulation

A visual simulation runs a DPD simulation and stores the state of the beads at
given intervals (default: each timestep). The interval, `emitperiod`, can be
changed in `inc/DPDConstants.hpp`.

The x86 host machine will store the bead states in `polite-dpd-states/` as
JSON files, one file per timestep. These files can then be used to generate a
PDB file for use with [VMD](https://www.ks.uiuc.edu/Research/vmd/) which
visualises the data, and can generate videos.

The following commands build the volume, add the beads, and then write it down
to hardware, which then runs the simulation. It is recommended to run `make
clean` between each make, to avoid clashes.

Build the simulation binaries:
```bash
make clean
make visual-gals-vesicle-fastest
```

Once these have compiled, run the simulation:
```bash
cd bin/
./run v
```

Where `v` is an integer >=3 for the length of one side of the volume.

This will run continuously until stopped with `Ctrl+C`.

#### DRAM

Large volumes may not fit in SRAM. In this case, when running with the above,
the simulation will terminate before running and indicate that SRAM is full.

In this case, return to the root directory of dpd-baremetal and run the
following:

```bash
make clean
make visual-gals-vesicle-fastest-dram
```

This will build the same simulation binaries, but this time direct the POLite
mapper to store cells in DRAM instead. This will allow much larger simulations
to be run, at a loss of performance (longer runtimes). Running this is the same
as before.

`-dram` can be applied to any Makefile recipe for this to allow for larger
simulations of all types.

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

Where `v` is an integer >=3 for the length of one side of the volume, x is the
number of boxes in the x-dimension and y is the number of boxes in the y
dimension.

Using more boxes means there are more available threads, so larger simulations
will run faster on more boxes.

### Timed DPD simulation

For the purposes of testing the performance of the simulator, we provide a timed
method, which runs the simulation for a given number of timesteps, and reports
the runtime when it has completed. This version **does not** emit any bead
states, this is a slow process, we're trying to find the best-case runtimes of
this simulator.

Building the simulation binaries:
```bash
make clean
make visual-gals-vesicle-fastest
```

Once these have compiled, run the simulation:
```bash
cd bin/
./run v
```

Where `v` is an integer >=3 for the length of one side of the volume.

By default, this will run for 10,000 timesteps, and the first cell to reach this
will inform the host x86 machine, which will report the runtime, and store it
in a file "mega_results.csv".

This CSV file  can be used in conjunction with the `mega-tests.sh` script.
`mega-tests.sh` is designed to run volume sizes from a given minimum and
maximum, for a given Makefile recipe. It runs each volume size 3 times, as the
runtimes are likely to change based on message orderings etc. These are averaged
to provide a better idea of runtimes.

#### Different length runs

10,000 timesteps is chosen as, MPI DPD (which we compare to a lot as it is the
gold-standard), the runtime includes set up times. Our runtimes do not, but to
make sure the runtimes are negligible for MPI DPD when we compare runtimes, we
use 10,000 timesteps.

However, you can run POETS DPD simulations for any number of timesteps. Simply
use:

```bash
./run v --time t
```

Where `v` is an integer >=3 for the length of one side of the volume, and `t` is
an integer for the number of timesteps to run.

This can also be used in conjunction with `--boxes-x` and `--boxes-y` to compare
runtimes on any arrangement of POETS boxes.

### Serial DPD simulator

A serial simulator is provided which uses the same DPD calculation code
`inc/dpd.hpp` as the POETS simulators (GALS and synchronous).

Developing with POETS can be a challenge as there's very little output
capabilities in order to debug when there is an error.

The serial simulator is aimed at running the same simulations and producing
similar results that can be indicative of those from the POETS boxes (x86 vs
RISCV FPUs produce different results). There is the added benefit of using
standard C++ tools to debug, and being able to output anything using stdout, as
well as being able to develop on any x86 machine.

The serial simulator can do visual and timed runs, the makefile recipes
determine which type.

```bash
make serial-visual-vesicle
```
OR
```bash
make serial-timed-vesicle
```

Both can be run in the same way as the POETS simulator:

```bash
cd bin
./run v --time t
```

Where `v` is an integer >=3 for the length of one side of the volume, and `t` is
an integer for the number of timesteps to run. `--boxes-x` and `--boxes-y` will
not work with this version.

### Directory information


