#include "common.h"
#include "psyq/gpu.h"
#include "game/race.h"
#include "game/render.h"
#include "game/sound.h"
#include "game/track.h"

#include "game/track_internal.h"

/* The three bytes of the packed slot-0 colour g_EnvColors[0].cur, addressed
 * individually because SetFarColor takes R, G and B separately. The cast is
 * what the byte view costs: render.h is included here, so this TU cannot
 * redeclare g_EnvColors as u8[] under a second name. g_EnvSpare is the fourth,
 * unused byte of the same word. */
#define ENV_FOG_RGB ((u8 *)g_EnvColors)

/* LerpEnvColor works in bytes, so the eight lerps below address the slots
 * of g_EnvColors through one byte cursor `pp` = &g_EnvColors[0].from. Slot
 * stride 0x0C, field order { cur, from, to }. */
#define ENV_CUR(k) (pp + 0x0C * (k) - 4)
#define ENV_FROM(k) (pp + 0x0C * (k))
#define ENV_TO(k) (pp + 0x0C * (k) + 4)

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
    u8 *rgb;
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

    rgb = (u8 *)g_EnvColors;
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
        g_EnvScriptCursor = (GameEnvironmentCue *)g_EnvScriptCues;
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
        g_EnvScriptCursor = (GameEnvironmentCue *)g_EnvScriptCues;
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
        ((u8 *)g_EnvColors)[2] != 0x80) {
        fog = 1;
    }
    *fogTarget = fog;
    SetFarColor(((u8 *)g_EnvColors)[0],
                ((u8 *)g_EnvColors)[1],
                ((u8 *)g_EnvColors)[2]);

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
    u8 *pp;

    if (g_EnvScriptEnabled == 0) {
        return;
    }

    cur = g_EnvScriptCursor;
    if (cur->time == g_EnvScriptClock) {
        g_EnvLerpFrame = 0;
        g_EnvScriptCursor = cur + 1;
        LoadEnvironmentCue(cur);
        if (g_EnvScriptCursor->time < 0) {
            g_EnvScriptCursor = (GameEnvironmentCue *)g_EnvScriptCues;
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

    /* cur = lerp(from, to, frac) for slots 0..4, then one of the two alternate
     * pairs. Slot 0 is the fog colour SetFarColor gets three lines below. */
    pp = (u8 *)&g_EnvColors[0].from;
    LerpEnvColor(ENV_FROM(0), ENV_TO(0), ENV_CUR(0), frac);
    LerpEnvColor(ENV_FROM(1), ENV_TO(1), ENV_CUR(1), frac);
    LerpEnvColor(ENV_FROM(2), ENV_TO(2), ENV_CUR(2), frac);
    LerpEnvColor(ENV_FROM(3), ENV_TO(3), ENV_CUR(3), frac);
    LerpEnvColor(ENV_FROM(4), ENV_TO(4), ENV_CUR(4), frac);
    if (g_CourseIndex == 2) {
        LerpEnvColor(ENV_FROM(5), ENV_TO(5), ENV_CUR(5), frac);
        LerpEnvColor(ENV_FROM(6), ENV_TO(6), ENV_CUR(6), frac);
    } else {
        LerpEnvColor(ENV_FROM(7), ENV_TO(7), ENV_CUR(7), frac);
        LerpEnvColor(ENV_FROM(8), ENV_TO(8), ENV_CUR(8), frac);
    }

    SetFarColor(ENV_FOG_RGB[0], ENV_FOG_RGB[1], ENV_FOG_RGB[2]);

    if (g_EnvSpareLerp != 0) {
        g_EnvSpare = (g_EnvSpareFrom * diff + g_EnvSpareTo * g_EnvLerpFrame) / g_EnvLerpDuration;
    }

    if (g_EnvLerpFrame == g_EnvLerpDuration) {
        if ((*(u32 *)&g_EnvFogEnabled & 0xFFFF0000) == 0x80800000 && ENV_FOG_RGB[2] == 0x80) {
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
