// This file contains the constants to be used by a simulator.
// Some are the same for any simulation.
// Some are different based on the type of simulation, i.e. Oil and water
// or Vesicle self-assembly. These are chosen by compiler flags.

#ifndef DPD_CONSTANTS_H
#define DPD_CONSTANTS_H

/********************* Typedefs **************************/
typedef float ptype;

/********************* DEFINES **************************/

#define DT10_RAND_MAX 4294967295

#define UNIT_SPACE 1.0 // a cube 1.0 x 1.0 x 1.0
#define PADDING 0

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#ifdef GALS
  #define UPDATE 0
  #define UPDATE_COMPLETE 1
  #define MIGRATION 2
  #define MIGRATION_COMPLETE 3
  #define EMIT 4
  #define EMIT_COMPLETE 5
  #define START 6
  #define END 7
#elif !defined(XML) // SYNC or SERIAL
  #define UPDATE 0
  #define MIGRATION 1
  #if defined(VISUALISE) || defined(TESTING)
  #define EMIT 2
  #endif
#endif

/********************* SIMULATION PARAMETERS *********************/

// Timestep and inverse sqrt of timestep
#ifndef SMALL_DT_EARLY
const ptype dt = 0.02;
// Inverse square root of dt - dt^(-1/2)
const ptype inv_sqrt_dt = 7.071067812;
#else
const ptype normal_dt = 0.02;
const ptype early_dt = 0.002;
// Inverse square root of dt - dt^(-1/2)
const ptype normal_inv_sqrt_dt = 7.071067812;
const ptype early_inv_sqrt_dt = 22.360679775;
#endif

#ifdef VISUALISE
const uint32_t emitperiod = 1;
#endif

#ifdef SERIAL
const uint8_t NEIGHBOURS = 26;
#elif defined(GALS)
 #ifndef ONE_BY_ONE
    const uint8_t NEIGHBOURS = 27;
 #else
    const uint8_t NEIGHBOURS = 26;
 #endif
#else
 #ifdef MESSAGE_MANAGEMENT
   #ifndef SEND_TO_SELF
    const uint8_t NEIGHBOURS = 26;
   #else
    const uint8_t NEIGHBOURS = 27;
   #endif
 #endif
#endif

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
#elif defined(GRAVITY)
const ptype A[4][4] = {  {ptype(25.0), ptype(75.0), ptype(35.0), ptype(75.0)},
                         {ptype(75.0), ptype(25.0), ptype(50.0), ptype(75.0)},
                         {ptype(35.0), ptype(50.0), ptype(25.0), ptype(75.0)},
                         {ptype(75.0), ptype(75.0), ptype(75.0), ptype(0)}
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

#endif // DPD_CONSTANTS_H
