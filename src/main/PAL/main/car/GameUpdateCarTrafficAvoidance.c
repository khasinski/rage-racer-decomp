#include "common.h"
#include "game/car.h"
#include "game/state.h"
#include "game/track.h"
#include "game/race.h"

void GameUpdateRivalRubberBand(void) asm("func_8003A974");

extern u8 g_PlayerTrackProgress[] asm("D_8009E744");
extern s32 g_PlayerSpeed asm("D_8009E778");

/*
 * Inside the loop these two fields must NOT be reached as struct members.
 * gcc 2.6.3 tags a struct-member MEM with MEM_IN_STRUCT_P and then assumes it
 * cannot alias a plain global scalar, which lets it hoist the g_PlayerSpeed load
 * out of the loop into an extra callee-saved register. Retail reloads the
 * global on every iteration, so the in-loop writes stay raw casts.
 */
#define AVOID_BLOCKED(w) (*(s16 *)((u8 *)(w) + 0x48)) /* ->field_104 */
#define AVOID_NEARBY(w) (*(u16 *)((u8 *)(w) + 0x50))  /* ->field_10C */

void GameUpdateCarTrafficAvoidance(GameCarRuntime *car, s32 arg1) asm("func_8003A280");
void GameUpdateCarTrafficAvoidance(GameCarRuntime *car, s32 arg1) {
    GameCarAiBlock *state = (GameCarAiBlock *)&car->field_BC;
    s32 acc8 = 0;
    s32 acc9 = 0;
    /* This pin is load-bearing: removing it changes .text. */
    register s32 i asm("$10") = 0;
    s32 k11 = 0xB;
    u8 *base = g_PlayerTrackProgress;
    s32 carProgress;
    s32 carField34;
    s32 carA4low;
    /* These pins are load-bearing: removing any one changes .text. */
    register u8 *block asm("$11");
    register s32 track asm("$12");
    s32 t6;
    s32 field34minus;
    s32 field34plus;
    s32 trackMinus;
    s32 total;
    s32 sums[4];

    carProgress = car->trackProgress;
    carField34 = car->field_34;
    carA4low = (u16)car->field_A4;
    block = (u8 *)&g_Cars[0].field_A4;
    car->field_120 = 0;
    car->field_10C = 0;
    sums[3] = 0;
    sums[2] = 0;
    sums[1] = 0;
    sums[0] = 0;
    track = g_TrackLength;
    {
        s32 tmp = car->field_A4 << 1;
        t6 = tmp + 0xC00;
    }
    field34minus = (s16)(carField34 - 0x30);
    field34plus = (s16)(carField34 + 0x30);
    trackMinus = track - 0x400;

    for (; i < 12; i++, block += sizeof(GameCarRuntime)) {
        s32 otherField34;
        s32 otherA4;
        s32 a2;
        /* These pins are load-bearing: removing any one changes .text. */
        register s32 t1 asm("$9");
        s32 diff;
        register s32 angleDiff asm("$3");
        s32 angleSaved;
        s16 bucket;
        s32 val;

        if (g_SceneId != 0xC && i == k11) {
            break;
        }
        if (i == arg1) {
            continue;
        }
        if (*(s16 *)(block + 8) == -1) { /* g_Cars[i].activeFlag */
            continue;
        }

        if (i == k11) {
            s32 op = *(s32 *)(base + 0);
            a2 = op + track;
            otherField34 = *(s32 *)(base - 0x3C);
            if (arg1 >= 4) {
                goto loadA4;
            }
            otherA4 = 0;
            goto afterA4;
        loadA4:
            otherA4 = *(u16 *)(base + 0x34);
        afterA4:
            t1 = 0;
            t6 = 0x1800 - (g_PlayerSpeed << 1);
        } else {
            s32 op;
            otherField34 = *(s32 *)(block - 0x70); /* g_Cars[i].field_34 */
            otherA4 = *(u16 *)block; /* g_Cars[i].field_A4, low half */
            op = *(s32 *)(block - 0x34); /* g_Cars[i].trackProgress */
            a2 = op + track;
            t1 = *(u16 *)&AVOID_BLOCKED(state); /* unsigned load of ->field_104 */
        }

        diff = (a2 - carProgress) % track;
        angleDiff = otherField34 - carField34;
        angleSaved = angleDiff;
        __asm__("" : "=r"(angleSaved) : "0"(angleSaved));
        otherA4 -= carA4low;

        if (diff > 0 && diff < t6) {
            AVOID_NEARBY(state)++;
            if (field34minus < otherField34 && otherField34 < field34plus) {
                if (!((s16)otherA4 > 0 && t1 == 0)) {
                    AVOID_BLOCKED(state) = 1;
                    val = angleDiff + 0x30;
                    if (val < 0) {
                        val = angleDiff + 0x4F;
                    }
                    bucket = val >> 5;
                    if (i == k11) {
                        if (diff < 0xC00) {
                            s32 s = sums[bucket] + 0xC00;
                            sums[bucket] = s - diff;
                        } else {
                            sums[bucket] = sums[bucket] + 0xC00;
                        }
                    } else {
                        sums[bucket] = (t6 - diff) + sums[bucket];
                    }
                }
            }
            if (diff < 0x200) {
                s32 ad = (s16)angleSaved;
                if (ad >= 0x41) {
                    s32 s = acc9 + 0xC00;
                    acc9 = s - diff;
                } else if (ad < -0x40) {
                    s32 s = acc8 + 0xC00;
                    acc8 = s - diff;
                }
            }
        } else {
            if (trackMinus < diff) {
                AVOID_NEARBY(state)++;
            }
        }
    }

    total = sums[0] + sums[1] + sums[2];
    sums[3] = total;
    if (total > 0) {
        if (acc8 == 0 && carField34 >= 0x51) {
            s32 f104 = state->field_104;
            state->field_11E = -0x50;
            state->field_120 = -8 - (f104 << 1);
        } else if (acc9 == 0 && carField34 < -0x50) {
            s32 f104 = state->field_104;
            state->field_11E = 0x50;
            state->field_120 = (f104 << 1) + 8;
        } else if (sums[0] <= sums[1] && sums[0] <= sums[2] && acc8 == 0) {
            s32 f104 = state->field_104;
            state->field_11E = -0x50;
            state->field_120 = -6 - (f104 << 1);
        } else if (sums[2] <= sums[1] && sums[2] <= sums[0] && acc9 == 0) {
            s32 f104 = state->field_104;
            state->field_11E = 0x50;
            state->field_120 = (f104 << 1) + 6;
        }
        __asm__("" : : "r"(carField34));
        if (sums[3] >= 0x3E9) {
            s32 fv = state->field_130;
            s32 d = ((fv << 4) - fv) << 1;
            state->field_130 = d / 100;
        }
    } else {
        state->field_120 = 0;
        state->field_104 = 0;
        state->field_11E = state->field_11C;
    }

    state->field_11C = state->field_11C + state->field_120;
}

/*
 * g_RankedCars - 1: this walker is indexed from the slot before the leader,
 * so the byte at 0x801E40B8 itself is never loaded. It stays raw because that
 * address already has a name for what actually lives there -- g_SceneTimer in
 * game/state.h -- and aliasing it a second time would be misleading.
 */
extern GameCarRuntime *D_801E40B8[];

void GameSlowRivalAhead(GameCarRuntime *arg0, s32 arg1) asm("func_8003A6A4");
void GameSlowRivalAhead(GameCarRuntime *arg0, s32 arg1) {
    GameCarRuntime *entry;
    s32 offset;
    s32 pos0Base;
    s32 pos0;
    s32 pos1;
    s32 value;

    offset = arg1 << 2;
    pos0Base = arg0->field_68;
    entry = *(GameCarRuntime **)((u8 *)D_801E40B8 + offset);
    pos0 = pos0Base + arg0->field_6C;
    pos1 = entry->field_68 + entry->field_6C;

    if ((pos1 - pos0) < 0x2800) {
        return;
    }

    if (entry->field_A4 >= 0x385) {
        value = entry->field_130;
        value = ((value * 5) + ((value * 5) << 4)) / 100;
        entry->field_130 = value;
    }
}

/* Base of the per-car progress pair, walked at the 0x19C GameCarRuntime
 * stride: g_Cars[0].field_68 / .field_6C, whose sum is race progress. */
extern s32 g_CarProgressA asm("D_801F18BC");
extern s32 g_CarProgressB asm("D_801F18C0");

/*
 * Ranks the first four cars by race progress (`field_68 + field_6C`) and
 * publishes the ordering into g_RankedCars: slot 0 the leader, slot 3 the
 * last of the four, slots 1/2 the middle pair in order. GameUpdateRivalRubberBand reads
 * the result to rubber-band the AI.
 */
void GameRankContenders(void) asm("func_8003A728");
void GameRankContenders(void) {
    s32 i;
    s32 offset;
    s32 maxValue;
    s32 minValue;
    s32 value;
    s32 sums[4];
    s32 *sumPtr;
    s16 indices[4];

    i = 0;
    sumPtr = sums;
    offset = 0;
    do {
        *sumPtr = *(s32 *)((u8 *)&g_CarProgressA + offset) + *(s32 *)((u8 *)&g_CarProgressB + offset);
        offset += sizeof(GameCarRuntime);
        i++;
        sumPtr++;
    } while (i < 4);

    indices[0] = 0;
    indices[3] = 0;
    maxValue = sums[0];
    minValue = sums[0];
    for (i = 1; i < 4; i++) {
        value = sums[i];
        if (maxValue < value) {
            maxValue = value;
            indices[0] = i;
        } else if (value < minValue) {
            minValue = value;
            indices[3] = i;
        }
    }

    g_RankedCars[0] = &g_Cars[indices[0]];
    g_RankedCars[3] = &g_Cars[indices[3]];

    for (i = 0; i < 4; i++) {
        if ((i != indices[0]) && (i != indices[3])) {
            indices[1] = i;
            break;
        }
    }

    for (i = 0; i < 4; i++) {
        if ((i != indices[0]) && (i != indices[3]) && (i != indices[1])) {
            indices[2] = i;
            break;
        }
    }

    if (sums[indices[1]] > sums[indices[2]]) {
        g_RankedCars[1] = &g_Cars[indices[1]];
        g_RankedCars[2] = &g_Cars[indices[2]];
    } else {
        g_RankedCars[1] = &g_Cars[indices[2]];
        g_RankedCars[2] = &g_Cars[indices[1]];
    }
}

extern s32 g_PlayerProgressA asm("D_8009E73C");
extern s32 g_PlayerProgressB asm("D_8009E740");
extern s32 g_RivalCueFlags asm("D_801E4BB4");
extern s32 g_ClosestRivalRank asm("D_801E7740");
extern s16 g_RivalCueCooldown3 asm("D_801E6F26");

void GamePlaySoundCue(s32 cue) asm("func_8005D6EC");

void GameUpdateRivalRubberBand(void) asm("func_8003A974");
void GameUpdateRivalRubberBand(void) {
    s32 s6;
    s32 s5;
    s32 s4;
    s32 s3;
    s16 *s2;
    s32 s1;
    s32 s0;

    s6 = g_PlayerProgressA + g_PlayerProgressB;
    if ((g_CourseIndex & 3) == 3) {
        s5 = 0xC00;
        s4 = 0x1400;
    } else {
        s5 = 0x600;
        s4 = 0xE00;
    }

    if (g_RacePhase >= 4) {
        return;
    }

    s1 = 3;
    s3 = 0x20;
    s2 = &g_RivalCueCooldown3;
    s0 = 4;

    do {
        s32 a0 = g_RankedCars[s1]->field_68 + g_RankedCars[s1]->field_6C - s6;

        if (a0 >= 0) {
            if (s1 == 0) {
                g_RivalCueFlags &= ~1;
            }
            g_ClosestRivalRank = s1;
            if (s4 < a0) {
                g_RivalCueFlags &= ~(0x200 >> s0);
                if (g_RankedCars[s1]->field_A4 >= 0x321) {
                    g_RankedCars[s1]->field_130 = g_RankedCars[s1]->field_130 * 90 / 100;
                }
                return;
            }
            if (s5 < a0) {
                s32 counter;

                if (g_RankedCars[s1]->field_A4 >= 0x3E9) {
                    g_RankedCars[s1]->field_130 = g_RankedCars[s1]->field_130 * 98 / 100;
                }
                counter = *s2;
                g_RivalCueFlags |= (s3 >> s0);
                if (counter >= 0x12D) {
                    *s2 = 0;
                }
                return;
            }
            if (!((0x200 >> s0) & g_RivalCueFlags)) {
                s32 bit;
                s32 flags;

                switch ((u32)g_SceneTimer % 3) {
                case 1:
                    goto cue_33;
                case 0:
                    goto cue_32;
                case 2:
                    goto cue_34;
                }
cue_32:
                if (g_RivalCueEnabled != 0) {
                    GamePlaySoundCue(0x32);
                }
                bit = 0x200;
                goto cue_done;
cue_33:
                if (g_RivalCueEnabled != 0) {
                    GamePlaySoundCue(0x33);
                }
                bit = 0x200;
                goto cue_done;
cue_34:
                if (g_RivalCueEnabled != 0) {
                    GamePlaySoundCue(0x34);
                }
                bit = 0x200;
cue_done:
                flags = g_RivalCueFlags;
                *s2 = 0;
                g_RivalCueFlags = (bit >> s0) | flags;
                return;
            }
            (*(u16 *)s2)++;
            return;
        } else {
            if (s1 == 0 && !(g_RivalCueFlags & 1) && a0 < -0x1C00) {
                if (g_RivalCueEnabled != 0 && g_RacePosition == 1) {
                    GamePlaySoundCue(0x2D);
                }
                g_RivalCueFlags = (g_RivalCueFlags & ~0x10) | 1;
            } else if (a0 >= -0x7FF && !((s3 >> s0) & g_RivalCueFlags)) {
                if (g_SceneTimer & 1) {
                    if (g_RivalCueEnabled != 0) GamePlaySoundCue(0x2F);
                } else {
                    if (g_RivalCueEnabled != 0) GamePlaySoundCue(0x30);
                }
                g_RivalCueFlags |= (s3 >> s0);
            } else {
                if (a0 < -0x1000) {
                    g_RivalCueFlags &= ~(s3 >> s0);
                } else if (a0 < -0x800) {
                    if (*s2 >= 0x12D) {
                        if (g_SceneTimer & 1) {
                            if (g_RivalCueEnabled != 0) GamePlaySoundCue(0x37);
                        } else {
                            if (g_RivalCueEnabled != 0) GamePlaySoundCue(0x36);
                        }
                        *s2 = 0;
                    }
                }
            }
            s2--;
            s0--;
            s1--;
        }
    } while (s1 >= 0);
}
