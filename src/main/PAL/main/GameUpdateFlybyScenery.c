#include "common.h"

/*
 * Ticks the course's one scripted airborne prop and its engine sound; the model
 * is submitted elsewhere. Armed by a scene-counter match, then runs 451 frames,
 * integrating position from a keyframed heading and feeding a distance-attenuated
 * volume to GameSetPitchedSoundCue. See docs/names.md 1.
 */
INCLUDE_ASM("asm/PAL/main/nonmatchings/main/GameUpdateFlybyScenery", func_8003E590);

