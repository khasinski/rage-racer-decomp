#include "common.h"
#include "psyq/gpu.h"
#include "game/race.h"

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/track/GameSeekEnvironmentScript", func_800458CC);

typedef struct {
    s32 id;
    u8 pad[0x2C];
} Cmd;

extern u8 g_EnvScriptEnabled asm("D_8019C8F4");
extern s32 g_EnvScriptClock asm("D_8019C8FC");
extern s32 g_EnvScriptLength asm("D_8019C774");

/* Next cue of g_EnvScriptCues due to fire; wraps back to the head when the
 * record it lands on has a negative id (the terminator). */
extern Cmd *g_EnvScriptCursor asm("D_801E40E8");
extern u32 *g_EnvScriptCues asm("D_801E42F4");

extern s16 g_EnvFogEnabled asm("D_801E3FB4");
/* Split symbols of the packed slot-0 colour: this file addresses the R, G and B
 * bytes individually to feed SetFarColor, so it declares them as three u8. */
extern u8 g_EnvFogColor asm("D_801E3FB6");
extern u8 g_EnvFogColorG asm("D_801E3FB7");
extern u8 g_EnvFogColorB asm("D_801E3FB8");
extern u8 g_EnvSpare asm("D_801E3FB9");
extern u32 g_EnvFogColorFrom asm("D_801E3FBA");

extern s16 g_EnvLerpFrame asm("D_801E4022");
extern s16 g_EnvLerpDuration asm("D_801E4024");
extern s16 g_EnvironmentMode asm("D_801E4026");
extern s16 g_EnvSpareLerp asm("D_801E4028");
extern s16 g_EnvSpareFrom asm("D_801E402A");
extern s16 g_EnvSpareTo asm("D_801E402C");
extern s32 g_EnvironmentModePrev asm("D_801E4FB0");
extern u8 *g_EnvPaletteTable asm("D_801E4140");
/* The 16-entry sky CLUT staged here and uploaded through the (0xE0, 0x1E6)
 * 16x1 VRAM rect. It MUST keep the raw D_ spelling: LA_ORDERED stringifies the
 * symbol into an inline-asm `la`, which does not follow asm() labels. */
extern s16 D_801E6DA4[];

/* GTE fog-near distance, ramped +-0xFA a frame: up to 0x7FFF (clear) in
 * environment mode 2, down to 0x1770 (hazy) in every other mode. */
extern s32 g_FogNear asm("D_8009B24C");

void GameLoadEnvironmentCue(void *arg0) asm("func_800455EC");
void GameLerpEnvColor(u8 *arg0, u8 *arg1, u8 *out, s32 arg3) asm("func_8004554C");
/* Deliberately unprototyped: the original passes only the rect and leaves
 * a1 live, so the psyq/gpu.h LoadImage prototype cannot be used here. */
void func_80065B24();
void func_80069A38(s32 arg0, s32 arg1, s32 arg2);
void func_80069B14(void *arg0, s32 arg1, void *arg2);
void func_800686D4(s32 arg0, s32 arg1);

void GameUpdateEnvironment(void) asm("func_80045CD4");
void GameUpdateEnvironment(void) {
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
        GameLoadEnvironmentCue(cur);
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
        func_80069A38(p2[0], p2[1], p2[2]);
        func_80069B14(local, frac, out);
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
    func_80065B24(&rect);

    pp = (u8 *)&g_EnvFogColorFrom;
    GameLerpEnvColor(pp + 0x0, pp + 0x4, pp - 0x4, frac);
    GameLerpEnvColor(pp + 0xC, pp + 0x10, pp + 0x8, frac);
    GameLerpEnvColor(pp + 0x18, pp + 0x1C, pp + 0x14, frac);
    GameLerpEnvColor(pp + 0x24, pp + 0x28, pp + 0x20, frac);
    GameLerpEnvColor(pp + 0x30, pp + 0x34, pp + 0x2C, frac);
    if (g_CourseIndex == 2) {
        GameLerpEnvColor(pp + 0x3C, pp + 0x40, pp + 0x38, frac);
        GameLerpEnvColor(pp + 0x48, pp + 0x4C, pp + 0x44, frac);
    } else {
        GameLerpEnvColor(pp + 0x54, pp + 0x58, pp + 0x50, frac);
        GameLerpEnvColor(pp + 0x60, pp + 0x64, pp + 0x5C, frac);
    }

    func_80069A38(g_EnvFogColor, g_EnvFogColorG, g_EnvFogColorB);

    if (g_EnvSpareLerp != 0) {
        g_EnvSpare = (g_EnvSpareFrom * diff + g_EnvSpareTo * g_EnvLerpFrame) / g_EnvLerpDuration;
    }

    if (g_EnvLerpFrame == g_EnvLerpDuration) {
        if ((*(u32 *)&g_EnvFogEnabled & 0xFFFF0000) == 0x80800000 && g_EnvFogColorB == 0x80) {
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

    func_800686D4(g_FogNear, 0x140);
}
