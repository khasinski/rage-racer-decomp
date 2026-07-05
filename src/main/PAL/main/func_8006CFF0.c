#include "common.h"
#include "psyq/cd.h"

extern s32 D_801F1850;
extern s32 D_801E6C98;
extern StHEADER_RR *D_801E8AAC;
extern s32 D_801E8278;
extern s32 D_801E8A94;
extern s32 D_801E3E10;

typedef struct {
    volatile u16 state;
    u8 pad2[0x1E];
} EventRecord;

typedef struct {
    s32 unk0;
    u8 pad[0x1C];
} UnkEventRecord;

u32 StFreeRing(u32 *base) {
    s32 temp_a1;
    s32 i;
    s16 nSectors;
    StHEADER_RR *temp_v0;
    StHEADER_RR *temp_v0_2;

    temp_a1 = (base - (u32 *)&D_801E8AAC[D_801F1850]) / 504;
    temp_v0 = &D_801E8AAC[temp_a1];
    nSectors = D_801E8AAC[temp_a1].nSectors;
    if ((s16)temp_v0->id != 4) {
        return 1;
    }

    for (i = 0; i < nSectors;) {
        temp_v0_2 = &D_801E8AAC[i + temp_a1];
        i++;
        *(s16 *)temp_v0_2 = 0;
    }

    D_801E6C98 = i + temp_a1;
    return 0;
}

void func_8006D0AC(s32 arg0, u32 arg1) {
    u32 i;

    for (i = 0; i < arg1; i++) {
        asm("" ::: "memory");
        ((UnkEventRecord *)D_801E8AAC)[i + arg0].unk0 = 0;
    }
}

s32 func_8006D0EC(EventRecord **arg0, EventRecord **arg1) {
    register EventRecord **out0 asm("$7") = arg0;
    register EventRecord **out1 asm("$8") = arg1;
    EventRecord *entry;
    s32 old_flag;

    entry = (EventRecord *)((D_801E6C98 << 5) + (s32)D_801E8AAC);

    if ((entry->state & 0xFFFF) == 1) {
        old_flag = D_801E8278;
        D_801E6C98 = 0;
        if (old_flag != 0) {
            entry->state = 0;
        }

        entry = (EventRecord *)((D_801E6C98 << 5) + (s32)D_801E8AAC);
    }

    if ((entry->state & 0xFFFF) == 2) {
        register s32 track asm("$4");
        register EventRecord *raw_base asm("$3");
        register s32 index asm("$5");
        register EventRecord *base asm("$4");
        register s32 offset asm("$3");

        entry->state = 4;
        asm("" ::: "memory");
        track = D_801F1850;
        raw_base = (EventRecord *)D_801E8AAC;
        index = D_801E6C98;
        base = (EventRecord *)((track << 5) + (s32)raw_base);
        offset = (index << 6) - index;
        offset <<= 5;
        base = (EventRecord *)((s32)base + offset);
        *out0 = base;
        *out1 = entry;
        return 0;
    }

    return 1;
}

void func_8006D1B0(s32 arg0, s32 arg1, s32 arg2) {
    D_801E8A94 = arg0;
    D_801E3E10 = arg1;
    D_801E8278 = arg2;
}
