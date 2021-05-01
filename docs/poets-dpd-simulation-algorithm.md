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

This phase is named so as it is where cells update their neighbours with their
local bead data, while concurrently being updated on their neighbours bead data.

##### Sending

If a cell has beads to share, it starts by calculating the forces that its local
beads affect on each other. This function is known as `local_beads()`. This
takes each bead in-turn and calculates its force acting on the other local beads.

Calculating the force uses a function called `force_update()`. This takes in the
data of two beads, and begins by calculating the **square Euclidean distance**.
The square is used to reduce the number of computationally heavy square root
calculations, and as the cutoff-radius is fixed per simulation, we can store the
square cutoff-radii for comparison to this.

If the square distance is greater than the square cutoff-radius, `force_update()`
returns a force value of 0. Only if the square distances is less than the
cutoff-radius does it then square root the square distance to get the accurate
Euclidean distance. This is then used to calculate the **Conservative**,
**Drag** and **Random** forces of one bead acting on the other, and returns the
sum of these forces.

The resulting force is then accumulated in the cell's state for the appropriate
bead and moves onto the next local bead pair.

After `local_calculations()` has completed, it returns to the send handler. This
function is run at the beginning of the update phase for every timestep, and is
run only once so multiple local calculations, before the first bead is sent.

Next, the cell then packs this cells location into the message, and this
beads ID, type, position and velocity (all needed for `force_update()`) into the
message and sends it to all neighbours. POETS automatically broadcasts, and
Tinsel handles the delivery of messages so there's nothing else to be done in
software for this.

The above is repeated (apart from `local_calcs()`) until a cell has sent all of
its beads, at which point it then waits.

##### Receiving

When a cell receives a message, it first takes the bead from this message, and
adjusts it's position relative to this cell. When a cell prepares a message for
sending in this phase, it packs the bead with it's position in the local space
of that cell. It also sends its own location within the volume.

This location can be compared to a receiving cells location and the difference
used to adjust the received beads position. Each cell is at most +/- 1 in each
dimension, and thus 1 is added/subtracted from the received positions in the
relevant dimensions. This gives us the accurate position of each bead relative
to the receiving cells location.

The cell then loops through all of its local beads, calling `force_update()` and
passing the received bead (with adjusted position) and each local bead in-turn.
Once again, the square Euclidean distance is calculated, and if greater than the
square cutoff-raidus, returns 0 force. Otherwise, it calculates and sums the
forces of the received bead acting on the local bead, returning these to be
accumulated for the relevant local bead in the cells state.

If a receiving cell has no beads, the receive handler ends.

##### End of phase

When a cell has sent all of it's beads, it waits. During this time it can
continue to receive beads to calculate the forces acting on its local beads.

The cell waits until all of its neighbours have sent all of their beads to it,
at which point it can progress into the next phase. This scenario can be
detected in a number of ways, and differs between the GALS, synchronous and
serial simulators. It then moves into the next phase.

#### 2. Velocity Verlet

This phase serves to take the accumulated forces calculated in the update phase,
and use this to determine the acceleration, the change in velocity and therefore
the new position for the next timestep.

This phase does not send or receive. Upon moving into this state, a cell simply
iterates through it's local beads, calling `velocity_verlet()`, which performs
the necessary calculations for updating their positions.

Once a bead's position has been updated, it then checks this to see if the
position falls outside of this cell's geography. This is a simple check to see
if the position of the bead in any dimension is either greater than 1.0, or less
than 0.0.

If this is the case, it then determines which neighbour cell it is migrating
to. If the position in any dimension is > 1.0, it is sending it to the +1 cell
in this dimension, and if < 0, it is sending it to the -1 cell in this
dimension. The code also checks for the edge of the volume, i.e. the 0th cell in
a dimension. If a cell is migrating -1 cells in this dimension, it will need to
be updated to appear on the opposite side of the volume, to the (c - 1)th cell,
where c is the number of cells in each dimension.

The migration destinations for any migrating bead are stored, ready for the
migration phase. This phase finishes after a cell has updated the positions
and determined migration for each of their local beads. It then moves into the
next phase.

#### 3. Migrate

Migration is a simple step of sending a bead to its new cell. However the
intricacies of the POETS hardware mean there are some caveats.

##### Sending

For each migrating bead, a cell will place it into a cell, which has its
adjusted position already. It will also place the location of the **target**
cell in the message.

##### Receiving

Concurrently, cells receive migration messages from their neighbouring cells.
As cells broadcast their messages to all neighbours, the target cell which is
placed in the message by the sender is immediately checked. If the target
location does not match the receiving cells location, the message is discarded.
This is vital to avoid bead duplication.

If the locations match, then this bead is accepted into the cells state. The
cell ensures that it has space in its state before accepting the bead. If there
is no space, the migrating bead will dissapear, which is not physically
possible, and therefore will report an error and prematurely terminate the
simulation.

##### End of phase

If a cell has no beads to migrate, or once it has sent all of its beads, it
waits. Once again it can continue to received messages from neighbours, and
disregard or accept a bead as appropriate.

A cell has completed the migrate phase when it has sent all of the beads it
needs to migrate, and received all migrate messages from it neighbours. Again,
this scenario can be detected in a number of ways, the method differing
depending on whether the simulation is using the GALS, synchronous or serial
method. It then moves into the next phase.

#### 4. Emit (Optional)

Emit is an optional phase. It is designed simply to pass all the bead data at a
timestep out to the host x86 machine, which can then store it for use in
visualisations or analysis.

This phase can take a while, as beads are being sent from many cells to one
host. The host may be more computationally capable than that of the individual
Tinsel threads, but a bottleneck can occur as the messages are being
sent from many threads via a single channel.

Therefore, this phase is optional. It is removed at compile time by macros, depending on
the mode used. Alternatively, it can be included but only called every given
number of timesteps, the **Emit period**.

If this number of timesteps has not elapsed since the previous emit, then cells
will move from the migrate phase to the update phase. If the correct number of
timesteps has elapsed, then the cell emits all its bead data, and then
immediately moves into the update phase.

The emit phase does not receive anything, and the end of phase is when a cell
has emitted all its beads, automatically beginning the update phase.

-------------------------------------------------------------------------------

Some ambiguities arise in this algorithm, such as how end of phases are
detected, and some questions on efficiency can be raised such as, why are all
local bead calculations performed upfront in the update phase?

These are answered to some extent within the remaining documents in this
directory, which describe the different simulators that use this algorithm
(GALS, synchronous and Serial), and the macro flag document which explains the
macros in place to provide options for how things are performed, which in-turn
provide trade-offs for total-instructions and performance.
