# Macro flags and makefile recipes

Macro flags are used by the pre-compiler to add/remove sections of source code
that are identified by these named flags.

In POETS DPD, we have used macros when adding code to implement various features
or improvements for the simulator. These allow us to select the features we
want in varying combinations, which can give us better performing code, more
accurate DPD calculations, or smaller number of instructions.

This document will list the macro flags and give an explanation of each,
explaining the reasoning behind its inclusion, its benefits, its shortcomings
and what other flags it can or cannot be use in conjunction with. Some
combinations which are currently the best for various reasones are described at
the end.

This will also explain how to use these in combinations for Makefile recipes.

## Simulator choice

These are the flags and recipes which choose which simulation implementation to
use. These **cannot** be used together, only one must be chosen.

### Synchronous

**FLAG: n/a** - **makefile: Prefix `sync-`**

This will compile the synchronous simulator for use with the POETS hardware. It
does not have a flag, it is the default. The sync and gals simulator will
produce the same result (when the same macro flags are used), with differing
performance.

### GALS

**FLAG: `-DGALS`** - **makefile: Prefix `gals-`**

This will ensure that the compiled simulator uses the GALS method of
synchronisations. The sync and gals simulator will
produce the same result (when the same macro flags are used), with differing
performance.

### Serial

**FLAG: `-DSERIAL`** - **makefile: Prefix `serial-`**

This will ensure the compiled simulator uses the serial x86 simulator. This will
be slower than the POETS simulator (for large simulations), but is much easier
to debug the DPD calculation code. It does not implement any messaging or
synchronisation to match that of the sync/gals simulators.

## Simulation operations

These flags can be used to produce and run the same simulations, but with
different outcomes for different purposes.

There part in the makefile recipes are **always** the second part, e.g.
`sync-timed-` or `serial-test-`.

These **cannot** be used together, only one must be chosen.

### Visual

**FLAG: `-DVISUALISE`** - **makefile: `-visual-`**

This flag will ensure emit is part of the simulator, and based on the
emitperiod, will output the cells state at regular intervals, which is stored
as JSON files in the `x-dpd-states`, where `x` is `polite` if the sync or gals
simulator are being used, or `serial` if the serial simulator is being used.

While called visual, the JSON files holding the bead states can be used for
visualisation or analysis.

This flag can be used for any simulator type, with combinations of any local
calculations flag, and any combination of feature flags.

### Testing

**FLAG: `-DTESTING`** - **makefile: `-test-`**

Testing sets any of the simulators up to run with a set of input beads for a
given number of timesteps. These then return all the beads which are compared
against a set of expected positions (stored in [tests/](../tests)).

This tests that the simulators are working correctly, and should be used
regularly when changing any code. It includes full regression testing for all
features, old and more recent, and should be kept updated. The makefile includes
all possible combinations of macro flags for testing.

When running testing with `-DBONDS` testing will automatically use an input file
with bonded beads. It is used to ensure that bond forces are not broken by any
sort of change.

#### Large tests

**FLAG: `-DLARGE_TEST`** - **makefile: `-large-`**

Large test is only used with testing, and indicates to the test code that a
much larger test is to be used (to make sure that when there are multiple cells
per thread that nothing breaks). It imports a certain set of beads, runs and
then outputs the bead states which is compared against a certain file full of
beads.

### Timed

**FLAG: `-DTIMER`** - **makefile: `-timed-`**

This flag will not include any emit mode. The simulators will simply run for a
given number of timesteps, and then send a message to the host x86 machine which
will report the time from the simulation being started to finishing. This is the
mode used for performance testing.

It can be used with sync, GALS, serial, any local calculations flag, and any
combination of feature flags.

### Stats

**FLAG: `-DSTATS`** - **makefile: `-stats-`**

POLite provides the option to record various statistics, such as the number of
messages threads send, the number of blocked sends that occur, the amount of
time the threads are kept idle, and so on. These stats are useful for seeing
more into how the hardware works.

This **only** works with the POLite simulators, synchronous and GALS. It can be
used with any other combination of local calculations flags, or feature flags.

## Writing vertices to DRAM

**FLAG: `-DDRAM`** - **makefile: Suffix `-dram`**

This is an outlier in that it can be applied to **any** combination of flags.
When a simulation volume becomes particularly large, the cells will no longer
fit in the POETS SRAM.

However, at this point we can map the cells to DRAM while sacrificing
performance, and continue to run simulations with larger and larger volumes

If this is to be used, it will **always** be the last part of the makefile
recipe.

## Simulation examples

These are what are to be used to compile one of the examples in the
[examples/](../examples/) directory. They cannot be used in conjunction with
each other, but can be used with any simulator, whichever operation, any of the
local calculation options and any combination of the features (unless otherwise
stated).

The makefile recipe will always include this as the fourth part: i.e.
`sync-timed-vesicle-` or `serial-visual-corners-`.

### Oil and Water

**FLAG: n/a** - **makefile: `-oilwater-`**

This includes: 60% water beads, 30% of 1 type of oil and 10% of a second type of
oil. This is the default in terms of flags, and selects the appropriate
constants from the constants file.
([inc/DPDConstants.hpp](../inc/DPDConstants.hpp)).

This requires an argument of the length of one side of the volume to be
simulated. It has a bead density of 3 (beads per unit volume).

### Vesicle self assembly

**FLAG: `-DVESICLE_SELF_ASSEMBLY`** - **makefile: `-vesicle-`**

This includes 1.5% of the total polymers as chains which form the vesicles and
98.5% of the total molecules as water beads.

The flag will select the constants for this simulation, which includes
different repulsive strengths (in the `A` matrix), and a different bond length.

This will see the polymer chains come together and form
[vesicles](https://en.wikipedia.org/wiki/Vesicle_(biology_and_chemistry)#:~:text=In%20cell%20biology%2C%20a%20vesicle,materials%20within%20the%20plasma%20membrane.).

This requires an argument of the length of one side of the volume to be
simulated. It has a bead density of 3 (beads per unit volume).

### Corner tests

**FLAG: n/a** - **makefile: `-corners-`**

This will place two beads in a small volume which repel eachother gently. One
of them will eventually be pushed across a corner boundary, and should appear at
the exact opposite corner. Eventually these beads will get closer and repel
eachother again, pushing eachother back in the opposite direction, and the same
bead will cross the same corner boundary once again.

This does not take any arguments. It uses the same `A` matrix as oil and water.

### Box of oil and water featuring gravity

**FLAG: `-DGRAVITY`** - **makefile: `-gravity-`**

In this example, the bead make-up is the same as oil and water: 60% water, 30%
oil type 1 and 10% oil type 2. It also uses most of the same constants as that
example, but a new `A` matrix which includes the wall beads interaction
strength.

We add some code to subject only water beads to gravity in this example, but in
order to see the effect more clearly, we add **walls** to this volume by
making it 1 cell larger in each dimension and placing 4 unmoving beads (they
are never subject to velocity Verlet) in the edge and corner cells. These will
then repel all non-wall beads back gently, which should allow us to see water
beads gently fall and settle at the bottom of the volume box.

The wall beads state will not be stored when emitted.

## Local calculations options

We can change the point in a timestep when local calculations are performed, and
spread the load of the bead calculations in various ways, which can
improve performance and total instructions to varying degrees. This
unfortunately cannot be extended to calculations featuring a neighbouring bead.

Only one of these can be used at a time. Any combination will lead to undefined
behaviour. Unless stated, any of these can be used with any of the simulators,
any operation, and any feature flag. If a local calculations flag is to be used,
this is always the fourth part of the makefile recipe, i.e.
`serial-timed-oilwater-onebyone`, or `gals-visual-vesicle-sendtoself`.

### Default

**FLAG: n/a** - **makefile: n/a**

The default performs as described in
[POETS DPD Simulation Algorithm](poets-dpd-simulation-algorithm.md).
In short, all of the local calculations are performed all at once before the
first bead is sent to a cells neighbours.

This can reduce performance as all of the cells will be trying to calculate
these at the same time, fighting for the small number of shared FPUs. Those that
finish first will then start sending their beads, which will have to wait to be
received until those cells that are still calculating the local forces have
finished.

### Local forces calculated one by one

**FLAG: `-DONE_BY_ONE`** - **makefile: `-onebyone-`**

Before any bead is shared during the update phase, `local_calcs()` is called
which calculates the force that this bead enacts upon the other local beads.
`local_calcs()` is changed to handle this.

With this option, performance is improved. This is due to each cell spending
less time wanting to use an FPU. And the cells get into a nice balance of some
sending a bead, some calculating forces of neighbour beads and some performing
local calculations. It spreads the load of the FPUs across the update phase,
rather than having lots of them upfront.

### Cells sending local beads to themselves

**FLAG: `-DSEND_TO_SELF`** - **makefile: `-sendtoself-`**

In the POETS programming model, vertices are capable of sending messages to any
other vertex, including themselves. We take advantage of this by including a
cell as a neighbour of itself. With this option, we remove the `local_calcs()`
function all together, and the force local beads have on each other are
calculated like any other neighbour bead.

This tends to improve performance vs the default local force option, but is
slower compared to one by one, and does not scale in as smooth a manner.
However, removing a function does free up some space for more features, at a
slight cost of performance.

By default, this option is used in the GALS simulator, to free up instruction
space. Using this flag when compiling this simulator does nothing.

This cannot be used by the serial simulator as beads are not sent via any form
of messaging system (this is not implemented in the serial simulator).

## Feature options

These are optional features providing various improvements either to the
simulation results, the performance or the capabilities of the simulator.

Unless stated, these can be used in combination with any of the previous macro
options on any simulator, or with any combination of each other.

If using a combination of these, they *should* be in the makefile in every
combination for ease of compilation.

### Bonds

Bonds are calculated as part of `force_update()`, and some checks are done to
make sure that beads are bonded, and then these bond forces are calculated.

Some examples, such as oil and water, do not use bonds so it is not necessary
to include this as it is wasted instructions. Removing the unnecessary code will
improve performance.

Vesicle self assembly however requires bond forces, so this macro flag is
included in the recipe automatically by including `-vesicle-`.

This is not included on its own in any recipes, but it can be included manually
to any simulation regardless of whether bonds are used or not.

### Improved GALS

**FLAG: `-DIMPROVED_GALS`** - **makefile: `-improvedgals-`**

Improved GALS can only be applied to the GALS simulator, but in combination with
any other flag. This improves upon the base GALS by reducing the number of
messages that are sent.

The first message sent by a cell in a phase is still sent and counted by
receiving cells, and the number of beads to be sent are still sent and counted
by receiving cells. The major change comes when the last bead a cell has is sent
(including if it is the only bead, making it the first and last bead). This is
encoded in the bead message itself, and the sending cell will move into the
phase complete stage. The receiving cell detects if this is the first, last or
first and last bead, and will update the counter for number of neighbours in the
phase complete phase.

The update complete and migrate complete messages are now **only** sent when a
cell has no beads to share or migrate. This is detected in the same way as
normal GALS, and will increment/decrement counters appropriately.

This improves performance of the GALS simulator greatly, and if the GALS
simulator is to be used it is strongly suggested that this macro flag is used.

### Improved Verlet

**FLAG: `-DBETTER_VERLET`** - **makefile: `-betterverlet-`**

The Verlet used by default is less accurate, and while it does correctly moves
bead, it is only to be considered *first-order*. It only takes into account the
current values for velocity.

Improved verlet keeps track of the previous timesteps velocity, and this is used
in the Verlet calculations to produce a new position which more accurately
reflects the previous momentum of the beads.

Overall this macro option provides improved accuracy of simulation, and keeps
the simulation temperature is more consistent. This does affect performance
somewhat, and the results will obviously be different than default Verlet, but
is not to be discounted due to the better resulting simulation.

### Using a smaller dt for early timesteps

**FLAG: `-DSMALL_DT_EARLY`** - **makefile: `-dtchange-`**

Beads are initially placed randomly. This means that some beads may be placed
unnaturally close, and the early forces between them will be massive. This can
lead to bonds "snapping" (where they get too far away for the bond force to be
calculated). The temperature will be very high initially, and beads can move
position in huge jumps each timestep, which takes a while to calm down.

If we reduce the set timestep, dt, by 10x for the first 1000 timesteps, we can
smooth this initial movement as beads gently move to more natrual distances from
each other. The timestep is then set back to the original value at timestep
1000.

This does not particularly affect performance, but helps keep the initial
temperature low and bonded beads are likely to stay within a reasonable range of
each other.

### Doubling square root accuracy

**FLAG: `-DDOUBLE_SQRT`** - **makefile: `-doublesqrt-`**

RISC-V does not have a built in square root, and square roots even on modern x86
machines are still quite computationally expensive. For this application we
provide our own sqaure root function that implements the
[Newton-Raphson sqaure root](https://en.wikipedia.org/wiki/Newton%27s_method).
This method calculates a square root by performing the same function over and
over again, the more times it performs it, the more accurate the square root is.

By default, we perform this function 3 times. This macro will double this to 6
iterations. Obviously the more we do this, the more accurate the square root
results will be, but at a cost of performance as we perform more floating
point operations overall.

This was initially added to see how much the floating-point performance affects
the overall performance. Doubling the square root is definitely a factor, but we
work hard to reduce the number of square roots needed, such as finding the
square Euclidean distance to determine if beads are within each others cut-off
radius. We only find the actual Euclidean distance if this is the case, as it is
used in the equations for all of the forces acting between beads, including
bonded beads.

Warning: Doubling the square root operations will **not** produce the same
results as without this macro flag, so needs a different expected results in
during testing.

### Message management

**FLAG: `-DMESSAGE_MANAGEMENT`** - **makefile: `-msgmgmt-`**

This macro affects the messaging of the POETS simulators, and therefore does not
work with the serial simulator which does not use messaging to communicate
between cells.

The idea of this was to reduce the strain on the messaging system of the
POETS Tinsel hardware. By default, cells try to send all their beads as fast as
possible, only stopping to handle incoming messages. This adds so much traffic
upfront to a phase, and can lead to many cells awaiting just to send a message.

Message management allows cells to send one message once they enter a phase, and
can only then send their next message after they have received **one** message
from each **active** neighbour.

An active neighbour is one who has not indicated that they have finished sending
all of their messages for the current phase. The last bead a cell sends will
include an identifier indicating this, and the receiving cells active neighbour
count will be decremented.

This feature aims to balance the messaging load to the cells who have been quiet
for longer. When Tinsel only sent using *unicast*, this provided quite a
substantial performance boost. However the latest versions of tinsel use
*multicast*, where some messages are grouped by key, based on the placement of
cells, so this actually causes a significant reduction in performance currently.
We keep this macro up to date incase it works better for some size of volume
(when we can reach much much larger sizes), or a new generation of hardware uses
a different type of messaging system.

### Floating point force accumulation

**FLAG: `-DFLOAT_ONLY`** - **makefile: `-floatonly-`**

Floating point addition is non-associative. This means that when we accumulate
forces in floating-point, the result depends on the order in which bead force
calculations were performed, something that is impossible to predict with the
POETS hardware. We therefore calculate a force acting on a bead in
floating-point, then convert it to a fixed point representation and accumulate
all forces using this. This way, the force accumulation is associative, and we
can get reproducible, and testable, results.

The float only option removes the fixed-point accumulation, instead accumulating
the forces in floating point. This does improve performance my a small degree,
but we can no longer test the results, or produce the exact same simulation
result again. This does not mean that the result is incorrect, just that it is
subject to increased randomness. The results may be similar, but never the same,
but the increased randomness can be a benefit to the simulator.

### Reduced local calculations

**FLAG: `-DREDUCE_LOCAL_CALCS`** - **makefile: `-reducelocalcalcs-`**

Consider a cell which has three local beads: `a`, `b` and `c`. Standard
`local_calcs()`, regardless of where it is performed, will start with `a` and
calculate its forces acting on `b` and `c`. Then, when it comes to `b` it will
calculate that beads forces on `a` and `c`, and finally with `c` it will
calculate the forces on `a` and `b`.

Forces acting between a pair of beads are equal and opposite. If we calculate a
force `f` acting on bead `b` by bead `a`, the force acting on `a` by `b` will be
`-f`. We can use this to perform fewer `force_update()` and, consequently, fewer
square roots to find the Euclidean distance.

Reduced local calculations modifies `local_calcs()` by calculating the force
acting on `b` by `a`, then adds the force to `b`s accumulated forces **and**
subtracts the same force from `a`s accumulated forces.

The macro also modifies the code to ensure we don't double up on forces. When we
come to calculate the forces `a` enacts on other local beads, we calculate it
against `b` and `c`. Then when we come to calulate `b`s forces on local beads,
we have already calculated the force of `b` acting on `a`, so we only calculate
the force `b` has on `c`. Therefore, when we come to perform local calculations
with `c`, we've calculated all the local pairwise forces involving it.

This does provide a small performance increase, however the majority of
`force_update()` calculations are performed based on neighbour beads, to which
this doesn't apply, as a cell cannot access its neighbours accumulated forces.
The results of doing this are exactly the same.

### Single force loop

**FLAG: `-DSINGLE_FORCE_LOOP`** - **makefile: `-singleforceloop-`**

By default, local calculations and calculations with neighbour beads are
performed separately using different loops. They are however laid out in very
similar ways: We take a bead `a` and calculate its affect on local beads.

This macro removes the neighbour and local calculations functions and replaces
them both with a single function `calc_bead_force_on_beads()`. Local
calculations pass the bead we're interested in currently to find its affect on
other local beads, and neighbour calculations pass the received bead to this
function. This works with the above reduced local calculations option.

This option reduces performance slightly, unfortunatley due to some generalising
that has to be done for a single loop for both of these. The benefit is however
that it reduces the size of the binaries, freeing up instruction space for new
features/improvements.

--------------------------------------------------------------------------------

## Some of the best combinations

There are many options above, and many combinations therefore. For the POETS
simulators (GALS/synchronous), we have limited space for instructions, and
currently many combinations can fit within this, but there is a trade-off to be
found for size of instructions vs performance.

Some of the options are strongly suggested to be included in everything, to
produce the best result, such as `-DBETTER_VERLET` and `-DSMALL_DT_EARLY`. These
also primarily use oil and water or vesicle self-assembly simulation examples,
as these are the examples which produce interesting results.

This section will suggest combinations for the fastest of each POETS simulator,
and the smallest in terms of total instructions.

### Fastest combination

The fastest combination for the POETS simulators includes:

- `sync-` or `gals-`, as these are the POETS simulators (not x86 serial).
- `-timed-` or `-visual-`, these are the most useful operational modes as they
provide us with performance information or states we can use for analysis.
- `-oilwater-` or `-vesicle-` as these are free flowing simulations, there are no
walls and provide us with an interesting result.
- `-onebyone-` provides the best performance boost with smooth scaling
- `-improvedgals-` if using the GALS simulator. It provides a considerable
performance improvement over the standard GALS.
- `-betterverlet` as it provides a more accurate result.
- `-dtchange-` because it smooths out the initial timesteps to reduce the
overall temperature of the volume more quickly
- `-floatonly-` if you do not mind about reproducible results. This provides a
nice speed up.
- `-reducelocalcalcs` as this provides a nice speed boost as well.

Specifically for the makefile here are some example recipes for either
simulator.

```bash
sync-timed-oilwater-onebyone-betterverlet-dtchange-floatonly-reducelocalcals

gals-visual-vesicle-onebyone-improvedgals-betterverlet-dtchange-reducelocalcalcs
```

### Smallest combination

The GALS version is much larger than the sync version because of the extra
instructions used to synchronise cells with their neighbours, but here are some
options to reduce the code size as much as possible.

- `sync-` or `gals-`, as these are the POETS simulators (not x86 serial).
- `-timed-` or `-visual-`, these are the most useful operational modes as they
provide us with performance information or states we can use for analysis.
- `-oilwater-` or `-vesicle-` as these are free flowing simulations, there are no
walls and provide us with an interesting result.
- `-sendtoself-` reduces lots of instructions for local calculations
- Don't include `-dtchange-` or `-betterverlet-` if accuracy is not a problem
- `floatonly` - This reduces instructions as well as improving performance, as
long as reproducibility is not required
- `singleforceloop` can be used if not using send to self, as this option
already removes the `local_calcs()` function.
