# GALS POETS DPD Simulator

**Globally Asynchronous Locally Synchronous** is a methodology used in computer
science and hardware design, whereby an entire system does not have any
synchronisation, but groups of components with faster completion
times synchronise with each other, allowing
them to perform more than components with slower completion times, which are
themselves grouped. Multiple different groups of components then communicate
asynchronously with one another.

This can allow the faster components to provide information more regularly, so
when the slower components are ready to use it it has the most up-to-date
information possible.

In terms of a POETS application, this means that vertices (DPD cells) will
communicate with their neighbours, sending and receiving data (via messages) to
keep each other apprised on their current state.

Vertices will only have to wait until they know their neighbours are able to
continue, meaning that at any given time, at least one vertex is progressing.
Some parts of an application graph can run ahead (in terms of timesteps for DPD)
and slower parts will continue to run at the fastest pace they can, and this
won't affect the rest of the application.

### A summary:

Pros:

- Independent speed - If it's neighbours are ready, a vertex can continue, it
doesn't need to know about the state of the global application.
- Continuous operation - At least one vertex will always be running, and overall
vertices are waiting for less time, more efficient.

Cons:

- Complex - Requires extra variables and code in the application to manage
- Increase load - Sends more messages overall, which adds traffic to the
messaging system

## Working in POETS DPD Simulator

The POETS DPD simulator is regularly connected - each cell has 26 neighbours and
sends and receives from all of them. We can therefore use the same edges to send
messages containing whatever we want, including beads and information about the
cells state.

This can be used to provide synchronisation data, such as how many beads a cell
has to send, and when it has finished sending beads. This information can be
used to keep an idea of how many neighbours are ready to move into the next
phase. This way, a cell is waiting for only 26 cells to complete before moving
into the next phase, instead of potentially all other cells.

The code for this is found in [inc/gals.h](../inc/sync.h).

### General GALS operation in DPD

GALS adds some extra modes:

1. Update
2. Update complete
3. Velocity Verlet
4. Migrate
5. Migrate complete
6. Emit (optional)
7. Emit complete

These *complete* modes are modes where extra messages are sent to indicate to
neighbours that these are ready to continue. The update and migrate modes also
send more data in bead messages such, and keep track of three key factors of
their neighbours:

#### First messages of this phase from each cell

On POETS hardware, the order that messages are sent from one vertex to another
does not guarantee that they will be received in the same order. It is therefore
possible that the first message of a phase from a cell will arrive after a later
message.

For these modes, the first message sent in each phase includes a type value
which indicates that it is the first message from this cell. The message also
includes the total number of beads this cell has to send for this phase. The
first message increments a counter referring to the first message of this phase
from each cell. It also adds the total number of beads from that cell to the
local cells total expected beads.

#### Total beads expected to receive

With the variable counting the number of first messages, we can ensure we do not
move on from a phase until we have received a first message from every
neighbour. These messages also give us the total number of expected beads, which
is necessary to ensure a cell does not move on until it has a chance to
calculate all the forces. When a bead is received, including in the first
message, it will decrement the total expected bead counter, and the phase will
be allowed to change when this is 0.

Since the first message sent will not necessarily arrive first, it is necessary
to keep a count of both of these as the expected bead count may be 0 before all
beads have arrived, so the combination of total expected beads and number of
first messages received (and consequently the messages containing the total
number of beads) will give us all the information necessary from neighbours.

#### Phase complete message.

The phase complete message is sent by a cell when it has finished sending all
the beads it needs to in this phase. This is done to indicate to all neighbours
that it has moved into the phase complete phase (update complete or migrate
complete).

In these complete phases, it can still receive and handle messages from
neighbours that are in the previous phase, but it can also receive messages from
neighbours in the next phase. This overlap of phases is the helpful for the GALS
simulator, to reduce idle time of cells.

Keeping a count of the neighbours that have moved into this state is important.
A cell that knows all of its neighbours are in this state (which this cell
will be in as well) means this cell an progress, safe in the knowledge that its
neighbours will be able to handle the messages it wishes to send.

## The modes

### 1. Update

As expected, in the update phase, if a cell has beads to send it will begin by
performing `local_calcs()` to find the forces acting between local beads. It
will then place the first bead in a message, again with the location of this
cell. As this is the first bead it is sending, it will then set an identifier in
this message to show that this is the first message this cell is sending, and
then place the total number of beads this cells has.

When a bead is received in this phase, it uses the bead in the same way as
previously, adjusts its position to make it relative, and for each local
bead, find if this received beads position falls within the cutoff-radius, and if
so, calculate its affect on the local beads.

When a bead is received with the first message identifier enclosed, the
receiving cell then increments the counter of first messages from each cell, and
adds the total beads the sender has reported to its expected beads to receive
variable.

The expected beads to receive counter will be decremented when any bead is
received.

Note: A corner case exists where a cell will receive all of its beads from all
of its neighbours before it has sent any beads itself. This event will set all
counter variables to the correct value for this cell to move into the next
phase, but it's neighbours will be awaiting its first bead, and therefore the
application will lock up. To prevent this, cells will not allow the transition
to the next phase until they have sent their first message.

### 2. Update complete

When a cell has sent all of its beads it automatically moves into the update
complete phase. A cell with no beads will automatically move into the update
complete phase. Regardless of these, when a cell enters update complete, it
sends a message identifying this to all of its neighbours.

If the cell had no beads to send, it places a token in the message to show this,
and the receiving cell will increment the variable which holds the count of the
number of first messages received, as well as the counter of the neighbours who
are in the update complete phase.

In this phase, cells can continue to receive update beads from neighbours still
in the update phase. These are used to calculate forces on local beads as
normal. However, in this phase a cell can also receive beads from the
next phase and onto migrate. The migrate beads received are once again checked
to ensure that this cell is the intended destination, rejected if not, and
accepted and stored in the cells state if they are.

As soon as a cell has received update complete message from all neighbours, it
will move into the Verlet phase. This does not mean all neighbours can move
into this phase, some of their neighbours may be sending update beads
still, but it is the indication that this will no longer be sending beads, and
that it can accept either an update bead or a migrate bead.

When a cell is in the update complete phase, whenever it receives or sends an
update bead message or an update complete message (it may be the last to send
an update complete message and therefore be able to transition immediately), it
runs `update_complete()`.
This function checks the following:

- Have I sent all of my beads?
- Have I received a first message from all neighbours?
- Have I received all expected beads?
- Have I moved into update complete?
- Have I received update complete from all neighbours?

If the answer to all of these questions is "yes", then the cell moves into the
next phase.

### 3. Velocity Verlet

This is performed in the `update_complete()` function. This is because, when a
cell knows all of its neighbours are in update complete, that they will not be
receiving any more update beads which are used to calculate forces acting on
local beads.

At this point, we can now perform velocity Verlet to update bead velocities and
positions. The cell will also determine if any beads need migrating and their
destinations, then automatically moves into the migrate phase.

### 4. Migrate

The migrate phase will start with a cell preparing to send a migrating bead by
packing it into a message, and placing the target cells location in the same
message. It will then place an identifier in the message to indicate this is the
cells first message, and place the total number of migrating beads this cell
has in this message.

When a cell receives a migrating bead, regardless of whether this cell is the
intended target or not, it will update the appropriate counters; The number of
cells whose first message it has received and the number of expected migrating
beads (regardless of target once again). It will then disregard the bead or
accept it depending on whether it is the target.

Receiving further migrating beads will then decrement the counters. Once this
cell has migrated all of the beads it intends to migrate, it will move into the
migrate complete phase.

### 5. Migrate Complete

When a cell has sent all of its migrating beads, it will then automatically move
into the migrate complete phase, and send a message indicating this has
happened. If a cell had no migrating beads, then it immediately moves into the
migrate complete phase and sends a message with an identifier to show that this
cell had no beads.

When a cell receives a migrate complete message, it will increment the counter
of how many neighbours are in this phase, and if the message indicates the
sending cell had no beads to send in the first place, it will increment the
variable counting the number of neighbouring cells which have sent their first
message.

Once again, this mode indicates to neighbours that a cell intends to send no
more beads. Some of these cells may then be able to transition into the next
phase safely, knowing they are not expecting to potentially accept more beads
into their state.

Similar to `update_complete()`, `migrate_complete()` is called when a cell is in
migrate complete and either sends its last migrating bead, receives a migrating
bead, sends migrate complete or receives migrate complete. This checks:

- Have I sent all of my beads?
- Have I received a first message from all neighbours?
- Have I received all expected beads?
- Have I moved into migrate complete?
- Have I received migrate complete from all neighbours?

If the answer to all of these questions is "yes", then the cell moves into the
next phase.

### 6. Emit (Optional)

`migrate_complete()` will check whether the number of timesteps since the last
emit have occurred and if so, move into the emit phase. Otherwise, it will skip
straight to emit complete.

This phase sees cells send only to the x86 host machine, and not receive
anything. However, it is necessary to synchronise before this using the migrate
complete phase as if a cell emits before it receives a bead which is being
migrated to it, this bead will not be emitted, which has repercussions as it
will not be included in any analysis, for example.

### 7. Emit complete

Emit complete is used to ensure that cells have finished emitting before they
move into the update phase again. It is similar to migrate complete and update
complete in that a cell sends a message to all neighbours indicating that it is
in this mode, and when a cell receives indication from all neighbours it can
move into the update phase to start the next timestep.

Cells in the emit complete mode can receive update bead messages and start the
next timestep.

------------------------------------------------------------------------------

GALS does add extra phases, but they overlap, allowing cells to move on at their
own pace, instead of waiting for all cells. It does also add extra variables and
logic, which means some of the total compute is focussed on non-DPD operations,
but this is a small percentage. The messaging system is fast and cheap, so extra
messages to synchronise with neighbours is quick and simple.

This is not the only method of synchronisation. A
[Synchronous](./synchronous-simulator.md)
version exists which uses the hardware idle detection features, and sees cells
wait for all to complete before moving phase.

POETS DPD sees similar performance in the synchronous and GALS versions.
Synchronous may see cells waiting, but when sending/receiving and calculating
forces, it doesn't have to deal with additional counters. GALS may mean that
more cells are running more of the time (compared to synchronous), but it has to
keep track of additional variables to sync, and send more messages.

The serial simulator runs on one x86 thread, so uses simple loops to iterate
through cells, so synchronisation is trivial.
