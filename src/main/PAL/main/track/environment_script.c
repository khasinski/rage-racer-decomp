#include "common.h"
#include "psyq/gpu.h"
#include "game/race.h"
#include "game/render.h"

typedef struct Cmd {
    s32 id;
    GameEnvColor colors[9];
    u16 duration;
    u16 unk2A;
    u16 mode;
    u16 unk2E;
} Cmd;

extern u8 g_EnvScriptEnabled;
extern s32 g_EnvScriptClock;
extern s32 g_EnvScriptLength;

/* Next cue of g_EnvScriptCues due to fire; wraps back to the head when the
 * record it lands on has a negative id (the terminator). */
extern Cmd *g_EnvScriptCursor;
extern u32 *g_EnvScriptCues;

extern s16 g_EnvFogEnabled;
/* The three bytes of the packed slot-0 colour g_EnvColors[0].cur, addressed
 * individually because SetFarColor takes R, G and B separately. The cast is
 * what the byte view costs: render.h is included here, so this TU cannot
 * redeclare g_EnvColors as u8[] under a second name. g_EnvSpare is the fourth,
 * unused byte of the same word. */
#define ENV_FOG_RGB ((u8 *)g_EnvColors)
extern u8 g_EnvSpare;

/* LerpEnvColor works in bytes, so the eight lerps below address the slots
 * of g_EnvColors through one byte cursor `pp` = &g_EnvColors[0].from. Slot
 * stride 0x0C, field order { cur, from, to }. */
#define ENV_CUR(k) (pp + 0x0C * (k) - 4)
#define ENV_FROM(k) (pp + 0x0C * (k))
#define ENV_TO(k) (pp + 0x0C * (k) + 4)

extern s16 g_EnvLerpFrame;
extern s16 g_EnvLerpDuration;
extern s16 g_EnvSpareLerp;
extern s16 g_EnvSpareFrom;
extern s16 g_EnvSpareTo;
/* The 16-entry sky CLUT staged here and uploaded through the (0xE0, 0x1E6)
 * 16x1 VRAM rect. It MUST keep the raw D_ spelling: LA_ORDERED stringifies the
 * symbol into an inline-asm `la`, which does not follow asm() labels. */
extern s16 D_801E6DA4[];

/* GTE fog-near distance, ramped +-0xFA a frame: up to 0x7FFF (clear) in
 * environment mode 2, down to 0x1770 (hazy) in every other mode. */
extern s32 g_FogNear;

/* Deliberately unprototyped: the seek path also leaves the cue duration and
 * clamped frame in a1/a2, while the normal update path passes only the cue. */
void LoadEnvironmentCue();
void LerpEnvColor(u8 *arg0, u8 *arg1, u8 *out, s32 arg3);

void SeekEnvironmentScript(s32 targetTime) {
    s32 clock;
    u32 count;
    s32 offset;
    s32 tailCount;
    s32 nextId;
    s32 duration;
    s32 frame;
    s32 clampedFrame;
    s32 signedFrame;
    s32 fog;
    Cmd *cue;
    u8 *rgb;
    s16 *fogOut;
    s16 *fogTarget;

    clock = (targetTime + g_EnvScriptLength) % g_EnvScriptLength;
    targetTime = (s32)g_EnvScriptCues;
    g_EnvScriptCursor = (Cmd *)targetTime;
    g_EnvScriptClock = clock;
    for (count = 0;
         (s32)((u32 *)targetTime)[count * 12] != -1;
         count++) {
        if (clock < (s32)((u32 *)targetTime)[count * 12]) {
            break;
        }
    }

    if ((s32)count >= 2) {
        offset = count * 0x30 - 0x60;
        offset += (s32)g_EnvScriptCursor;
        g_EnvScriptCursor = (Cmd *)offset;
    } else {
        targetTime = (s32)g_EnvScriptCursor;
        for (tailCount = 0;
             (s32)((u32 *)targetTime)[(tailCount + 1) * 12] != -1;
             tailCount++) {
        }
        offset = tailCount * 0x30;
        offset += (s32)g_EnvScriptCursor;
        g_EnvScriptCursor = (Cmd *)offset;
    }

    rgb = (u8 *)g_EnvColors;
    g_EnvColors[0].cur =
        *(GameEnvColor *)((u8 *)g_EnvScriptCursor + 0x04);
    g_EnvColors[1].cur =
        *(GameEnvColor *)((u8 *)g_EnvScriptCursor + 0x08);
    g_EnvColors[2].cur =
        *(GameEnvColor *)((u8 *)g_EnvScriptCursor + 0x0C);
    g_EnvColors[3].cur =
        *(GameEnvColor *)((u8 *)g_EnvScriptCursor + 0x10);
    g_EnvColors[4].cur =
        *(GameEnvColor *)((u8 *)g_EnvScriptCursor + 0x14);
    g_EnvColors[5].cur =
        *(GameEnvColor *)((u8 *)g_EnvScriptCursor + 0x18);
    g_EnvColors[6].cur =
        *(GameEnvColor *)((u8 *)g_EnvScriptCursor + 0x1C);
    g_EnvColors[7].cur =
        *(GameEnvColor *)((u8 *)g_EnvScriptCursor + 0x20);
    g_EnvColors[8].cur =
        *(GameEnvColor *)((u8 *)g_EnvScriptCursor + 0x24);

    g_EnvironmentMode = g_EnvScriptCursor->mode;
    nextId = *(s32 *)((u8 *)g_EnvScriptCursor + 0x30);
    g_EnvScriptCursor = g_EnvScriptCursor + 1;
    if (nextId < 0) {
        g_EnvScriptCursor = (Cmd *)g_EnvScriptCues;
    }

    cue = g_EnvScriptCursor;
    duration = cue->duration;
    g_EnvLerpDuration = duration;
    frame = (u16)g_EnvScriptClock - *(u16 *)cue;
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

    nextId = *(s32 *)((u8 *)g_EnvScriptCursor + 0x30);
    g_EnvScriptCursor = g_EnvScriptCursor + 1;
    if (nextId < 0) {
        g_EnvScriptCursor = (Cmd *)g_EnvScriptCues;
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
    Cmd *cur;
    u8 *pp;

    if (g_EnvScriptEnabled == 0) {
        return;
    }

    cur = g_EnvScriptCursor;
    if (cur->id == g_EnvScriptClock) {
        g_EnvLerpFrame = 0;
        g_EnvScriptCursor = cur + 1;
        LoadEnvironmentCue(cur);
        if (g_EnvScriptCursor->id < 0) {
            g_EnvScriptCursor = (Cmd *)g_EnvScriptCues;
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
            LA_ORDERED(palo, D_801E6DA4, idx);
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
