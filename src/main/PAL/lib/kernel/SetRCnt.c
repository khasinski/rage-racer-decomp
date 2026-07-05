#include "psyq/kernel.h"

extern volatile u16 *D_8009A574;
extern volatile u32 *D_8009A570;
extern u32 D_8009A578[];

s32 SetRCnt(s32 arg0, s32 arg1, s32 arg2) {
    register s32 index asm("$8") = arg0 & 0xFFFF;
    register u16 flags asm("$7") = 0x48;
    register s32 offset asm("$3");
    register volatile u16 *entry asm("$3");
    register s32 base_v0 asm("$2");
    register s32 ret asm("$2");
    register s32 base asm("$4");
    s32 small;

    if (index >= 3) {
        return 0;
    }

    base_v0 = (s32)D_8009A574;
    offset = index << 4;
    entry = (volatile u16 *)(offset + base_v0);
    small = (u32)index < 2U;
    entry[2] = 0;
    entry[4] = arg1;

    if (small) {
        if (arg2 & 0x10) {
            flags = 0x49;
        }
        if (!(arg2 & 1)) {
            flags |= 0x100;
        }
    } else if (index == 2) {
        if (!(arg2 & 1)) {
            flags = 0x248;
        }
    }

    offset = index << 4;
    if (arg2 & 0x1000) {
        flags |= 0x10;
    }

    ret = 1;
    asm("" : "=r"(ret) : "0"(ret));
    base = (s32)D_8009A574;
    asm("nop");
    entry = (volatile u16 *)(offset + base);
    entry[2] = flags;
    return ret;
}

s32 GetRCnt(s32 arg0) {
    s32 index;

    index = arg0 & 0xFFFF;
    if (index >= 3) {
        return 0;
    }
    return D_8009A574[index * 8];
}

s32 StartRCnt(s32 arg0) {
    s32 index;

    index = arg0 & 0xFFFF;
    D_8009A570[1] |= D_8009A578[index];
    return index < 3;
}

s32 StopRCnt(s32 arg0) {
    s32 index;

    index = arg0 & 0xFFFF;
    D_8009A570[1] = ~D_8009A578[index] & D_8009A570[1];
    asm volatile("" ::: "memory");
    return 1;
}

s32 ResetRCnt(s32 arg0) {
    s32 index;

    index = arg0 & 0xFFFF;
    if (index >= 3) {
        return 0;
    }
    D_8009A574[index * 8] = 0;
    return 1;
}
