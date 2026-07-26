#include "common.h"
#include "game/car.h"

/* g_RankedCars - 1: this walker is indexed from the slot before the leader. */
extern GameCarRuntime *D_801E40B8[];

void func_8003A6A4(GameCarRuntime *arg0, s32 arg1) {
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
