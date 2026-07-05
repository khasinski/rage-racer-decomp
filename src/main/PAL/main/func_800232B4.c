#include "common.h"

extern s32 D_8007BED8;
extern s32 D_8007D53C;
extern s32 D_8007D540;
extern s32 D_8007D544;
extern s32 D_801E40B8;
extern u16 D_801E436A;

void func_80019AF0(s32 arg0);
void func_8001BE9C(s32 arg0, s32 arg1, s32 arg2);
void func_8001C088(s32 arg0, s32 arg1, s32 arg2);
void func_800230B0(void);
void func_8002317C(void);
void func_80065860(s32 arg0);

void func_800232B4(void) {
    s32 state;

    if (D_8007D540 < 110) {
        if (D_8007D540 >= 10) {
            func_80065860(1);
        }
        func_800230B0();
        goto inc_timer;
    } else if (D_8007D540 == 110) {
        func_80065860(0);
        func_8001C088(0, 0, 0);
inc_timer:
        D_8007D540++;
        return;
    }

    if (D_8007D544 != 0) {
        D_8007D544--;
        if ((D_8007BED8 == 0) && (D_801E436A != 0)) {
            D_8007D544 = 0;
        }
    }

    state = D_8007D53C;
    switch (state) {
    case 0:
        if ((u32)D_801E40B8 < 0x100) {
            D_801E40B8 += 8;
        } else {
            D_8007D53C = 1;
        }
        break;
    case 1:
        if (D_8007D544 == 0) {
            D_8007D53C = 2;
        }
        break;
    case 2:
        D_801E40B8 -= 8;
        if (D_801E40B8 == 0) {
            D_8007D53C = 3;
            func_8001BE9C(0, 0, 0);
        }
        break;
    case 3:
        D_801E40B8++;
        if ((u32)D_801E40B8 >= 21) {
            func_80019AF0(3);
        }
        break;
    }

done:
    if (D_8007D53C != 3) {
        func_8002317C();
        if ((u32)D_801E40B8 >= 10) {
            func_80065860(1);
        }
    }
}
