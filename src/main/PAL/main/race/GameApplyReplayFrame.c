#include "common.h"
#include "game/race.h"
#include "game/state.h"
#include "game/render.h"
#include "game/car.h"

#define AVG(a, b) ((s32)((a) + (b) + ((u32)((a) + (b)) >> 31)) >> 1)

typedef struct {
    s32 x;
    s32 y;
    s32 z;
    u8 pad0C[0x14];
    s32 rotX;
    s32 rotY;
    s32 rotZ;
    u8 pad2C[0x18];
    s32 field44;
    s32 flags48;
    u8 pad4C[0x14];
    s32 z2;
    u8 pad64[0x28];
    s16 field8C;
    u8 pad8E[0x20];
    s16 variantAE;
} MenuObj;

typedef struct {
    u16 x0;
    s16 y0;
    u16 z0;
    s16 z20;
    s16 rotX0;
    s16 rotY0;
    s16 rotZ0;
    s16 flags0;
    u16 x1;
    s16 y1;
    u16 z1;
    s16 z21;
    s16 rotX1;
    s16 rotY1;
    s16 rotZ1;
    s16 flags1;
    u16 field20;
    u8 pad22[0xA];
    s16 field44_0;
    s16 field44_1;
} MenuBigFrame;

typedef struct {
    u16 x;
    s16 y;
    u16 z;
    s16 z2;
    s16 rotX;
    s16 rotY;
    s16 rotZ;
    s16 flags;
    u16 field10;
    u8 pad12[0x6];
    s16 field44;
} MenuSmallFrame;

extern u16 g_ReplayPlayerModel asm("D_801E4D8C");
extern u16 g_ReplayRivalModel asm("D_801E4BC0");
extern MenuBigFrame *g_ReplayFramesGp asm("D_8009F0A4");
extern u8 *g_ReplayFramesTimeAttack asm("D_8019C7A4");
extern s32 g_ReplayWriteCursor asm("D_801E4BB0");
extern s32 g_ReplayFrameCount asm("D_8019CB6C");
extern s32 g_ReplayBufferWrapped asm("D_8009EC8C");
extern u8 g_PlayerCar asm("D_8009E6D4");
void func_8001F134(s32 arg0, u8 *arg1, u8 *arg2);
void func_8001F274(s32 arg0, u8 *arg1);
extern s32 g_ReplayReadCursor asm("D_801F179C");
extern u8 *g_EnvScriptClock asm("D_8019C8FC");
void func_800458CC(void *arg0);
void GameSeedReplayCars(void) asm("func_80034F74");
extern s32 g_SeriesCleared asm("D_8019C8EC");
s32 func_80016EC4(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
s32 func_80017390(s32 arg0, s32 arg1, s32 arg2);

void GameApplyReplayFrame(s32 arg0, MenuObj *arg1, MenuObj *arg2) asm("func_8001F330");
void GameApplyReplayFrame(s32 arg0, MenuObj *arg1, MenuObj *arg2) {
    s32 index;
    MenuBigFrame *big;
    MenuSmallFrame *small;
    if (g_GrandPrixMode != 0) {
        arg1->variantAE = g_ReplayPlayerModel;
        arg2->variantAE = g_ReplayRivalModel;
        if ((arg0 & 1) == 0) {
            index = arg0 >> 1;
            big = &g_ReplayFramesGp[index];
            arg1->x = big->x0;
            arg1->y = big->y0;
            arg1->z = big->z0;
            arg1->z2 = big->z20;
            arg1->rotX = big->rotX0;
            arg1->rotY = big->rotY0;
            arg1->rotZ = big->rotZ0;
            arg1->flags48 = big->flags0;
            arg1->field44 = big->field44_0;
            arg2->x = big->x1;
            arg2->y = big->y1;
            arg2->z = big->z1;
            arg2->z2 = big->z21;
            arg2->rotX = big->rotX1;
            arg2->rotY = big->rotY1;
            arg2->rotZ = big->rotZ1;
            arg2->flags48 = big->flags1;
            arg2->field44 = big->field44_1;
        } else {
            arg0 = arg0 >> 1;
            arg0 += 1;
            if (arg0 == 0x2EE) {
                arg0 = 0;
            }
            big = &g_ReplayFramesGp[arg0];
            arg1->x = AVG(big->x0, arg1->x);
            arg1->y = AVG(big->y0, arg1->y);
            arg1->z = AVG(big->z0, arg1->z);
            arg1->z2 = AVG(big->z20, arg1->z2);
            arg1->rotX = AVG(big->rotX0, arg1->rotX);
            arg1->rotY = AVG(big->rotY0, arg1->rotY);
            arg1->rotZ = AVG(big->rotZ0, arg1->rotZ);
            arg1->flags48 = AVG(big->flags0, arg1->flags48);
            arg1->field44 = AVG(big->field44_0, arg1->field44);
            arg2->x = AVG(big->x1, arg2->x);
            arg2->y = AVG(big->y1, arg2->y);
            arg2->z = AVG(big->z1, arg2->z);
            arg2->z2 = AVG(big->z21, arg2->z2);
            arg2->rotX = AVG(big->rotX1, arg2->rotX);
            arg2->rotY = AVG(big->rotY1, arg2->rotY);
            arg2->rotZ = AVG(big->rotZ1, arg2->rotZ);
            arg2->flags48 = AVG(big->flags1, arg2->flags48);
            arg2->field44 = AVG(big->field44_1, arg2->field44);
        }
        arg1->field8C = big->field20;
    } else {
        arg1->variantAE = g_ReplayPlayerModel;
        if ((arg0 & 1) == 0) {
            index = arg0 >> 1;
            small = (MenuSmallFrame *)(g_ReplayFramesTimeAttack + (((index << 3) - index) << 2));
            arg1->x = small->x;
            arg1->y = small->y;
            arg1->z = small->z;
            arg1->z2 = small->z2;
            arg1->rotX = small->rotX;
            arg1->rotY = small->rotY;
            arg1->rotZ = small->rotZ;
            arg1->flags48 = small->flags;
            arg1->field44 = small->field44;
        } else {
            arg0 = arg0 >> 1;
            arg0 += 1;
            if (arg0 == 0x505) {
                arg0 = 0;
            }
            small = (MenuSmallFrame *)(g_ReplayFramesTimeAttack + (((arg0 << 3) - arg0) << 2));
            arg1->x = AVG(small->x, arg1->x);
            arg1->y = AVG(small->y, arg1->y);
            arg1->z = AVG(small->z, arg1->z);
            arg1->z2 = AVG(small->z2, arg1->z2);
            arg1->rotX = AVG(small->rotX, arg1->rotX);
            arg1->rotY = AVG(small->rotY, arg1->rotY);
            arg1->rotZ = AVG(small->rotZ, arg1->rotZ);
            arg1->flags48 = AVG(small->flags, arg1->flags48);
            arg1->field44 = AVG(small->field44, arg1->field44);
        }
        arg1->field8C = small->field10;
    }
}

void GameApplyReplayFrameAndTilt(s32 arg0, u8 *arg1, u8 *arg2) asm("func_8001F8D0");
void GameApplyReplayFrameAndTilt(s32 arg0, u8 *arg1, u8 *arg2) {
    /* This pin is load-bearing: removing it changes .text. */
    register s32 index asm("s0");
    u8 *primary;
    u8 *secondary;
    /* These pins are load-bearing: removing any one changes .text. */
    register s32 next asm("a0");
    register s32 offset asm("v0");
    register u8 *base asm("v1");

    index = arg0;
    primary = arg1;
    secondary = arg2;

    GameApplyReplayFrame(index, (MenuObj *)primary, (MenuObj *)secondary);

    if (g_GrandPrixMode != 0) {
        if ((index & 1) == 0) {
            index >>= 1;
            offset = index * 3;
        } else {
            index >>= 1;
            next = index + 1;
            if (next == 0x2EE) {
                next = 0;
            }
            offset = next * 3;
        }
        base = (u8 *)((offset << 4) + (s32)g_ReplayFramesGp);
        *(s32 *)(primary + 0x30) = *(s32 *)(base + 0x24);
        *(s32 *)(secondary + 0x30) = *(s32 *)(base + 0x28);
    } else {
        if ((index & 1) == 0) {
            index >>= 1;
            offset = index * 7;
        } else {
            index >>= 1;
            next = index + 1;
            if (next == 0x505) {
                next = 0;
            }
            offset = next * 7;
        }
        offset = (offset << 2) + (s32)g_ReplayFramesTimeAttack;
        *(s32 *)(primary + 0x30) = *(s32 *)(offset + 0x14);
    }
}

void GameRecordReplayFrame(void) asm("func_8001F9D8");
void GameRecordReplayFrame(void) {
    if (g_GrandPrixMode != 0) {
        func_8001F134(g_ReplayWriteCursor, &g_PlayerCar, (u8 *)g_Cars);
    } else {
        func_8001F274(g_ReplayWriteCursor, &g_PlayerCar);
    }

    g_ReplayWriteCursor++;
    if (g_ReplayWriteCursor == g_ReplayFrameCount) {
        g_ReplayWriteCursor = 0;
        g_ReplayBufferWrapped = 1;
    }
}

void GameBeginReplay(void) asm("func_8001FA70");
void GameBeginReplay(void) {
    s32 mode;

    g_FadeLevel = 0xFF;
    g_SceneTimer = 0;
    g_FadeStep = -4;

    if (g_ReplayBufferWrapped != 0) {
        g_ReplayReadCursor = (g_ReplayWriteCursor & -2) + 2;
    } else {
        g_ReplayReadCursor = 0;
        g_ReplayFrameCount = g_ReplayWriteCursor - 2;
    }

    if (!(g_ReplayReadCursor < g_ReplayFrameCount)) {
        g_ReplayReadCursor = 0;
    }

    if (g_GrandPrixMode != 0) {
        mode = g_GrandPrixClass;
        if (mode != 5) {
            func_800458CC(g_EnvScriptClock - 1800);
        }
    } else {
        mode = g_GrandPrixClass;
        if (mode != 5) {
            func_800458CC(g_EnvScriptClock - 3000);
        }
    }

    GameSeedReplayCars();
}

void GameDrawReplayBadge(void) asm("func_8001FB8C");
void GameDrawReplayBadge(void) {
    volatile s32 *scratch;
    s32 base;
    s32 next;
    s32 value;

    if ((g_SceneTimer & 0x10) && (g_SeriesCleared == 0)) {
        scratch = (volatile s32 *)0x1F800000;
        value = *scratch;
        base = (s32)g_DrawBuffer + 0xCC;
        next = func_80016EC4(base, value, 0x10, 0x10, 0x48, 0x10, 0, 0x68, 0x780D);
        *scratch = func_80017390(base, next, 9);
    }
}
