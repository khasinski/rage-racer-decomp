#include "common.h"

typedef struct {
    u32 value __attribute__((packed));
} PackedWord;

extern s16 g_EnvFogEnabled asm("D_801E3FB4");
/*
 * The environment colour state: nine 12-byte { cur, from, to } packed RGB slots
 * starting here. Slot 0 is the GTE far/fog colour (SetFarColor takes its three
 * bytes), slots 1..8 the sky-gradient bands. A cue rolls every slot's current
 * value into `From` and the cue's own value into `To`; GameUpdateEnvironment
 * then walks `cur` from one to the other over g_EnvLerpDuration frames.
 * g_EnvSpare is the fourth, otherwise unused byte of slot 0.
 */
extern PackedWord g_EnvFogColor asm("D_801E3FB6");
extern u8 g_EnvSpare asm("D_801E3FB9");
extern PackedWord g_EnvFogColorFrom asm("D_801E3FBA");
extern PackedWord g_EnvFogColorTo asm("D_801E3FBE");
extern PackedWord g_EnvColor1 asm("D_801E3FC2");
extern PackedWord g_EnvColor1From asm("D_801E3FC6");
extern PackedWord g_EnvColor1To asm("D_801E3FCA");
extern PackedWord g_EnvColor2 asm("D_801E3FCE");
extern PackedWord g_EnvColor2From asm("D_801E3FD2");
extern PackedWord g_EnvColor2To asm("D_801E3FD6");
extern PackedWord g_EnvColor3 asm("D_801E3FDA");
extern PackedWord g_EnvColor3From asm("D_801E3FDE");
extern PackedWord g_EnvColor3To asm("D_801E3FE2");
extern PackedWord g_EnvColor4 asm("D_801E3FE6");
extern PackedWord g_EnvColor4From asm("D_801E3FEA");
extern PackedWord g_EnvColor4To asm("D_801E3FEE");
extern PackedWord g_EnvColor5 asm("D_801E3FF2");
extern PackedWord g_EnvColor5From asm("D_801E3FF6");
extern PackedWord g_EnvColor5To asm("D_801E3FFA");
extern PackedWord g_EnvColor6 asm("D_801E3FFE");
extern PackedWord g_EnvColor6From asm("D_801E4002");
extern PackedWord g_EnvColor6To asm("D_801E4006");
extern PackedWord g_EnvColor7 asm("D_801E400A");
extern PackedWord g_EnvColor7From asm("D_801E400E");
extern PackedWord g_EnvColor7To asm("D_801E4012");
extern PackedWord g_EnvColor8 asm("D_801E4016");
extern PackedWord g_EnvColor8From asm("D_801E401A");
extern PackedWord g_EnvColor8To asm("D_801E401E");
extern s16 g_EnvLerpDuration asm("D_801E4024");
extern s16 g_EnvironmentMode asm("D_801E4026");
/* Cue halfword +0x2E, bit 15 inverted. Its only effect anywhere is to enable
 * the g_EnvSpareFrom -> g_EnvSpareTo lerp of g_EnvSpare; the name states that
 * and nothing more, because no reader of g_EnvSpare survives in the image. */
extern s16 g_EnvSpareLerp asm("D_801E4028");
extern s16 g_EnvSpareFrom asm("D_801E402A");
extern s16 g_EnvSpareTo asm("D_801E402C");
extern s32 g_IsEnvironmentMode4 asm("D_801E4030");
extern s32 g_EnvironmentModePrev asm("D_801E4FB0");

void GameLoadEnvironmentCue(PackedWord *arg0) asm("func_800455EC");
void GameLoadEnvironmentCue(PackedWord *arg0) {
    register PackedWord *src asm("a2");
    register s32 mode asm("v1");
    register s32 newMode asm("a0");
    register s32 compareMode asm("v1");
    register s32 signedMode asm("v0");
    s32 field28;
    register u32 flag asm("v0");

    src = arg0;
    asm volatile("" : "=r"(src) : "0"(src));
    g_EnvFogEnabled = 1;

    g_EnvFogColorTo = src[1];
    g_EnvFogColorFrom = g_EnvFogColor;
    g_EnvColor1To = src[2];
    g_EnvColor1From = g_EnvColor1;
    g_EnvColor2To = src[3];
    g_EnvColor2From = g_EnvColor2;
    g_EnvColor3To = src[4];
    g_EnvColor3From = g_EnvColor3;
    g_EnvColor4To = src[5];
    g_EnvColor4From = g_EnvColor4;
    g_EnvColor5To = src[6];
    g_EnvColor5From = g_EnvColor5;
    g_EnvColor6To = src[7];
    g_EnvColor6From = g_EnvColor6;
    g_EnvColor7To = src[8];
    g_EnvColor7From = g_EnvColor7;
    g_EnvColor8To = src[9];
    g_EnvColor8From = g_EnvColor8;

    field28 = *(u16 *)((u8 *)src + 0x28);
    mode = g_EnvironmentMode;
    g_EnvLerpDuration = field28;
    newMode = *(u16 *)((u8 *)src + 0x2C);
    g_EnvironmentMode = newMode;
    flag = *(u16 *)((u8 *)src + 0x2E);
    g_EnvironmentModePrev = mode;
    g_EnvSpareLerp = ((flag >> 15) ^ 1);
    compareMode = 4;

    if (g_EnvSpareLerp != 0) {
        g_EnvSpareFrom = g_EnvSpare;
        g_EnvSpareTo = *(u16 *)((u8 *)src + 0x2E);
    }

    signedMode = (s16)newMode;
    if (signedMode == compareMode) {
        g_IsEnvironmentMode4 = 1;
    } else {
        g_IsEnvironmentMode4 = 0;
    }
}
