// This file contains the constants to be used by a simulator.
// Some are the same for any simulation.
// Some are different based on the type of simulation, i.e. Oil and water
// or Vesicle self-assembly. These are chosen by compiler flags.

/********************* DEFINES **************************/

#define DT10_RAND_MAX 4294967295

/********************* DPD CONSTANTS **************************/

// Cut-off radius
const ptype r_c(1.0);

// Square of cutoff radius (to reduce number of sqrt)
const ptype sq_r_c(r_c * r_c);

// Interaction matrix for water and two types of oil
#ifdef VESICLE_SELF_ASSEMBLY
const ptype A[4][4] = {  {ptype(25.0), ptype(50.0), ptype(20.0), ptype(50.0)},
                         {ptype(50.0), ptype(25.0), ptype(75.0), ptype(25.0)},
                         {ptype(20.0), ptype(75.0), ptype(25.0), ptype(75.0)},
                         {ptype(50.0), ptype(25.0), ptype(75.0), ptype(25.0)}
                      };
#else // OIL AND WATER
const ptype A[3][3] = {  {ptype(25.0), ptype(75.0), ptype(35.0)},
                         {ptype(75.0), ptype(25.0), ptype(50.0)},
                         {ptype(35.0), ptype(50.0), ptype(25.0)}
                      };
#endif

// Drag coefficient
const ptype drag_coef = 4.5;

// Random coefficient. Related to drag_coef and temperature - sigma = sqrt(2 * drag_coef * temp)
const ptype sigma_ij = 3;

// Mass of all beads
// const ptype p_mass = 1.0;

// Lambda used in verlet
const ptype lambda = 0.5;

/********************* BOND CONSTANTS **************************/

#ifdef BONDS
#ifdef VESICLE_SELF_ASSEMBLY
const ptype bond_r0 = 0.3;
const ptype bond_kappa = 256.0;
#else // Standard bonds
// 0.5 is the distance aimed to be kept between two bonded beads
const ptype bond_r0 = 0.5;
// Bond_kappa is the force between two bonded beads. It is very strong
const ptype bond_kappa = 128.0;
#endif
#endif
