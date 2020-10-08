// BeadMap contains functions that deal with arrays of beads and the bitmaps
// which represent these. This has been set up to separate the manipulation
// from the calculations of forces for DPD, and the synchronisation
// needed for POETS.

#ifndef __BEADMAP_H
#define __BEADMAP_H

#define MAX_BEADS 31

// Given a bitmap and a position in that, clear this slot (set to 0)
// This will indicate that this position in the bead array is empty
inline uint32_t clear_slot(uint32_t bitmap, uint8_t pos);

// Given a bitmap and a position in that, set this slot (set to 1)
// This will indicate that this position in the bead array contains a bead
inline uint32_t set_slot(uint32_t bitmap, uint8_t pos);

// Given a bitmap and a position in that, report whether there is a bead in
// this slot
inline bool is_slot_set(uint32_t bitmap, uint8_t pos);

// Given a bitmap, return the position to the next occupied slot
inline uint8_t get_next_slot(uint32_t bitmap);

// Given a bitmap, return the position of the next empty slot
inline uint8_t get_next_free_slot(uint32_t bitmap);

// Given a bitmap, print the occupancy to the screen
inline void print_bitmap(uint32_t bitmap);

// Given a bitmap, count the number of occupied slots and therefore, the number
// of beads this bitmap represents
inline uint8_t get_num_beads(uint32_t bitmap);

#include "../src/BeadMap.cpp"

#endif /* _BEADMAP_H */
