#include "common.h"

/*
 * Ticks the course's permanently looping prop and its positional sound. Two
 * keyframe tracks (position at D_801E42DC, rotation at D_801E42E8) eased
 * sinusoidally between waypoints; the sound is GameSetPitchedSoundCue cue 0 with
 * a Doppler-approximating pitch. See docs/names.md 1.
 */
INCLUDE_ASM("asm/PAL/main/nonmatchings/main/GameUpdatePathScenery", func_8003F9C4);

