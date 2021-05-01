# POETS DPD Simulation Algorithm

## Dissipative Particle Dynamics

DPD simulators use pair-wise forces to approximate the effects that beads have
on each other. For each bead the forces acting upon it by every other bead are
accumulated at each timestep and this is used to perform velocity Verlet, which
updates all beads velocities and positions. The forces between beads are
calculated with the beads in their new positions, and velocity Verlet is
performed again, their velocity and positions are updated again, and so on and
so forth.

The volume has periodic boundary conditions, and any bead which moves outside
the bounds of the volume has its positioned changed so that it appears on the
opposite side of the volume with the same velocity.

A better explanation of how this works, and the equations used can be found in
[[1](http://www.cse.scitech.ac.uk/ccg/software/DL_MESO/MANUAL/USRMAN.pdf)] and
[[2](https://www.sciencedirect.com/science/article/pii/S0010465514002203)].

Forces acting between a pair of beads only occur when the Euclidean distance
between those beads is less than a **cut-off radius**. This radius is generally
set to **1.0**. The forces scale throughout this radius, with the stronger force
occurring the closer the beads are, and the force being 0 if their radius is
greater than the cut-off radius.

The challenge with DPD comes from working out which beads are within the cut-off
radius and then calculating the forces.

Consider a naive implementation, where a volume full of beads is calculated on
one thread, with no separation of the volume space into separate cells. For every
timestep, it would need to take a bead, and check its Euclidean distance against
every other bead in the volume, calculating the forces if it is less than 1.0.
In a large enough volume, 1.0 is a negligible radius, and it's possible that you
would be calculating the Euclidean distance (which includes a
computationally heavy square root functions) `(b^2 - 1)` times, where `b` is the
number of beads.

Parallelisation is therefore a useful tool in speeding up DPD simulations.
Splitting a volume into several cells, with each cell running in parallel is the
method used by parallel DPD simulations. This means that each cell calculates
the Euclidean distance of pairs of beads only within this cell. Now, if a
volume is split into `x` cells these will have `1/x` of the Euclidean distance
calculations to perform, and as they are running in parallel, theoretically they
can reduce the runtime by `1\x`.

However, while these cells can perform calculations within themselves faster,
they are geographically adjacent to one-another, and the beads at the
neighbouring edges of cells will interact, and be able to move between them.
This adds some extra steps: 1. Sharing beads between cells for calculating the
forces acting on beads in the neighbouring cell, and 2. Migrating beads whose
position changes to outside of it's current holding cell.

For beads near the edge, cells can have **Halo Regions**. These are areas around
each edge of a cell with a width/height at least the same as the cut-off radius.
These areas will contain only the beads which are likely to interact with beads
in neighbouring cells. These are then shared in any way necessary, and cells
calculate the effect these beads have on their local beads existing in the
appropriate halo region. This method drastically reduces the overhead of
communicating between cells (and threads) by sending fewer beads which are more
likely to interact with beads in neighbouring cells, instead of sending all
beads.

Migrating beads simply involves checking the position of beads following
calculating the new positions during velocity Verlet. These are then passed to
the appropriate neighbouring cell.

These extra steps, sharing halo region beads and migrating beads, still help to
reduce the total number of unnecessary Euclidean distance calculations and
reduce the overhead of communication as much as possible, and are therefore much
preferred to a single threaded volume.

Such simulators generally use existing x86 parallel methodologies: They use
multiple threads per machine and/or multiple machines, communication methods
such as MPI and so on. The more threads you can use, the more cells can you
use, the smaller each cell is, with each cell having fewer beads to perform
calculations and and share with neighbours. However, in such systems,
communication through sharing information tends to be a bottleneck, and a sweet
spot must be found where cell size and communication are balanced to provide the
smallest and therefore fastest cells possible without being bogged down by the
communication between these cells.

## POETS DPD Simulation Algorithm

The POETS DPD algorithm takes the general algorithm as discussed above, and
adjusts it to play to the strengths of the Tinsel hardware and POETS programming
model. The hardware has incredibly fast communication, albeit with small message
sizes, albeit with small messages, and we have access to thousands of threads
(maybe more in the future), albeit with much reduced compute power and shared
resources.

For this reason, we switch the focus from compute to communication as much as
possible, i.e. Reduce the number of calculations a thread has to perform, but
increase the communication between the threads.

Reducing the number of calculations can be done by reducing the cell size. If
each cell can only contain a handful of beads they will have a much reduced
workload. But will have to send and receive beads to and from all neighbouring
cells. We shrink the cell size to **1.0**, the same as the cut-off radius. This
means that any bead in a cell will be able to interact with any bead in the same
cell or in neighbouring cells. This helps us build an application graph,
where a vertex is a cell, and each cell connects **only** to it's nearest
26 neighbouring cells (a 3D simulation).

The increase in messaging comes from not using halo regions. Halo regions are
around the same size as the cell is in POETS DPD, so using these would add
overhead. Instead of only sharing beads which exist near the edge of a cell with
the appropriate neighbour, we share every bead with every neighbour. Sharing a
bead with all neighbours may lead to beads being received by many neighbours
which don't affect any of their local beads, but messaging is so cheap in POETS
it is worth it to discard a message than add compute-overhead to work out the
best neighbours to send a bead to.

In principle, this algorithm combined with the POETS hardware and programming
model should produce a simulator which produces the same or similar results, but
in a faster runtime. This performance should scale according to the volume size
or the number of beads, depending on how many threads we have. The more threads
there are, the more cells we can run in parallel, and the more messages we can
send per second.

### Phases of the algorithm

This algorithm, similar to the x86 parallel implementaions described in [The
first section](#dissipative-particle-dynamics) has the same basic phases:

1. Update
2. Velocity Verlet
3. Migrate
4. Emit (optional)

We will describe the operation of each of these phases in the POETS DPD
algorithm, aiming to describe the difference to that of the x86 parallel
algorithm.

#### 1. Update


