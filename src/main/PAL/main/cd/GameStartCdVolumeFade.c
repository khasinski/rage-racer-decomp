#include "common.h"

extern s32 g_CdFadeFrames asm("D_8009B1B4");

void GameStartCdVolumeFade(s32 arg0) asm("func_80042CCC");
void GameStartCdVolumeFade(s32 arg0) {
    g_CdFadeFrames = arg0;
    if (arg0 >= 0x1000) {
        g_CdFadeFrames = 0xFFF;
    }
    if (g_CdFadeFrames < -0xFFF) {
        g_CdFadeFrames = -0xFFF;
    }
}

extern u32 g_CdMixLL asm("D_8009B174");
extern u32 g_CdMixLR asm("D_8009B178");
extern u32 g_CdMixRR asm("D_8009B17C");
extern u32 g_CdMixRL asm("D_8009B180");
extern u32 g_CdMixFullLL asm("D_8009B184");
extern u32 g_CdMixFullLR asm("D_8009B188");
extern u32 g_CdMixFullRR asm("D_8009B18C");
extern u32 g_CdMixFullRL asm("D_8009B190");

void CdMix(u8 *arg0) asm("func_8006A94C");

void GameStepCdVolumeFade(void) asm("func_80042D10");
void GameStepCdVolumeFade(void) {
    u8 buf[4];
    register s32 cnt asm("$13");

    cnt = g_CdFadeFrames;
    if (cnt > 0) {
        register u32 *p asm("$7");
        register s32 n asm("$5");
        register u32 q1 asm("$6");
        register u32 q2 asm("$4");
        register u32 q3 asm("$3");
        register u32 q4 asm("$2");

        p = &g_CdMixLL;
        n = cnt - 1;
        q1 = (*p * n) / cnt;
        q2 = (g_CdMixLR * n) / cnt;
        q3 = (g_CdMixRR * n) / cnt;
        q4 = (g_CdMixRL * n) / cnt;
        g_CdFadeFrames = n;
        *p = q1;
        g_CdMixLR = q2;
        g_CdMixRR = q3;
        g_CdMixRL = q4;
    } else if (cnt < 0) {
        register u32 *p asm("$12");
        register u32 v184 asm("$11");
        register u32 inv asm("$4");
        register u32 div2 asm("$3");
        register u32 d1 asm("$9");
        register u32 v188 asm("$10");
        register u32 d2 asm("$7");
        register u32 v18C asm("$8");
        register u32 d3 asm("$5");
        register u32 v190 asm("$6");
        register u32 d4 asm("$4");
        register s32 c2 asm("$2");

        p = &g_CdMixLL;
        v184 = g_CdMixFullLL;
        inv = ~cnt;
        div2 = inv + 1;
        d1 = ((v184 - *p) * inv) / div2;
        v188 = g_CdMixFullLR;
        d2 = ((v188 - g_CdMixLR) * inv) / div2;
        v18C = g_CdMixFullRR;
        d3 = ((v18C - g_CdMixRR) * inv) / div2;
        v190 = g_CdMixFullRL;
        d4 = ((v190 - g_CdMixRL) * inv) / div2;
        v184 = v184 - d1;
        *p = v184;
        c2 = cnt + 1;
        g_CdFadeFrames = c2;
        v188 = v188 - d2;
        g_CdMixLR = v188;
        v18C = v18C - d3;
        g_CdMixRR = v18C;
        v190 = v190 - d4;
        g_CdMixRL = v190;
    }
    buf[0] = g_CdMixLL >> 12;
    buf[1] = g_CdMixLR >> 12;
    buf[2] = g_CdMixRR >> 12;
    buf[3] = g_CdMixRL >> 12;
    CdMix(buf);
}
