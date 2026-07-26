#include "common.h"
#include "game/car.h"
#include "game/state.h"
#include "game/track.h"

extern u8 D_8009E744[];
extern s32 D_8009E778;

/*
 * Inside the loop these two fields must NOT be reached as struct members.
 * gcc 2.6.3 tags a struct-member MEM with MEM_IN_STRUCT_P and then assumes it
 * cannot alias a plain global scalar, which lets it hoist the D_8009E778 load
 * out of the loop into an extra callee-saved register. Retail reloads the
 * global on every iteration, so the in-loop writes stay raw casts.
 */
#define AVOID_BLOCKED(w) (*(s16 *)((u8 *)(w) + 0x48)) /* ->field_104 */
#define AVOID_NEARBY(w) (*(u16 *)((u8 *)(w) + 0x50))  /* ->field_10C */

void func_8003A280(GameCarRuntime *car, s32 arg1) {
    register GameCarAiBlock *state asm("$8") = (GameCarAiBlock *)&car->field_BC;
    register s32 acc8 asm("$24") = 0;
    register s32 acc9 asm("$25") = 0;
    register s32 i asm("$10") = 0;
    register s32 k11 asm("$17") = 0xB;
    register u8 *base asm("$16") = D_8009E744;
    register s32 carProgress asm("$22");
    register s32 carField34 asm("$15");
    register s32 carA4low asm("$21");
    register u8 *block asm("$11");
    register s32 track asm("$12");
    s32 t6;
    s32 field34minus;
    s32 field34plus;
    register s32 trackMinus asm("$18");
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
            register s32 op asm("$2") = *(s32 *)(base + 0);
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
            t6 = 0x1800 - (D_8009E778 << 1);
        } else {
            register s32 op asm("$2");
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
                    register s32 s asm("$2") = acc9 + 0xC00;
                    acc9 = s - diff;
                } else if (ad < -0x40) {
                    register s32 s asm("$2") = acc8 + 0xC00;
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
            register s32 fv asm("$3") = state->field_130;
            register s32 d asm("$2") = ((fv << 4) - fv) << 1;
            state->field_130 = d / 100;
        }
    } else {
        state->field_120 = 0;
        state->field_104 = 0;
        state->field_11E = state->field_11C;
    }

    state->field_11C = state->field_11C + state->field_120;
}
