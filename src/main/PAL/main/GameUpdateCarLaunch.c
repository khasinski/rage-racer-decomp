#include "common.h"

/*
 * Car motion-state handler for state98 == 1: the one-frame takeoff of a jump.
 * Turns the launch spin func_8002F690 seeded into clamped yaw, recomputes revs /
 * tacho / world velocity, then sets route+0x38 = 0x14 and route+0x98 = 2 to hand
 * the car to the airborne handler func_80030814. See docs/names.md 1.
 *
 * PERMUTER-CANDIDATE: real C exists (scratch/permuter-seeds/func_80030030.wip.c,
 * DIFFS=68), 505/505 words, structure correct. Residual is s0/s2 CSE +
 * scheduling. Byte-exact INCLUDE_ASM until permuter.
 */

INCLUDE_ASM("asm/nonmatchings/PAL/main", func_80030030);
