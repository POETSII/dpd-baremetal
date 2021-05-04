# Serial POETS DPD Simulator

This is a simulator which runs on a single thread on an x86 machine. It is
designed to use the same small cells as a POETS DPD simulation, but use x86
features to loop through them and use the same code to calculate forces, Verlet
and determine migrations etc.

With it running on x86, we can use logging and trace statements, standard C++
debug tools and so on, to debug any issues that are affecting any of the POETS
DPD simulators and fix them.

This is not meant to be a good or fast implementation, just a simple serial
version of the POETS DPD simulator to help us understand the inner workings and
fix things.

As much as possible it implements the same features and is subject to the same
macro flags, so anything that affects the POETS simulations should affect this
as well. If not, that may also help find the issue.

This does not implement any messaging, the hardware idle detection thats used
for the synchronous simulator, or the GALS communication and synchronisation.
This is only used for debugging the specific DPD code found in [inc/dpd.hpp]
(../inc/dpp.hpp).

The serial simulator also **does not** produce the same results as those that
are produced by the simulators that use the POETS hardware. The synchronous and
GALS simulators will produce the same results, but the differences between
RISC-V FPUs and x86 FPUs means the results for a floating-point operation may
differ slightly, and over time the more of these minor differences add up to
produce different results.

The results differing however does not mean that either is incorrect.

### A summary:

Pros:

- Allows more details to be output and checked for more deeper understanding of
the working of the DPD calculations
- Various tools and methods can be used to help debug problems

Cons:

- Very slow, it still operates on 1x1x1 cells.
- Doesn't implement any synchronisation methods used by POETS simulators (sync
or gals).

The code for this is found in
[inc/SerialSimulator.hpp](../inc/SerialSimulator.hpp) and
[src/SerialSimulator.cpp](../src/SerialSimulator.cpp).

### General serial operation in DPD

1. Update
2. Velocity Verlet
3. Migrate
4. Emit (optional)

The serial version adds no extra phases.

### 1. Update

In this phase, the serial simulator loops through every cell, starting by
running `local_calcs()`. Then to calculate forces from neighbour beads, rather
than send messages from cell to cell, it simply reads the state of each
individual cell. The serial simulator has cell states contain the pointers to
their neighbours, and these are looped through for the update phase.

The phase ends when the the loop has performed this for each cell, meaning all
the beads will have all their forces accumulated. It automatically moves into
the next phase.

2. Velocity Verlet

The simulator then loops through each bead in each cell, using the accumulated
force to calculate the new velocity and from this, the new positions. The
new positions are used to determine if any beads are migrating to another
cell. After this has been done for all beads in all ells, the loop will end and
the simulator moves into the next phase.

3. Migrate

As stated, the serial simulator does not implement messages being sent between
cells. Instead, for the migrate phase, the simulator loops through each cell,
finding if it has beads to migrate, and simply placing this in the target cells
state.

Once again, after this has looped through all cells, it will automatically move
into the next phase.

4. Emit (optional)

If the number of timesteps that have elapsed since the last emit is equal to the
`emitperiod`, then the simulator moves into emit phase. Otherwise, it simply
transitions into the update phase again, as this all occurs within an endless
loop.

The emit loop once again loops through each individual cell, emitting its
beads to the host, moving onto the next cell and so on. In this simulator, the
beads are passed via a concurrent queue to a separate x86 thread which handles
the messages in the same way as the message handler for the POETS
synchronous/GALS simulators.

When the loop has iterated through every cell, the simulator moves back to the
update state for the next timestep.

-------------------------------------------------------------------------------

This simulator has been implemented naively, but on purpose. This way it is
easier to understand and debug. The focus is primarily on this using as much of
the same functions to calculate pairwise bead forces as the POETS hardware
would, in order to provide a simpler method of developing and debugging.

The idea is to develop a new simulation example, or DPD feature using this first
and emitting and visualising the states, or analysing them, to check that the
outcome is what you expect. If not, you can add trace statements and use
debugging tools to find where the issue is and fix it. Following this, adding a
makefile command to compile a synchronous/GALS version of this is trivial, and
should produce similar results.
