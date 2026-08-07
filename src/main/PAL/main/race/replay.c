#include "common.h"
#include "game/race.h"
#include "game/state.h"
#include "game/render.h"
#include "game/car.h"
#include "game/replay.h"

#define AVG(a, b) (((a) + (b)) / 2)

extern u16 g_ReplayPlayerModel;
extern u16 g_ReplayRivalModel;
extern ReplayGrandPrixFrame *g_ReplayFramesGp;
extern s32 g_ReplayWriteCursor;
extern s32 g_ReplayFrameCount;
extern s32 g_ReplayBufferWrapped;
extern u8 g_PlayerCar;
extern u8 *g_EnvScriptClock;
s32 GameQueueDrawModePrimWide(s32 base, s32 next, s32 code) asm("QueueDrawModePrim");

void ApplyReplayFrame(s32 subframe, ReplayCarState *playerObj, ReplayCarState *rivalObj) {
    s32 index;
    ReplayGrandPrixFrame *big;
    ReplayTimeAttackFrame *small;
    if (g_GrandPrixMode != 0) {
        playerObj->variantAE = g_ReplayPlayerModel;
        rivalObj->variantAE = g_ReplayRivalModel;
        if ((subframe & 1) == 0) {
            index = subframe >> 1;
            big = &g_ReplayFramesGp[index];
            playerObj->x = big->x0;
            playerObj->y = big->y0;
            playerObj->z = big->z0;
            playerObj->z2 = big->z20;
            playerObj->rotX = big->rotX0;
            playerObj->rotY = big->rotY0;
            playerObj->rotZ = big->rotZ0;
            playerObj->flags48 = big->flags0;
            playerObj->field44 = big->field44_0;
            rivalObj->x = big->x1;
            rivalObj->y = big->y1;
            rivalObj->z = big->z1;
            rivalObj->z2 = big->z21;
            rivalObj->rotX = big->rotX1;
            rivalObj->rotY = big->rotY1;
            rivalObj->rotZ = big->rotZ1;
            rivalObj->flags48 = big->flags1;
            rivalObj->field44 = big->field44_1;
        } else {
            subframe = subframe >> 1;
            subframe += 1;
            if (subframe == 0x2EE) {
                subframe = 0;
            }
            big = &g_ReplayFramesGp[subframe];
            playerObj->x = AVG(big->x0, playerObj->x);
            playerObj->y = AVG(big->y0, playerObj->y);
            playerObj->z = AVG(big->z0, playerObj->z);
            playerObj->z2 = AVG(big->z20, playerObj->z2);
            playerObj->rotX = AVG(big->rotX0, playerObj->rotX);
            playerObj->rotY = AVG(big->rotY0, playerObj->rotY);
            playerObj->rotZ = AVG(big->rotZ0, playerObj->rotZ);
            playerObj->flags48 = AVG(big->flags0, playerObj->flags48);
            playerObj->field44 = AVG(big->field44_0, playerObj->field44);
            rivalObj->x = AVG(big->x1, rivalObj->x);
            rivalObj->y = AVG(big->y1, rivalObj->y);
            rivalObj->z = AVG(big->z1, rivalObj->z);
            rivalObj->z2 = AVG(big->z21, rivalObj->z2);
            rivalObj->rotX = AVG(big->rotX1, rivalObj->rotX);
            rivalObj->rotY = AVG(big->rotY1, rivalObj->rotY);
            rivalObj->rotZ = AVG(big->rotZ1, rivalObj->rotZ);
            rivalObj->flags48 = AVG(big->flags1, rivalObj->flags48);
            rivalObj->field44 = AVG(big->field44_1, rivalObj->field44);
        }
        playerObj->field8C = big->field20;
    } else {
        playerObj->variantAE = g_ReplayPlayerModel;
        if ((subframe & 1) == 0) {
            index = subframe >> 1;
            small = (ReplayTimeAttackFrame *)(g_ReplayFramesTimeAttack + ((index * 7) << 2));
            playerObj->x = small->x;
            playerObj->y = small->y;
            playerObj->z = small->z;
            playerObj->z2 = small->z2;
            playerObj->rotX = small->rotX;
            playerObj->rotY = small->rotY;
            playerObj->rotZ = small->rotZ;
            playerObj->flags48 = small->flags;
            playerObj->field44 = small->field44;
        } else {
            subframe = subframe >> 1;
            subframe += 1;
            if (subframe == 0x505) {
                subframe = 0;
            }
            small = (ReplayTimeAttackFrame *)(g_ReplayFramesTimeAttack + ((subframe * 7) << 2));
            playerObj->x = AVG(small->x, playerObj->x);
            playerObj->y = AVG(small->y, playerObj->y);
            playerObj->z = AVG(small->z, playerObj->z);
            playerObj->z2 = AVG(small->z2, playerObj->z2);
            playerObj->rotX = AVG(small->rotX, playerObj->rotX);
            playerObj->rotY = AVG(small->rotY, playerObj->rotY);
            playerObj->rotZ = AVG(small->rotZ, playerObj->rotZ);
            playerObj->flags48 = AVG(small->flags, playerObj->flags48);
            playerObj->field44 = AVG(small->field44, playerObj->field44);
        }
        playerObj->field8C = small->field10;
    }
}

void ApplyReplayFrameAndTilt(s32 subframe, u8 *playerObj, u8 *rivalObj) {
    register s32 index asm("s0");
    u8 *primary;
    u8 *secondary;
    register s32 next asm("a0");
    register s32 offset asm("v0");
    register u8 *base asm("v1");

    index = subframe;
    primary = playerObj;
    secondary = rivalObj;

    ApplyReplayFrame(index, (ReplayCarState *)primary, (ReplayCarState *)secondary);

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
        base = (u8 *)((offset * 16) + (s32)g_ReplayFramesGp);
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
        offset = (offset * 4) + (s32)g_ReplayFramesTimeAttack;
        *(s32 *)(primary + 0x30) = *(s32 *)(offset + 0x14);
    }
}

void RecordReplayFrame(void) {
    if (g_GrandPrixMode != 0) {
        StoreReplayCarFrame(g_ReplayWriteCursor, &g_PlayerCar, (u8 *)g_Cars);
    } else {
        StoreReplayTimeAttackFrame(g_ReplayWriteCursor, &g_PlayerCar);
    }

    g_ReplayWriteCursor++;
    if (g_ReplayWriteCursor == g_ReplayFrameCount) {
        g_ReplayWriteCursor = 0;
        g_ReplayBufferWrapped = 1;
    }
}

void BeginReplay(void) {
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
            SeekEnvironmentScript(g_EnvScriptClock - 1800);
        }
    } else {
        mode = g_GrandPrixClass;
        if (mode != 5) {
            SeekEnvironmentScript(g_EnvScriptClock - 3000);
        }
    }

    SeedReplayCars();
}

void DrawReplayBadge(void) {
    volatile s32 *scratch;
    s32 base;
    s32 next;
    s32 value;

    if ((g_SceneTimer & 0x10) && (g_SeriesCleared == 0)) {
        scratch = (volatile s32 *)0x1F800000;
        value = *scratch;
        base = (s32)g_DrawBuffer + 0xCC;
        next = (s32)GameQueueSprite(base, value, 0x10, 0x10, 0x48, 0x10, 0, 0x68, 0x780D);
        *scratch = GameQueueDrawModePrimWide(base, next, 9);
    }
}
