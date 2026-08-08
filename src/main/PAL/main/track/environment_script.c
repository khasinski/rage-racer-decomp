#include "common.h"
#include "psyq/gpu.h"
#include "game/race.h"
#include "game/render.h"
#include "game/sound.h"
#include "game/track.h"

#include "game/track_internal.h"

void SeekEnvironmentScript(s32 targetTime) {
    s32 clock;
    u32 count;
    s32 tailCount;
    s32 nextId;
    s32 duration;
    s32 frame;
    s32 clampedFrame;
    s32 signedFrame;
    s32 fog;
    GameEnvironmentCue *cue;
    s16 *fogOut;
    s16 *fogTarget;

    clock = (targetTime + g_EnvScriptLength) % g_EnvScriptLength;
    targetTime = (s32)g_EnvScriptCues;
    g_EnvScriptCursor = (GameEnvironmentCue *)targetTime;
    g_EnvScriptClock = clock;
    for (count = 0;
         ((GameEnvironmentCue *)targetTime)[count].time != -1;
         count++) {
        if (clock < ((GameEnvironmentCue *)targetTime)[count].time) {
            break;
        }
    }

    if ((s32)count >= 2) {
        g_EnvScriptCursor += count - 2;
    } else {
        targetTime = (s32)g_EnvScriptCursor;
        for (tailCount = 0;
             ((GameEnvironmentCue *)targetTime)[tailCount + 1].time != -1;
             tailCount++) {
        }
        g_EnvScriptCursor += tailCount;
    }

    g_EnvColors[0].cur = g_EnvScriptCursor->colors[0];
    g_EnvColors[1].cur = g_EnvScriptCursor->colors[1];
    g_EnvColors[2].cur = g_EnvScriptCursor->colors[2];
    g_EnvColors[3].cur = g_EnvScriptCursor->colors[3];
    g_EnvColors[4].cur = g_EnvScriptCursor->colors[4];
    g_EnvColors[5].cur = g_EnvScriptCursor->colors[5];
    g_EnvColors[6].cur = g_EnvScriptCursor->colors[6];
    g_EnvColors[7].cur = g_EnvScriptCursor->colors[7];
    g_EnvColors[8].cur = g_EnvScriptCursor->colors[8];

    g_EnvironmentMode = g_EnvScriptCursor->mode;
    nextId = RAW(g_EnvScriptCursor[1].time);
    g_EnvScriptCursor = g_EnvScriptCursor + 1;
    if (nextId < 0) {
        g_EnvScriptCursor = g_EnvScriptCues;
    }

    cue = g_EnvScriptCursor;
    duration = cue->duration;
    g_EnvLerpDuration = duration;
    frame = (u16)g_EnvScriptClock - (u16)RAW(cue->time);
    g_EnvLerpFrame = frame;
    clampedFrame = frame;
    /* Keep the unclamped store and the call-value copy as distinct lifetimes. */
    asm("" : "=r"(clampedFrame) : "0"(clampedFrame));
    signedFrame = (s16)frame;
    if ((s16)duration < signedFrame) {
        clampedFrame = duration;
    }
    g_EnvLerpFrame = clampedFrame;
    LoadEnvironmentCue(cue, duration, clampedFrame);

    nextId = RAW(g_EnvScriptCursor[1].time);
    g_EnvScriptCursor = g_EnvScriptCursor + 1;
    if (nextId < 0) {
        g_EnvScriptCursor = g_EnvScriptCues;
    }

    fogOut = (s16 *)((u8 *)&g_EnvColors[0].from - 6);
    g_EnvScriptEnabled = 1;
    *fogOut = 1;
    UpdateEnvironment();

    fog = 0;
    if (g_GrandPrixClass >= 5) {
        g_EnvScriptEnabled = 0;
    }
    fogTarget = fogOut;
    if ((*(u32 *)fogOut & 0xFFFF0000) != 0x80800000 ||
        g_EnvColors[0].cur.bytes.b != 0x80) {
        fog = 1;
    }
    *fogTarget = fog;
    SetFarColor(g_EnvColors[0].cur.bytes.r,
                g_EnvColors[0].cur.bytes.g,
                g_EnvColors[0].cur.bytes.b);

    if (g_EnvironmentMode == 2) {
        g_FogNear = 0x7FFF;
    } else {
        g_FogNear = 0x1770;
    }
    SetFogNear(g_FogNear, 0x140);
}

void UpdateEnvironment(void) {
    Rect rect;
    s32 local[3];
    u8 out[4];
    s32 i;
    s32 diff;
    s32 frac;
    u8 *p1;
    u8 *p2;
    GameEnvironmentCue *cur;

    if (g_EnvScriptEnabled == 0) {
        return;
    }

    cur = g_EnvScriptCursor;
    if (cur->time == g_EnvScriptClock) {
        g_EnvLerpFrame = 0;
        g_EnvScriptCursor = cur + 1;
        LoadEnvironmentCue(cur);
        if (g_EnvScriptCursor->time < 0) {
            g_EnvScriptCursor = g_EnvScriptCues;
        }
    }

    g_EnvScriptClock = (g_EnvScriptClock < g_EnvScriptLength) ? g_EnvScriptClock + 1 : 0;

    if (g_EnvFogEnabled == 0) {
        return;
    }

    if (g_EnvScriptEnabled != 0) {
        if (g_EnvLerpFrame < g_EnvLerpDuration) {
            g_EnvLerpFrame = g_EnvLerpFrame + 1;
        }
    }

    diff = g_EnvLerpDuration - g_EnvLerpFrame;
    frac = (g_EnvLerpFrame << 12) / g_EnvLerpDuration;

    for (i = 0; i < 0x10; i++) {
        s16 *dst;
        p1 = (u8 *)(i * 3) + (g_EnvironmentModePrev * 48 + (s32)g_EnvPaletteTable);
        local[0] = p1[0] << 4;
        local[1] = p1[1] << 4;
        local[2] = p1[2] << 4;
        p2 = (u8 *)(i * 3) + (g_EnvironmentMode * 48 + (s32)g_EnvPaletteTable);
        SetFarColor(p2[0], p2[1], p2[2]);
        Intpl(local, frac, out);
        {
            u8 *idx = (u8 *)(i * 2);
            register s32 palo asm("$5");
            LA_ORDERED(palo, g_EnvironmentClut, idx);
            dst = (s16 *)(idx + palo);
        }
        *dst = 0;
        *dst = out[0];
        *dst |= out[1] << 5;
        *dst |= out[2] << 10;
    }

    rect.x = 0xE0;
    rect.y = 0x1E6;
    rect.w = 0x10;
    rect.h = 0x1;
    LoadImage(&rect);

    LerpEnvColor(&g_EnvColors[0].from, &g_EnvColors[0].to,
                 &g_EnvColors[0].cur, frac);
    LerpEnvColor(&g_EnvColors[1].from, &g_EnvColors[1].to,
                 &g_EnvColors[1].cur, frac);
    LerpEnvColor(&g_EnvColors[2].from, &g_EnvColors[2].to,
                 &g_EnvColors[2].cur, frac);
    LerpEnvColor(&g_EnvColors[3].from, &g_EnvColors[3].to,
                 &g_EnvColors[3].cur, frac);
    LerpEnvColor(&g_EnvColors[4].from, &g_EnvColors[4].to,
                 &g_EnvColors[4].cur, frac);
    if (g_CourseIndex == 2) {
        LerpEnvColor(&g_EnvColors[5].from, &g_EnvColors[5].to,
                     &g_EnvColors[5].cur, frac);
        LerpEnvColor(&g_EnvColors[6].from, &g_EnvColors[6].to,
                     &g_EnvColors[6].cur, frac);
    } else {
        LerpEnvColor(&g_EnvColors[7].from, &g_EnvColors[7].to,
                     &g_EnvColors[7].cur, frac);
        LerpEnvColor(&g_EnvColors[8].from, &g_EnvColors[8].to,
                     &g_EnvColors[8].cur, frac);
    }

    SetFarColor(g_EnvColors[0].cur.bytes.r,
                g_EnvColors[0].cur.bytes.g,
                g_EnvColors[0].cur.bytes.b);

    if (g_EnvSpareLerp != 0) {
        g_EnvSpare = (g_EnvSpareFrom * diff + g_EnvSpareTo * g_EnvLerpFrame) / g_EnvLerpDuration;
    }

    if (g_EnvLerpFrame == g_EnvLerpDuration) {
        if ((*(u32 *)&g_EnvFogEnabled & 0xFFFF0000) == 0x80800000 &&
            g_EnvColors[0].cur.bytes.b == 0x80) {
            g_EnvFogEnabled = 0;
        }
    }

    if (g_EnvironmentMode == 2) {
        g_FogNear += 0xFA;
        if (g_FogNear > 0x7FFF) {
            g_FogNear = 0x7FFF;
        }
    } else {
        g_FogNear -= 0xFA;
        if (g_FogNear < 0x1770) {
            g_FogNear = 0x1770;
        }
    }

    SetFogNear(g_FogNear, 0x140);
}
