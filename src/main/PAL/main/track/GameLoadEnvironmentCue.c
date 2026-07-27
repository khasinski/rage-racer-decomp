#include "common.h"

typedef struct {
    u32 value __attribute__((packed));
} PackedWord;

extern s16 g_EnvFogEnabled asm("D_801E3FB4");
extern PackedWord D_801E3FB6;
extern u8 D_801E3FB9;
extern PackedWord D_801E3FBA;
extern PackedWord D_801E3FBE;
extern PackedWord D_801E3FC2;
extern PackedWord D_801E3FC6;
extern PackedWord D_801E3FCA;
extern PackedWord D_801E3FCE;
extern PackedWord D_801E3FD2;
extern PackedWord D_801E3FD6;
extern PackedWord D_801E3FDA;
extern PackedWord D_801E3FDE;
extern PackedWord D_801E3FE2;
extern PackedWord D_801E3FE6;
extern PackedWord D_801E3FEA;
extern PackedWord D_801E3FEE;
extern PackedWord D_801E3FF2;
extern PackedWord D_801E3FF6;
extern PackedWord D_801E3FFA;
extern PackedWord D_801E3FFE;
extern PackedWord D_801E4002;
extern PackedWord D_801E4006;
extern PackedWord D_801E400A;
extern PackedWord D_801E400E;
extern PackedWord D_801E4012;
extern PackedWord D_801E4016;
extern PackedWord D_801E401A;
extern PackedWord D_801E401E;
extern s16 g_EnvLerpDuration asm("D_801E4024");
extern s16 g_EnvironmentMode asm("D_801E4026");
extern s16 D_801E4028;
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

    D_801E3FBE = src[1];
    D_801E3FBA = D_801E3FB6;
    D_801E3FCA = src[2];
    D_801E3FC6 = D_801E3FC2;
    D_801E3FD6 = src[3];
    D_801E3FD2 = D_801E3FCE;
    D_801E3FE2 = src[4];
    D_801E3FDE = D_801E3FDA;
    D_801E3FEE = src[5];
    D_801E3FEA = D_801E3FE6;
    D_801E3FFA = src[6];
    D_801E3FF6 = D_801E3FF2;
    D_801E4006 = src[7];
    D_801E4002 = D_801E3FFE;
    D_801E4012 = src[8];
    D_801E400E = D_801E400A;
    D_801E401E = src[9];
    D_801E401A = D_801E4016;

    field28 = *(u16 *)((u8 *)src + 0x28);
    mode = g_EnvironmentMode;
    g_EnvLerpDuration = field28;
    newMode = *(u16 *)((u8 *)src + 0x2C);
    g_EnvironmentMode = newMode;
    flag = *(u16 *)((u8 *)src + 0x2E);
    g_EnvironmentModePrev = mode;
    D_801E4028 = ((flag >> 15) ^ 1);
    compareMode = 4;

    if (D_801E4028 != 0) {
        g_EnvSpareFrom = D_801E3FB9;
        g_EnvSpareTo = *(u16 *)((u8 *)src + 0x2E);
    }

    signedMode = (s16)newMode;
    if (signedMode == compareMode) {
        g_IsEnvironmentMode4 = 1;
    } else {
        g_IsEnvironmentMode4 = 0;
    }
}
