#include "common.h"
#include "game/asset.h"
#include "game/car.h"

typedef struct UnkEventPair {
    s16 timer;
    s16 carIndex;
} UnkEventPair;

extern s32 D_801E682C;
extern u32 D_801E42E4;
extern u16 D_801E436E;
extern u32 D_8019C760;
extern void (*D_8007D6D0[])(void);
void func_80017BE4(void);
void func_800268EC(void);
extern s32 D_8019C768;
extern s32 D_801E42E0;
extern s32 D_801E42A0;
extern s32 D_801E4178;
extern s32 D_801E6824;
extern s32 D_8009E66C;
void func_80065860(s32 arg0);
void func_8001BE9C(s32 arg0, s32 arg1, s32 arg2);
extern s32 D_801E428C;
extern u8 D_80011010[];
void func_80033AA0(s32 arg0, s32 arg1);
void func_80025940(void);
void func_80042BF0(void);
void func_80016EA0(s32 arg0, s32 arg1, void *arg2, s32 arg3);
void func_80019730(void);
void func_80019844(void);
void func_80026C0C(void);
void func_80042BC0(u32 arg0);
extern s16 D_8007D6DC[];
extern s16 D_8007D6DE[];
extern s32 D_8007D6E0[];
extern u32 D_801E40B8;
extern u8 *D_8019C900;
void func_800168AC(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
s32 func_8001720C(u8 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
s32 func_80017390(u8 *arg0, s32 arg1, s32 arg2);
void func_80042C0C(void);
void func_80018410(void);
extern s32 D_8009E694;
extern u32 D_8009E870;
extern u32 D_801E4030;
extern u8 D_801F18CC[];
extern GameCarRuntime D_801F1854[];
extern UnkEventPair D_8007D74C[];
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

void func_80026AE0(void) {
    D_8007D6D0[D_801E682C]();

    if ((D_801E42E4 == 0x1E) && ((D_801E436E & 0x860) != 0)) {
        if (g_AssetLoadState != 0) {
            func_80017BE4();
            D_801E42E4 = 3;
            D_8019C760 = 0;
        } else {
            func_800268EC();
        }
    }
}

void func_80026B88(void) {
    func_80065860(0);
    func_8001BE9C(0, 0, 0);

    D_8019C768 = 0x80;
    D_801E42E0 = 0x108;
    D_801E42A0 = -4;
    D_801E42E4 = 0x20;
    D_801E4178 = 0;
    D_801E6824 = 0;
    D_801E40B8 = 0;
    D_8009E66C = 3;
}

void func_80026C0C(void) {
    register s32 delta asm("$3");

    if (D_801E40B8 == 2) {
        func_80065860(1);
    }

    delta = D_801E42A0;
    if (delta < 0) {
        register s32 value asm("$2");

        value = D_801E42E0;
        value = value + delta;
        D_801E42E0 = value;

        if (D_801E42E0 < 0) {
            D_801E42E0 = 0;
            D_801E42A0 = 0;
        }

        func_80033AA0(D_801E42E0, 0x49);
    } else if (delta > 0) {
        register s32 value asm("$4");

        value = D_801E42E0;
        value = value + delta;
        D_801E42E0 = value;

        func_80033AA0(D_801E42E0, 0x49);

        if (D_801E42E0 >= 0x101) {
            func_80065860(0);
            D_801E428C = 0;
            func_80025940();
            func_80042BF0();
            D_801E4178 = 3;
            D_801E42E0 = 0x100;
            D_801E42A0 = 0;
        }
    }

    func_80016EA0(0x5E, 0x72, D_80011010, 0x7812);
}

void func_80026D30(void) {
    if (g_AssetLoadState == 0) {
        func_80019730();
        func_80019844();
        D_801E4178 = 1;
    }

    func_80026C0C();
}

void func_80026D78(void) {
    if (g_AssetLoadState == 0) {
        D_801E42A0 = 4;
        func_80042BC0(2);
        D_801E4178 = 2;
    }

    func_80026C0C();
}

void func_80026DC4(void) {
    func_80026C0C();
}

void func_80026DE4(void) {
    s32 i;
    s32 offset;
    s32 adjusted;
    s32 delta;
    s32 value;
    s32 clamped;
    s32 next;
    s32 arg1;
    s32 arg7;
    s32 scale_b;
    s32 arg8;

    for (i = 0, offset = 0; i < 14; offset += 8) {
        s32 tableY;

        adjusted = (D_801E40B8 / 3) - 0xD0;
        tableY = *(s16 *)((u8 *)D_8007D6DE + offset);
        delta = tableY - adjusted;

        if (delta < 0x60) {
            value = (0x60 - delta) << 1;
        } else if (delta >= 0x91) {
            value = (delta - 0x90) << 1;
        } else {
            value = 0;
        }

        if (value >= 0) {
            clamped = value;
            if (clamped >= 0x80) {
                clamped = 0x7F;
            }
        } else {
            clamped = 0;
        }

        value = 0x7F - clamped;
        i++;
        if (value != 0) {
            func_800168AC(
                *(s16 *)((u8 *)D_8007D6DC + offset),
                delta,
                *(s32 *)((u8 *)D_8007D6E0 + offset),
                0x78CC,
                value);
        }
    }

    {
        register s32 camera asm("t1");
        register s32 *scratch asm("s1");
        register u8 *ptr asm("s0");
        register s32 scale_a asm("v1");
        register s32 tmp asm("v0");
        register u8 *base asm("v0");

        camera = D_801E42E0;
        scratch = (s32 *)0x1F800000;
        tmp = (camera << 3) - camera;
        scale_a = tmp << 5;
        base = D_8019C900;
        arg1 = *scratch;
        ptr = base + 0xD0;
        arg7 = (scale_a / 0x100) + 0x20;
        scale_b = ((camera << 1) + camera) << 6;
        arg8 = (scale_b / 0x100) + 0x40;

        next = func_8001720C(ptr, arg1, 0, 0, 0x140, 0xF0, camera, arg7, arg8);
        *scratch = func_80017390(ptr, next, 0x49);
    }
}

void func_80026F68(void) {
    D_801E42E4 = 6;
    func_80042C0C();
    func_80018410();
}

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
