#include "common.h"
#include "psyq/gte.h"
#include "game/render.h"
void SetGteObjectMatrix(void *arg0, void *arg1, Matrix *mtx);

extern s32 g_ModelBankCount;
extern s32 g_FlybySceneryRotX;
extern s32 g_FlybySceneryRotY;
extern s32 g_FlybySceneryRotZ;
extern s32 g_FlybyScenery[];


void DrawFlybyScenery(void) {
    Matrix mtx0;
    Matrix mtx1;
    s32 *state;

    state = g_FlybyScenery;
    if (state[0] > 0) {
        BuildRotMatrixY(&mtx0, 0x800 - g_FlybySceneryRotY);
        BuildRotMatrixX(&mtx1, g_FlybySceneryRotX);
        MulMatrix2(&mtx0, &mtx1);
        MulMatrix2((Matrix *)0x1F800028, &mtx1);
        BuildRotMatrixZ(&mtx0, g_FlybySceneryRotZ);
        MulMatrix2(&mtx1, &mtx0);
        SelectModelBank(2);
        SetGteObjectMatrix((void *)0x1F80011C, state + 4, &mtx0);
        *(s32 *)0x1F800084 = 0;
        SubmitModel((void *)0x1F800000, g_ModelBankCount < 1);
    }
}

extern volatile s32 g_RaceSeries;
extern u8 *g_RouteSceneryData;
/* 0 while the route prop is not running; the seeder sets it to 1 and
 * UpdateRouteScenery increments it every frame, so it is both the enable
 * and the frame count since the seed. */
extern volatile s32 g_RouteSceneryClock;
extern volatile s32 g_RouteSceneryFrame;
/* Deliberately raw: the seeder's `= 1` is its only appearance in the image;
 * nothing ever reads it. */
extern volatile s16 D_801E433C;
extern volatile s16 g_RouteSceneryKeyIndex;
extern s32 g_RouteSceneryX;
extern s32 g_RouteSceneryRotX;
extern s32 g_RouteSceneryRotY;
extern s32 g_RouteSceneryRotZ;
extern u8 *g_RouteSceneryKeyframe;

void SeedRouteScenery(void) {
    register s32 index0 asm("$2");
    s32 index1;
    register u8 *base asm("$4");
    register s32 *dst asm("$8");
    s32 word0;
    register s32 word1 asm("$6");
    register s32 word2 asm("$7");
    register u8 *src asm("$2");
    register u8 *record asm("$4");
    s32 value;

    D_801E433C = 1;
    g_RouteSceneryClock = 1;

    index0 = g_RaceSeries;
    base = g_RouteSceneryData;
    index1 = g_RaceSeries;
    src = (u8 *)((index0 * 32) + (s32)base);
    __asm__ volatile("" ::: "memory");
    dst = &g_RouteSceneryX;
    __asm__ volatile("" : "=r"(dst) : "0"(dst), "r"(src) : "memory");

    word0 = *(s32 *)(src + 0x10);
    word1 = *(s32 *)(src + 0x14);
    word2 = *(s32 *)(src + 0x18);
    dst[0] = word0;
    dst[1] = word1;
    dst[2] = word2;
    word0 = *(s32 *)(src + 0x1C);
    dst[3] = word0;
    __asm__ volatile("" ::: "memory");

    index1 = index1 << 2;
    index1 = index1 + (s32)base;
    g_RouteSceneryKeyIndex = 0;
    g_RouteSceneryFrame = 0;

    index1 = *(s16 *)(index1 + 8);
    value = ((index1 * 3) << 2) + 0x50;
    record = base + value;

    g_RouteSceneryRotX = *(s16 *)(record + 0);
    g_RouteSceneryRotY = *(s16 *)(record + 2);
    value = *(s16 *)(record + 4);
    g_RouteSceneryKeyframe = record;
    g_RouteSceneryRotZ = value;
}
