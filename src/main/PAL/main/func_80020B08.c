#include "common.h"
#include "game/car.h"
#include "game/race.h"

extern s32 D_801E4B94;
extern s32 D_8019C8EC;
extern GameRaceProgress *D_801E4FAC;
extern GameCarEntry *D_8019C7C8;
extern s32 D_8009E6A4;
extern s32 D_801E419C;
extern s16 D_801E4034;
extern s32 D_801E772C[];
extern s32 D_801E42E4;

void func_80021288(s32 arg0, s32 arg1);
void func_800212F0(s32 arg0);
void func_80019BB8(s32 arg0);
void func_80019B3C(s32 arg0);

void func_80020B08(void) {
    s32 oldValue;
    GameRaceProgress *ptr;
    s32 *entry;

    if (D_801E4B94 != 0) {
        if (D_8019C8EC != 0) {
            s32 magic;
            GameRaceProgress *afterPtr;

            ptr = D_801E4FAC;
            oldValue = ptr->progression;
            func_80021288((s32)D_8019C7C8, (s32)ptr);
            magic = 0x3B9AC9FF;
            afterPtr = D_801E4FAC;
            afterPtr->elapsedTime = magic;
            afterPtr->progression = oldValue;
            func_800212F0(0);
            func_80019BB8(0x21);
        } else {
            s32 current;
            s32 next;
            GameRaceProgress *menuPtr;
            s32 enabled;

            func_80019B3C(7);
            current = D_8009E6A4;
            menuPtr = D_801E4FAC;
            enabled = D_801E419C;
            next = current + 1;
            D_8009E6A4 = next;
            menuPtr->lap = next;
            menuPtr->state = 0;

            if (enabled != 0) {
                menuPtr->progression = next;
                entry = &D_801E772C[D_801E4034];
                if (*entry < next) {
                    *entry = next;
                }
            }

            func_800212F0(D_8009E6A4);
        }
    } else {
        D_801E42E4 = 6;
    }
}
