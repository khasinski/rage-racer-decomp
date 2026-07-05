#include "common.h"
#include "game/audio.h"

extern s16 D_80082F28[];

s32 GameSetSoundToneTableEntry(s32 arg0, s32 arg1, s32 arg2) {
    s16 *base = D_80082F28;
    s16 *row;
    register s16 *entry asm("$5");
    s32 old;

    row = (s16 *)((s32)base + (arg0 << 2));
    entry = (s16 *)((arg1 << 1) + (s32)row);
    __asm__("" : "=r"(entry) : "0"(entry));
    old = *entry;

    if (arg2 >= 0) {
        *entry = arg2;
    }
    return old;
}
