#include "common.h"
#include "game/car.h"
#include "game/race.h"

extern s32 D_8009B33C;
extern s32 D_8019C7B8;

s32 func_8001785C(s32 arg0);

void func_80055524(void) {
    s32 index;
    s32 value;
    GameCarEntry *ptr;
    GameCarEntry *enabledPtr;
    s32 byte;
    s32 enabledBase;

    D_8019C7B8 = -1;

    if (D_8009B33C != 0) {
        index = 12;
        enabledBase = (s32)g_CarTable;
        enabledPtr = (GameCarEntry *)(enabledBase + 0x60);
loop_enabled:
        byte = enabledPtr->enabled;
        enabledPtr--;
        if (byte == 0) {
            D_8019C7B8 = index;
        }
        index--;
        if (index < 0) {
            goto done;
        }
        goto loop_enabled;
    }

    index = 12;
loop_disabled:
    {
        value = func_8001785C(index);
        {
            s32 offset = index * 8;
            ptr = (GameCarEntry *)(offset + (s32)g_CarTable);
        }
        if (ptr->enabled == 0) {
            if (g_RaceProgress->maxClassReached < 4) {
                if ((g_RaceProgress->maxClassReached + 1) < value) {
                    index--;
                    goto loop_disabled_check;
                }
            } else if (g_RaceProgress->maxClassReached < value) {
                index--;
                goto loop_disabled_check;
            }
            D_8019C7B8 = index;
        }
        index--;
    }
loop_disabled_check:
    if (index >= 0) {
        goto loop_disabled;
    }

done:
}
