#include "common.h"

extern u16 D_801E4370;
extern u16 D_801E436E;
extern s32 D_801E4D68;
extern s32 D_801E4D6C;
extern s32 D_801E4B8C;
extern s32 D_801E4B9C;
extern s32 D_8019CB14;
extern u16 D_8019CE9C;
extern u16 D_8019CE9E;
extern u16 D_801C0684;
extern u16 D_801C0686;

void func_800253A4(void);
void func_8005D6EC(s32 arg0);

void func_800254C8(void) {
    s32 oldX;
    s32 oldY;
    u16 input;
    u16 confirm;
    u32 confirmMask;
    s32 value;

    func_800253A4();

    input = D_801E4370;
    oldX = D_801E4D68;
    oldY = D_801E4D6C;

    if ((input & 0x1000) && (oldY >= -31)) {
        D_801E4D6C = oldY - 1;
    }

    if (D_801E4370 & 0x4000) {
        value = D_801E4D6C;
        if (value < 23) {
            D_801E4D6C = value + 1;
        }
    }

    if (D_801E4370 & 0x8000) {
        value = D_801E4D68;
        if (value >= -10) {
            D_801E4D68 = value - 1;
        }
    }

    if (D_801E4370 & 0x2000) {
        value = D_801E4D68;
        if (value < 32) {
            D_801E4D68 = value + 1;
        }
    }

    if ((oldX != D_801E4D68) || (oldY != D_801E4D6C)) {
        func_8005D6EC(1);
    }

    confirm = D_801E436E;
    if (confirm & 0x860) {
        func_8005D6EC(2);
        D_8019CB14 = 1;
        D_801E4B8C = D_801E4D68;
        D_801E4B9C = D_801E4D6C;
    } else {
        confirmMask = confirm & 0x90;
        if (confirmMask != 0) {
            func_8005D6EC(3);
            D_8019CB14 = 1;
            D_801E4D68 = D_801E4B8C;
            D_801E4D6C = D_801E4B9C;
        }
    }

    D_8019CE9C = D_801E4D68;
    D_8019CE9E = D_801E4D6C + 29;
    D_801C0684 = D_801E4D68;
    D_801C0686 = D_801E4D6C + 29;
}
