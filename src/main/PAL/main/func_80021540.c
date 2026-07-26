#include "common.h"
#include "game/screens.h"
#include "game/race.h"
#include "game/state.h"
#include "game/render.h"
#include "game/cd.h"

extern s16 g_ClassRecords[] asm("D_8019CB40");
extern volatile s32 D_801E4DA8;
extern s32 g_BgmTrackCount asm("D_801E40A8");
extern u8 D_80010E68;
void func_80016B7C(u32 arg0, u32 arg1, void *arg2, u32 arg3, u32 arg4);
extern s32 D_8019C768;
extern s32 D_801E3E0C;
void func_8005B190(s32 arg0, s32 arg1);
void func_800215B8(s32 arg0);
extern void *g_CourseProgress asm("D_8009E67C");
extern char D_80010E80[];
extern char D_80010E8C[];
extern char D_80010E98[];
extern char D_80010EA0[];
extern s16 D_8007D438[];
void func_80016EA0(s32 arg0, s32 arg1, void *arg2, s32 arg3);
void func_80016754(s32 arg0, s32 arg1, void *arg2, s32 arg3);
extern volatile u16 g_PadEdge2 asm("D_801E436E");
void GamePlaySoundCue(s32 cue) asm("func_8005D6EC");
void func_80018410(void);
void func_80033AA0(s32 arg0, s32 arg1);
void func_80021654(void);
void func_80046A2C(void *arg0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8, s32 a9, s32 a10, s32 a11, s32 a12, s32 a13);
void func_800218A0(s32 arg0);
void func_800212F0(s32 arg0);

void func_80021540(void) {
    s32 offset;
    s32 current;
    s32 one;
    s32 value;

    D_801E4DA8 = 0;
    one = 1;
    offset = 0;
    do {
        current = *(s16 *)((u8 *)g_ClassRecords + offset);
        offset += 4;
        if (current == one) {
            D_801E4DA8 = D_801E4DA8 + 1;
        }
    } while (offset < 0x2C);

    value = D_801E4DA8;
    value = value < 5;
    if (value) {
        value = 9;
    } else {
        value = 10;
    }
    g_BgmTrackCount = value;
}

void func_800215B8(s32 arg0) {
    if (arg0 >= 0x100) {
        arg0 = 0xFF;
    }
    arg0 >>= 1;
    func_80016B7C(0x28, 0x40, &D_80010E68, 0x7812, arg0);
}

void func_800215FC(void) {
    D_8019C768 = 0x80;
    func_8005B190(0x28, 0x28);
    g_SceneId = 0xE;
    D_801E3E0C = 0;
    g_SceneTimer = -1;
    func_800215B8(0xFF);
}

void GameDrawRaceEndPrompt(void) {
    s32 color = 0x7812;
    s32 drawColor;
    s16 index;

    if (g_SceneTimer & 4) {
        color = 0x784C;
    }

    drawColor = 0x7812;
    if (D_801E3E0C == 0) {
        drawColor = color;
    }
    func_80016EA0(0x6A, 0x68, D_80010E80, drawColor);

    drawColor = 0x7812;
    if (D_801E3E0C != 0) {
        drawColor = color;
    }
    func_80016EA0(0x70, 0x78, D_80010E8C, drawColor);

    func_80016EA0(0x76, 0xB8, D_80010E98, 0x7812);

    index = *(s16 *)((u8 *)g_CourseProgress + 6);
    func_80016EA0(0xBE, 0xB8, &D_8007D438[index], 0x7812);

    func_80016754(0x58, 0xD0, D_80010EA0, 0x78CC);
    func_800215B8(0xFF);
}

void func_80021748(void) {
    s32 timer;
    s32 old;
    s32 current;
    u8 *ptr;
    u16 value;

    timer = g_SceneTimer;
    if (timer == -1) {
        old = D_801E3E0C;
        if ((g_PadEdge2 & 0x1000) && (old == 1)) {
            D_801E3E0C = 0;
        }
        if ((g_PadEdge2 & 0x4000) && (D_801E3E0C == 0)) {
            D_801E3E0C = 1;
        }
        current = D_801E3E0C;
        if (old != current) {
            GamePlaySoundCue(1);
        }
        if (g_PadEdge2 & 0x800) {
            GamePlaySoundCue(2);
            if (D_801E3E0C != 0) {
                func_80018410();
            }
            ptr = g_CourseProgress;
            value = *(u16 *)(ptr + 6);
            g_SceneTimer = 0;
            *(u16 *)(ptr + 6) = value - 1;
        }
    } else {
        timer += 2;
        g_SceneTimer = timer;
        func_80033AA0(timer, 0x49);
        if (g_SceneTimer == 0x100) {
            if (D_801E3E0C != 0) {
                g_SceneId = 6;
            } else {
                g_SceneId = 0xB;
            }
        }
    }

    func_80021654();
}

void func_800218A0(s32 arg0) {
    if (arg0 >= 256) {
        arg0 = 0xFF;
    }
    arg0 >>= 1;
    func_80046A2C(g_DrawBuffer + 204, 0x50, 0x6C, 0xA0, 0x18, 0, 0x28, arg0, arg0, arg0, 0xC, 0, 1, 0x29);
}

void func_80021920(void) {
    D_8019C768 = 0x80;
    g_SceneId = 0x10;
    g_SceneTimer = 0x22B;
    func_800218A0(0x22B);
}

void func_80021964(void) {
    s32 v = g_SceneTimer - 1;
    g_SceneTimer = v;
    if ((g_PadEdge2 & 0x860) && (u32)v >= 261) {
        GameStartCdVolumeFade(0xFA);
        g_SceneTimer = 0xFF;
    }
    if (g_SceneTimer == 0) {
        func_80018410();
        func_800212F0(g_GrandPrixClass);
        g_SceneId = 6;
    }
    func_800218A0(g_SceneTimer);
}
