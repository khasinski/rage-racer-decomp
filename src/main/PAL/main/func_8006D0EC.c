#include "common.h"

typedef struct {
    volatile u16 state;
    u8 pad2[0x1E];
} EventRecord;

extern s32 D_801E6C98;
extern s32 D_801E8278;
extern EventRecord *D_801E8AAC;
extern s32 D_801F1850;

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
        raw_base = D_801E8AAC;
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
