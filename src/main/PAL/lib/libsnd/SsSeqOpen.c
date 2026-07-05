#include "common.h"

extern s32 D_801E4B80;
extern char D_80013C10[];

void func_8001674C(char *arg0);
s32 func_8006ECDC(s32 arg0, s32 arg1, s32 arg2);

s32 SsSeqOpen(s32 seq_data, s32 vab_id) asm("func_8006F004");

s32 SsSeqOpen(s32 seq_data, s32 vab_id) {
    s32 used;
    s32 i;
    s32 found;
    s32 slot;

    used = D_801E4B80;
    if (used == -1) {
        func_8001674C(D_80013C10);
        return -1;
    }

    i = 0;
    found = 0;
    do {
        if (((1 << i) & used) == 0) {
            slot = i;
            found = 1;
        }
        i++;
    } while ((u8)found == 0);

    D_801E4B80 = (1 << (s16)slot) | D_801E4B80;

    if ((s16)func_8006ECDC((s16)slot, (s16)vab_id, seq_data) == -1) {
        return -1;
    }
    return (s16)slot;
}
