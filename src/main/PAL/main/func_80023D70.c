#include "common.h"

extern s32 D_8009E6A4;
extern s32 D_8019C7B4;
extern s32 D_8019C868;
extern s32 D_8019C97C;
extern s16 D_8019CABC;
extern s32 D_8019CB14;
extern s32 D_801E428C;
extern u16 D_801E436E;
extern s32 D_801E4B8C;
extern s32 D_801E4B9C;
extern s32 D_801E4D68;
extern s32 D_801E4D6C;
extern s16 D_801E4DAC;

s32 func_800153FC(void);
void func_8001965C(void);
void func_80023B08(s32 arg0);
void func_80023BB4(void);
s32 func_800632B0(void);
void func_8005D6EC(s32 arg0);

void func_80023D70(void) {
    s32 old;
    s32 value;
    register s32 buttons asm("$3");

    func_80023BB4();

    old = D_8019C7B4;
    if (D_801E436E & 0x1000) {
        D_8019C7B4 = old - 1;
    } else if (D_801E436E & 0x4000) {
        D_8019C7B4 = old + 1;
    }

    D_8019C7B4 = (D_8019C7B4 + 6) % 6;
    if (old != D_8019C7B4) {
        func_8005D6EC(1);
    }

    buttons = D_801E436E;
    if (buttons & 0x860) {
        func_8005D6EC(2);
        switch (D_8019C7B4) {
        case 0:
            D_8019CB14 = 2;
            D_8019C97C = 0;
            D_801E4D6C = 0;
            D_801E4D68 = 0;
            break;
        case 1:
            func_800153FC();
            D_8019CB14 = 7;
            break;
        case 2:
            D_8019CB14 = 4;
            D_8019C868 = 0;
            break;
        case 3:
            D_801E4DAC = 0;
            D_8019CABC = 0;
            D_8009E6A4 = (func_800632B0() & 0xFFF) % 5;
            value = func_800632B0() & 0xFFF;
            D_801E428C = value % 4;
            if ((D_8009E6A4 < 2) && (D_801E428C == 3)) {
                D_801E428C = (func_800632B0() & 0xFFF) % 3;
            }
            func_8001965C();
            func_80023B08(0x1B);
            break;
        case 4:
            D_8019CB14 = 6;
            D_801E4D68 = D_801E4B8C;
            D_801E4D6C = D_801E4B9C;
            break;
        case 5:
            func_80023B08(2);
            break;
        }
    } else {
        register s32 masked asm("$2");

        masked = buttons & 0x90;
        if (masked) {
            func_8005D6EC(3);
            func_80023B08(2);
        }
    }
}

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/func_80023D70", func_80023FE8);
INCLUDE_ASM("asm/PAL/main/nonmatchings/main/func_80023D70", func_80024420);
