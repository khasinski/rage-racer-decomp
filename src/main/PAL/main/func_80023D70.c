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

extern u8 *D_8019C900;

typedef struct { s16 x, y; } XY;
typedef struct { u8 r, g, b; } RGB;
typedef struct { s16 flag; u16 val; } ScoreRec;

extern XY D_8007D5A8[];
extern RGB D_8007D658[];
extern ScoreRec D_8019CB40[];

s32 func_800153FC(void);
void func_8001965C(void);
void func_80023B08(s32 arg0);
void func_80023BB4(void);
s32 func_800632B0(void);
void func_8005D6EC(s32 arg0);
s32 func_80017138(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8);
s32 func_80017390(s32 a0, s32 a1, s32 a2);
s32 func_80032F34(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8);

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

void func_80023FE8(void) {
    s32 raw = (s32)D_8019C900;
    s32 base = raw + 0xCC;
    s32 next = *(s32 *)0x1F800000;
    s32 idx = D_801E4D6C * 6 + D_801E4D68;
    s32 x;
    s32 y = 0x38;
    s32 i;

    if (D_8019CB14 == 3) {
        next = func_80032F34(raw + 0xD4, next,
                             D_8007D5A8[idx].x - 2, D_8007D5A8[idx].y - 4,
                             0x24, 0x58, 0x89, 0xFF, 0x76);
    }
    next = func_80017138(base, next, 0xBC, 0x40, 0x18, 0x10, 0, 0x6C, 0x7F40);
    next = func_80017138(base, next, 0xD8, 0x40, 8, 0x10, D_801E4D68 * 8 + 8, 0x18, 0x7F40);

    x = 0xB4;
    if (D_8019CB40[idx].flag == -1) {
        for (i = 0; i < 8; i++) {
            next = func_80017138(base, next, x + 0x30 + i * 8, y + 8, 8, 0x10, 0x38, 0x28, 0x7F40);
        }
    } else {
        next = func_80017138(base, next, 0xE4, 0x40,
                             D_8007D658[idx].b, 0x10,
                             D_8007D658[idx].r, D_8007D658[idx].g, 0x7F40);
    }

    next = func_80017138(base, next, x | 8, y + 0x28, 0x44, 0x10, 0x1C, 0x6C, 0x7F40);
    next = func_80017138(base, next, x + 100, y + 0x28, 8, 0x10,
                         (s16)((s16)D_8019CB40[idx].val / 10) << 3, 0x18, 0x7F40);
    next = func_80017138(base, next, x + 108, y + 0x28, 8, 0x10,
                         (s16)((s16)D_8019CB40[idx].val % 10) << 3, 0x18, 0x7F40);
    next = func_80017390(base, next, 0x3B);
    next = func_80032F34(base, next, x + 78, y + 47, 0x14, 2, 0xFF, 0xFF, 0xFF);
    next = func_80032F34(base, next, x, y, 0x7C, 0x1E, 0, 0, 0);
    next = func_80032F34(base, next, x, y + 32, 0x7C, 0x1E, 0, 0, 0);
    *(s32 *)0x1F800000 = func_80032F34(base, next, x - 1, y - 2, 0x7E, 0x42, 0xFF, 0xFF, 0xFF);
}
INCLUDE_ASM("asm/PAL/main/nonmatchings/main/func_80023D70", func_80024420);
