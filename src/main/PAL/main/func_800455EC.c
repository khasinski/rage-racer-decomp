#include "common.h"

typedef struct {
    u32 value;
} __attribute__((packed)) PackedWord;

extern s16 D_801E3FB4;
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
extern s16 D_801E4024;
extern s16 D_801E4026;
extern s16 D_801E4028;
extern s16 D_801E402A;
extern s16 D_801E402C;
extern s32 D_801E4030;
extern s32 D_801E4FB0;

void func_800455EC(PackedWord *arg0) {
    register PackedWord *src asm("a2");
    register s32 mode asm("v1");
    register s32 newMode asm("a0");
    register s32 compareMode asm("v1");
    register s32 signedMode asm("v0");
    s32 field28;
    register u32 flag asm("v0");

    src = arg0;
    asm volatile("" : "=r"(src) : "0"(src));
    D_801E3FB4 = 1;

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
    mode = D_801E4026;
    D_801E4024 = field28;
    newMode = *(u16 *)((u8 *)src + 0x2C);
    D_801E4026 = newMode;
    flag = *(u16 *)((u8 *)src + 0x2E);
    D_801E4FB0 = mode;
    D_801E4028 = ((flag >> 15) ^ 1);
    compareMode = 4;

    if (D_801E4028 != 0) {
        D_801E402A = D_801E3FB9;
        D_801E402C = *(u16 *)((u8 *)src + 0x2E);
    }

    signedMode = (s16)newMode;
    if (signedMode == compareMode) {
        D_801E4030 = 1;
    } else {
        D_801E4030 = 0;
    }
}
