#include "common.h"
#include "game/car.h"

extern s32 D_801E40D4;
extern GameCarEntry *D_8019C7C8;
extern s16 D_8019CA18;
extern s16 D_801E41A4;

void func_80055454(void) {
    s32 index;
    GameCarEntry *ptr;

    D_8019CA18 = -1;
    index = D_801E40D4 - 1;
    if (index >= 0) {
        s32 one = 1;
        s32 offset = index * 8;
        ptr = (GameCarEntry *)(offset + (s32)D_8019C7C8);
        while (index >= 0) {
            if (ptr->enabled == one) {
                D_8019CA18 = index;
                break;
            }
            index--;
            ptr--;
        }
    }

    D_801E41A4 = -1;
    index = D_801E40D4 + 1;
    if (index < 13) {
        s32 one = 1;
        s32 offset = index * 8;
        ptr = (GameCarEntry *)(offset + (s32)D_8019C7C8);
        while (index < 13) {
            if (ptr->enabled == one) {
                D_801E41A4 = index;
                break;
            }
            index++;
            ptr++;
        }
    }
}
