#include "common.h"
#include "game/car.h"

extern s32 D_801E408C;
extern s32 D_801E40D8;
extern u8 *D_801E4150;

s32 func_80039184(GameCarRuntime *arg0) {
    register u8 *base asm("t2");
    register s32 pos0 asm("a1");
    register s32 pos1 asm("t0");
    register s32 row asm("v1");
    register s32 temp asm("t1");
    register s32 crossed asm("t3");
    register s32 i asm("a2");
    register s32 offset asm("a3");
    register s32 sentinel asm("t4");
    register s32 cursor asm("a1");
    register s32 diff asm("v0");
    register s32 cmp asm("v0");
    register s32 threshold asm("a1");
    register s32 resultOffset asm("v0");

    base = D_801E4150;
    if (arg0->field_A4 < 0x320) {
        return 0;
    }

    pos0 = arg0->trackProgress;
    pos1 = arg0->previousTrackProgress;
    row = arg0->routeRow;

    if (D_801E408C != 0) {
        pos0 = D_801E40D8 - pos0;
        pos1 = D_801E40D8 - pos1;
    }

    if (pos1 < pos0) {
        temp = pos0;
        diff = temp - pos1;
        goto ordered;
    }
    goto not_crossed;

crossed_label:
    crossed = 1;
    goto finish;

not_crossed:
    temp = pos1;
    pos1 = pos0;
    diff = temp - pos1;

ordered:
    if (diff >= 0x1000) {
        temp = 0;
        pos1 = 0;
    }

    crossed = 0;
    i = 0;
    sentinel = -1;
    offset = row << 6;
    cursor = (s32)(base + offset);

loop:
    if (*(s32 *)(cursor + 8) == sentinel) {
        goto finish;
    }
    threshold = *(s32 *)(cursor + 4);
    cmp = temp < threshold;
    offset += 8;
    if (cmp != 0) {
        goto next;
    }
    cmp = pos1 < threshold;
    if (cmp != 0) {
        goto crossed_label;
    }

next:
    i++;
    if (i < 8) {
        cursor = (s32)(base + offset);
        goto loop;
    }

finish:
    if (crossed != 0) {
        resultOffset = i << 3;
        resultOffset += row << 6;
        resultOffset = (s32)(base + resultOffset);
        return *(s32 *)(resultOffset + 8);
    }
    return 0;
}
