# src

Many of the files within this directory are the implementation of files found in
[`inc/`](../inc/). For those, I will provide links to the description of those
in the README of `inc/`.

For the rest I will provide a brief description here.

## Implementations of Class headers

- [BeadMap.cpp](../inc/README.md#beadmaphpp)
- [Cells.cpp](../inc/README.md#cellshpp)
- [DPDSimulation.cpp](../inc/README.md#dpdsimulationhpp)
- [Executor.cpp](../inc/README.md#executorhpp)
- [ExternalClient.cpp](../inc/README.md#externalclienthpp)
- [ExternalServer.cpp](../inc/README.md#externalserverhpp)
- [gals.cpp](../inc/README.md#galsh) - The implementation is in `gals.h`.
- [HostMessenger.cpp](../inc/README.md#hostmessengerhpp)
- [POLiteCells.cpp](../inc/README.md#politecellshpp)
- [POLiteMessenger.cpp](../inc/README.md#politemessengerhpp)
- [POLiteSimulator.cpp](../inc/README.md#politesimulatorhpp)
- [POLiteVolume.cpp](../inc/README.md#politevolumehpp)
- [RDFCalculator.cpp](../inc/README.md#rdfcalculatorhpp)
- [RDFCells.cpp](../inc/README.md#rdfcellshpp)
- [SerialCells.cpp](../inc/README.md#serialceppshpp)
- [SerialMessenger.cpp](../inc/README.md#serialmessengerhpp)
- [SerialSimulator.cpp](../inc/README.md#serialsimulatorhpp)
- [SerialVolume.cpp](../inc/README.md#serialvolumehpp)
- [Simulator.cpp](../inc/README.md#simulatorhpp)
- [SimulationVolume.cpp](../inc/README.md#simulationvolumehpp)
- [RDFVolume.cpp](../inc/README.md#rdfvolumehpp)
- [sync.cpp](../inc/README.md#synch) - The implementation is in `sync.h`
- [utils.cpp](../inc/README.md#utilshpp)
- [Vector3D.cpp](../inc/README.md#vector3dhpp)
- [Volume.cpp](../inc/README.md#volumehpp)
- [XMLCells.cpp](../inc/README.md#xmlcellshpp)
- [XMLGenerator.cpp](../inc/README.md#xmlgeneratorhpp)
- [XMLVolume.cpp](../inc/README.md#xmlvolumehpp)

### test.cpp

It is good to test an application regularly, and basic testing involves having a
set of fixed input and some expected output. DPD is no different, in the
`tests/` directory, we have a few CSV files containing different input, and we
load these into a volume and simulate them to test them.

This way, when developing we can run the tests to see if anything no longer
works correctly, and even pinpoint it to a certain feature (or macro flag).

The results of the simulations are output from the simulators and then compared
agains the expected outputs (also in CSV files) in `tests/`. These differ based
on what macros are being used (`-DDOUBLE_SQRT`, `-DBETTER_VERLET` and
`-DSMALL_DT_EARLY` have different results compared to not using them).

`test.cpp` loads the beads from the CSV, builds the simulator and volume, and
then starts the simulation and compares the result, reporting an error.

### RDF.cpp

RDF is discussed in [RDFCalculator.hpp](../inc/README.md#rdfcalculatorhpp). This
file takes states from a DPD simulation (in the form of JSON) and uses this to
build multiple `RDFVolumes` and `RDFCalculators` to calculate the RDF on those
volumes.

It handles the setting up of threads and placing them on cores that are not
busy, and even displays the status of the RDF.

### restart.cpp

Running simulations can take a long time, and sometimes you need to pause a
simulation to let other people use the hardware. In this case, if you are
running a **visual** simulation, you can take the last stored state (JSON),
convert it to a form usable by `restart.cpp` (CSV), and then this simulation
can be continued.

NOTE: This is not in the current Makefile, it will need adding. You can make
sure that the **exact** same flags are used for the original and restarted
simulation this way, as changing them could lead to a simulation going wrong.

This has also been somewhat left behind so may need some TLC to work with the
refactored code. I reccomend comparing it to any of the examples in `examples/`
as this will help work out how to build the volume and simulator.

### dpd-bridge.cpp

This is run on a client PC when it is visualising a simulation at runtime.

It has not been used for a long time, as we now use
[VMD](https://www.ks.uiuc.edu/Research/vmd/) which produces much nicer
visualisations and videos. It could still be used in some capacity if there
becomes scope for real-time visualisations once again.
