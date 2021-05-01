# Synchronous POETS DPD Simulator

Applications such as DPD which use POLite to make use of the POETS Tinsel
hardware have access to the **hardware idle detection**. This is built into the
hardware and requires no extra code in the application to detect it.

This feature detects when all messages have been received and handled, and no
vertices (cells in the case of DPD) wish to send. This can be detected very
quickly, and calls a function, `step()` in all vertices in the POLite
application. This **synchronises** all of the vertices.

This synchronisation can be used at natural points in applications where
vertices need to await all other vertices reaching this point before any
vertices can continue.

### A summary:

Pros:

- Built in - no code in the application to synchronise, so smaller total
instructions, requires no extra messages.
- Simple to use - A handler is provided which is automatically called.

Cons:

- Slowest cell dependent - The synchronisation will occur only after the last
cell has sent/received all messages. If this is a particularly slow cell others
could be waiting a while, which is inefficient.
- Global synchronisation - All vertices across all threads across all boxes must
have completed, and skew can occur in calling `step()` leading to some cells
moving ahead and waiting longer for the next synchronisation.

## POETS DPD Simulator

The POETS DPD simulator can also make use of this. As discussed in the [POETS
DPD Simulation Algorithm document](./poets-dpd-simulation.md), certain phases
end when a cell has sent all it needs to send, and received all it needs to
receive. This is precisely what this idle detection does.

The code for this is found in [inc/sync.h](../inc/sync.h).

This does not add or remove phases, but helps the cells move through phases as
follows:

### 1. Update

The update phase occurs as expected, all cells performing `local_calcs()` before
sending all beads to all neighbours, and concurrently receiving beads from
neighbours and calculating the forces these beads have on local beads.

The synchronisation point will occur when *all* cells have sent *all* of their
beads, and *all* beads have been received and used in force calculations. Upon
the last bead being received, the update phase will be complete. The hardware
will then call the `step()` function of *all* cells, which moves it into the
next phase.

If one cell is particularly slow, due to containing more beads than the others,
this cell will be the last to finish. Any cells which finish before this will
therefore be idling for a long time awaiting this slow cell to complete this
phase. This problem pervades other phases too.

### 2. Velocity Verlet

This is performed in the `step()` function. There is one `step()` function in a
vertex (cell), so it checks that the previous phase was the update phase before
calculating the new velocities and positions, and determining which, if any,
bead are migrating in the next phase.

As this phase does not send or receive, the entire process can be performed
within the `step()` function.

After Verlet for all beads has been calculated, the cell automatically moves
into the next phase.

### 3. Migrate

This phase, similar to update, will send and receive beads concurrently, this
time migrating them to their new cell, cells will reject beads if the target
location embedded in the received message doesn't match their own location, and
accept them (if they have space) if the locations match.

Once again, the natural synchronisation occurs when *all* cells have migrated *all*
of their beads, and *all* migrate messages have been received. The `step()`
handler is then called once again.

### 4. Emit

When the `step()` function is called after idle is detected by the hardware at
the end of the migrate phase, it again checks which phase just completed, and
will then move into emit following the migrate phase.

At this point, it checks whether the number of elapsed timesteps since the last
time the state was emitted, and if it is equal to `emitperiod`, at the end of
`step()`, it will then start sending beads to the host.

Otherwise, it will move into the update phase, and `step()` will end, starting
the next timestep.

------------------------------------------------------------------------------

Overall this is a simpler method of writing a POETS application. It
allows more focus on writing the application than dealing with synchronisation.
In certain applications it can be faster, as it frees up the messaging system
and there can be less logic.

This is not the only method of synchronisation. A [GALS](./gals-simulator.md)
version exists which doesn't use hardware idle detection, and sees neighbouring
cells synchronise among themselves after communicating via messages.

POETS DPD sees similar performance in the synchronous and GALS versions.
Synchronous may see cells waiting, but when sending/receiving and calculating
forces, it doesn't have to deal with additional counters. GALS may mean that
more cells are running more of the time (compared to synchronous), but it has to
keep track of additional variables to sync, and send more messages.

The serial simulator runs on one x86 thread, so uses simple loops to iterate
through cells, so synchronisation is trivial.
