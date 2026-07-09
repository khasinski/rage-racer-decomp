/* MASPSX_FLAGS: --addiu-branch-delay */
#include "common.h"

typedef struct TrackZone {
    s32 start;
    s32 end;
    s16 code;
    s16 value;
} TrackZone;

extern s32 D_801E408C;
extern s32 D_801E40D8;
extern u8 *D_801E4150;
extern s16 D_8019CE34;
extern s16 D_8019C78C;
extern s16 D_8019CAB0;

s32 func_800350B4(s32 position) {
    register u8 *base asm("$2");
    register s32 scene asm("$3");
    register TrackZone *first asm("$9");
    register TrackZone *zone asm("$6");
    register s32 status asm("$5");
    register s32 two asm("$10");
    register s32 start asm("$7");
    register s32 finish asm("$8");
    register s32 code asm("$3");
    register u16 rawCode asm("$2");

    base = D_801E4150;
    scene = D_801E408C;
    first = (TrackZone *)(base + 0xA74);
    if (scene != 0) {
        position = D_801E40D8 - position;
    }

    status = 0;
    two = 2;
    zone = first;
    D_8019CE34 = 0;
    D_8019C78C = 0;
    D_8019CAB0 = 0;

loop:
    start = zone->start;
    finish = zone->end;
    if (start == -1) {
        goto done;
    }

    if ((start < position) && (position < finish)) {
        if (position < start + 0x100) {
            status = 1;
        } else if (finish - 0x100 < position) {
            status = 2;
        } else {
            status = 3;
        }

        rawCode = zone->code;
        D_8019CE34 = rawCode;
        asm("" : : : "memory");
        code = (s16)rawCode;
        if (code == 0) {
            goto code_zero;
        }
        if (code > 0) {
            goto code_positive;
        }
        if (code == -3) {
            goto code_minus_three;
        }
        goto normalize_code;

code_positive:
        if (code == two) {
            goto code_two;
        }
        goto normalize_code;

code_zero:
        D_8019CAB0 = 3;
        goto store_value;

code_two:
        status = 4;
code_minus_three:
        if (status == two) {
            status = 3;
        }
        D_8019CE34 = 1;
        goto store_value;

normalize_code:
        if (D_8019CE34 < 0) {
            D_8019CE34 = -D_8019CE34;
            status = 3;
        }
store_value:
        D_8019C78C = zone->value;
    }

    {
        register TrackZone *end asm("$2");

        asm("" : "=r"(end) : "0"((TrackZone *)((u8 *)first + 0xF0)));
        if (status > 0) {
            goto done;
        }
        if ((s32)(zone + 1) < (s32)end) {
            zone++;
            goto loop;
        }
        zone++;
    }

done:
    switch (status) {
    case 1:
        return position - start;
    case 2:
        return finish - position;
    case 3:
        return 0x100;
    default:
        return 0;
    }
}
