#include "common.h"
#include "game/car.h"

extern s32 D_801E40B8;
extern s32 D_801E42A0;
extern s32 D_801E42E0;
extern u16 D_801E436E;
extern s32 D_801E6824;
extern s32 D_8009E694;
extern s32 D_8009E66C;
extern u32 D_8009E870;
extern u32 D_801E4030;
extern u8 D_801F18CC[];
extern GameCarRuntime D_801F1854[];

typedef struct UnkEventPair {
    s16 timer;
    s16 carIndex;
} UnkEventPair;

extern UnkEventPair D_8007D74C[];

void func_80065860(s32 arg0);
void func_80026F68(void);
void func_80026DE4(void);
void func_8003BB50(void);
void func_80019EFC(s32 arg0);
void func_80043BCC(u32 arg0, GameCarRuntime *arg1);
void func_800389F0(void);
void func_80045CD4(void);
void func_800418D4(void);
void func_80041888(void);
void func_8004123C(void);
void func_8003E2E8(s32 arg0, s32 arg1);

void func_80026F9C(void) {
    s32 timer;
    s32 active;
    s32 eventIndex;

    if (D_801E40B8 == 2) {
        func_80065860(1);
    }

    if ((u32)D_801E40B8 >= 0x79 && (D_801E436E & 0x860)) {
        func_80026F68();
    }

    timer = D_801E40B8;
    if (timer == 0x3C) {
        D_801E42A0 = -4;
    } else if (timer == 0x42E) {
        D_801E42A0 = 2;
    } else if (timer == 0x500) {
        func_80026F68();
    }

    D_801E42E0 += D_801E42A0;
    if (D_801E42E0 < 0) {
        D_801E42E0 = 0;
        D_801E42A0 = 0;
    } else if (D_801E42E0 >= 0x100) {
        D_801E42E0 = 0xFF;
        D_801E42A0 = 0;
    }

    func_80026DE4();

    active = (u32)(D_801E40B8 - 0x10) < 0x40F;
    if (active) {
        eventIndex = D_801E6824;
        D_8009E694++;
        if (D_8007D74C[eventIndex].timer == D_801E40B8) {
            D_801E6824 = eventIndex + 1;
            D_8009E66C = D_8007D74C[eventIndex].carIndex;
        }

        func_8003BB50();

        func_80019EFC(*(s16 *)&D_801F18CC[(((((D_8009E66C * 3) * 4) + D_8009E66C) * 8) - D_8009E66C) * 4]);

        func_80043BCC(D_8009E870, &D_801F1854[D_8009E66C]);
        func_80045CD4();
    }

    func_800389F0();
    func_800418D4();
    *(u32 *)0x1F800084 = D_801E4030;
    func_80041888();
    func_8004123C();
    func_8003E2E8(D_8009E694, active);
}
