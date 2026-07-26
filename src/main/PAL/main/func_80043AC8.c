#include "common.h"
#include "game/race.h"

extern void *D_8019C7CC;

void func_80043AC8(u8 *arg0, s32 arg1) {
    void *ptr;

    if (arg1 != 0) {
        if (g_GrandPrixSeries != 0) {
            ptr = arg0 + *(s32 *)(arg0 + 4);
        } else {
            ptr = arg0 + *(s32 *)(arg0 + 0);
        }
    } else {
        ptr = arg0 + *(s32 *)(arg0 + 8);
    }

    D_8019C7CC = ptr;
}
