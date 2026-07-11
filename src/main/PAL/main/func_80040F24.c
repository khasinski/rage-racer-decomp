#include "common.h"

#include "game/audio.h"

extern u8 *D_801E4150;
extern s32 D_801E7A50;
extern s32 D_801E408C;
extern s16 D_8009E74C;
extern s16 D_8009E83C;
extern s32 D_801E4364;
extern s16 D_801E8A8C;

void func_80040F24(void) {
    register u8 *rawBase asm("v0");
    register u8 *base asm("s0");
    register s32 i asm("a1");
    register s32 mask asm("a2");
    register u8 *stateBase asm("a3");
    register s32 temp asm("v0");
    register s32 entry asm("v1");
    register s32 loopFlags asm("t0");
    register s32 current asm("a0");
    s32 product;

    rawBase = D_801E4150;
    current = D_801E7A50;
    base = rawBase + 0x1CCC;

    if (!(current & 8)) {
        if (D_8009E74C == *(s16 *)((D_801E408C << 2) + (s32)base)) {
            entry = D_8009E83C;
            if (entry == D_801E4364) {
                entry = current | 8;
                D_801E7A50 = entry;
                if (D_801E8A8C < 10) {
                    GamePlaySoundCue(0x2A);
                }
            }
        }
    }

    if (D_801E8A8C != 0) {
        return;
    }

    stateBase = (u8 *)&D_8009E74C;
    i = 0;
    temp = 0x10;
    do {
        loopFlags = D_801E7A50;
        mask = temp << i;
        temp = mask & loopFlags;
        if (temp == 0) {
            temp = D_801E408C;
            entry = ((temp * 3) + i) << 2;
            entry += (s32)base;
            current = *(s16 *)(entry + 0x10);
            temp = -1;
            if (current == temp) {
                return;
            }

            temp = *(s16 *)stateBase;
            if (temp == current) {
                entry = *(s16 *)(entry + 0x12);
                temp = *(s32 *)(stateBase + 0xD0);
                product = entry * temp;
                temp = product / 100;
                entry = *(s32 *)(stateBase + 0x2C);
                if (temp < entry) {
                    temp = *(s16 *)(stateBase + 0xA);
                    if (temp <= 0) {
                        temp = mask | loopFlags;
                        D_801E7A50 = temp;
                        GamePlaySoundCue(0x23);
                    }
                }
                return;
            }
        }
        i++;
        temp = i < 3;
        if (temp == 0) {
            break;
        }
        temp = 0x10;
    } while (1);
}
