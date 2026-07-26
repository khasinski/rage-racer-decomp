#include "common.h"
#include "game/asset.h"
#include "game/car.h"
#include "game/race.h"
#include "game/render.h"
#include "game/state.h"

extern s32 D_8019C99C;
extern void (*D_8007D6B8[])(void);
extern s32 D_801E4B30;
extern s32 D_8019C768;
extern s32 D_801E682C;
extern s32 D_8009E66C;
void func_80065860(s32 arg0);
void func_8001BE9C(s32 arg0, s32 arg1, s32 arg2);
void func_8001A3C0(void *arg0);
void func_80019730(void);
s32 func_80019844(void);
extern s16 D_8007D6CC[];
s32 func_8002664C(s32 arg0);

void func_80046A2C(
    u8 *arg0,
    u32 arg1,
    u32 arg2,
    u32 arg3,
    u32 arg4,
    u32 arg5,
    u32 arg6,
    u32 arg7,
    u32 arg8,
    u32 arg9,
    u32 arg10,
    u32 arg11,
    u32 arg12,
    u32 arg13);

extern u32 D_8009E6CC;
extern u8 D_801E7734[];
void func_80025940(void);
void func_80025E54(s32 arg0);
void func_80042BC0(s32 arg0);
void func_80042BF0(void);
void func_800266D0(void);
extern u32 D_8019C760;
void func_80042C94(void);
extern u32 D_8009E870;
extern u32 D_801E4030;
extern u8 D_801F18CC[];
extern GameCarRuntime D_801F1854[];
void func_80033AA0(s32 arg0, u32 arg1);
void func_80042CCC(u32 arg0);
void func_800268EC(void);
s32 func_8001A1F0(u32 arg0, s32 arg1);
void func_8003BB50(void);
void func_80019EFC(s32 arg0);
void func_80043BCC(u32 arg0, GameCarRuntime *arg1);
void func_800389F0(void);
void func_80045CD4(void);
void func_800418D4(void);
void func_80041888(void);
void func_8004123C(void);
void GameDrawCourseScenery2(u32 arg0, u32 arg1) asm("func_8003E2E8");

void func_80026570(void) {
    void (*func)(void);

    func = D_8007D6B8[D_8019C99C];
    g_SceneTimer++;
    func();
}

void func_800265BC(void) {
    s32 initialValue;

    func_80065860(0);
    func_8001BE9C(0, 0, 0);

    initialValue = 0x80;
    D_8019C768 = initialValue;
    func_8001A3C0((void *)D_801E4B30);
    func_80019730();
    func_80019844();

    D_801E682C = 0;
    g_FadeLevel = initialValue;
    g_SceneTimer = 0;
    g_SceneId = 0x1E;
    D_8009E66C = 0;
}

s32 func_8002664C(s32 arg0) {
    s32 value;

    if (D_801E682C == 0) {
        value = (g_SceneTimer * 4) - D_8007D6CC[arg0];
    } else {
        if (g_FadeLevel > 0) {
            g_FadeLevel--;
        }
        value = g_FadeLevel;
    }

    return value < 0 ? 0 : (value < 0x80 ? value : 0x7F);
}

void func_800266D0(void) {
    u8 *ptr;
    s32 value;
    u32 one;
    u32 arg13;

    ptr = g_DrawBuffer + 0xCC;
    value = func_8002664C(0);
    one = 1;
    arg13 = 0x29;
    func_80046A2C(ptr, 0x74, 0x34, 0x58, 0x38, 0xA8, 0xA8, value, value, value, 0x1F, 0, one, arg13);
    func_80046A2C(ptr, 0x44, 0x70, 0xB8, 0x14, 0x48, 0xE8, value, value, value, 0x80, 0, one, arg13);
    value = func_8002664C(1);
    func_80046A2C(ptr, 0x5E, 0x90, 0x84, 0xC, 0, (g_CourseIndex * 12) + 0x9C, value, value, value, 0x12, 0, one, arg13);
}

void func_80026824(void) {
    s32 mode;
    u32 timer;

    timer = g_SceneTimer;
    if (timer < 0x2710) {
        g_SceneTimer = timer + 1;
    }

    if (g_SceneTimer == 2) {
        func_80065860(1);
    }

    if (g_AssetLoadState == 0) {
        func_80025940();

        D_801E682C = 1;
        mode = D_801E7734[D_8009E6CC];
        func_80025E54(mode);

        mode += 3;
        if (mode == 0xC) {
            mode = 0x11;
        }

        func_80042BC0(mode);
        func_80042BF0();
    }

    func_800266D0();
}

void func_800268EC(void) {
    g_SceneId = 3;
    D_8019C760 = 0;
    func_80042C94();
}

void func_80026920(void) {
    u32 value;
    u32 timer;
    s32 index;
    s32 offset;

    g_SceneTimer++;
    if ((u32)g_SceneTimer < 0x3D) {
        func_800266D0();
        value = g_SceneTimer - 6;
        func_80033AA0(0xFF - (((value * 3) * 4) - value), 0x49);
    }

    timer = g_SceneTimer;
    if (timer == 0x6CC) {
        func_80042CCC(0x38);
        timer = g_SceneTimer;
    }
    if (timer >= 0x6CD) {
        u32 adjusted;

        adjusted = timer - 0x6CC;
        func_80033AA0((adjusted * 4) + adjusted, 0x49);
    }

    if (g_SceneTimer == 0x708) {
        func_800268EC();
    }

    g_AnimTimer++;
    D_8009E66C = func_8001A1F0(0xFF, D_8009E66C);
    func_8003BB50();

    index = D_8009E66C;
    offset = (((((index * 3) * 4) + index) * 8) - index) * 4;
    func_80019EFC(*(s16 *)&D_801F18CC[offset]);

    func_80043BCC(D_8009E870, &D_801F1854[D_8009E66C]);
    func_800389F0();
    func_80045CD4();
    func_800418D4();
    *(u32 *)0x1F800084 = D_801E4030;
    func_80041888();
    func_8004123C();
    GameDrawCourseScenery2(g_AnimTimer, 1);
}
