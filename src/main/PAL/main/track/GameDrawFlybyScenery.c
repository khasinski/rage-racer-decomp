#include "common.h"
#include "psyq/gte.h"
#include "game/render.h"

extern s32 g_ModelBankCount asm("D_801E4168");
extern s32 g_FlybySceneryRotX asm("D_801E431C");
extern s32 g_FlybySceneryRotY asm("D_801E4320");
extern s32 g_FlybySceneryRotZ asm("D_801E4324");
extern s32 g_FlybyScenery[] asm("D_801E42FC");

void func_80017794(void *arg0, void *arg1, Matrix *mtx);

void GameDrawFlybyScenery(void) asm("func_8003EAF4");
void GameDrawFlybyScenery(void) {
    Matrix mtx0;
    Matrix mtx1;
    register Matrix *mtx0Ptr asm("$4");
    register s32 *state asm("$17");
    register Matrix *mtx1Ptr asm("$16");
    register s32 angle asm("$2");
    register s32 baseAngle asm("$5");
    register void *drawBase asm("$4");
    s32 frameValue;

    state = g_FlybyScenery;
    if (state[0] > 0) {
        mtx0Ptr = &mtx0;
        angle = g_FlybySceneryRotY;
        __asm__ volatile("" : "=r"(angle) : "0"(angle));
        baseAngle = 0x800;
        __asm__ volatile("" : "=r"(baseAngle) : "0"(baseAngle), "r"(angle));
        GameBuildRotMatrixY(mtx0Ptr, baseAngle - angle);
        mtx1Ptr = &mtx1;
        GameBuildRotMatrixX(mtx1Ptr, g_FlybySceneryRotX);
        MulMatrix2(&mtx0, mtx1Ptr);
        MulMatrix2((Matrix *)0x1F800028, mtx1Ptr);
        GameBuildRotMatrixZ(&mtx0, g_FlybySceneryRotZ);
        MulMatrix2(mtx1Ptr, &mtx0);
        GameSelectModelBank(2);
        func_80017794((void *)0x1F80011C, state + 4, &mtx0);
        frameValue = g_ModelBankCount;
        __asm__ volatile("" : "=r"(frameValue) : "0"(frameValue));
        drawBase = (void *)0x1F800000;
        *(s32 *)0x1F800084 = 0;
        GameSubmitModel(drawBase, frameValue < 1);
    }
}

extern volatile s32 g_RaceSeries asm("D_801E408C");
extern u8 *g_RouteSceneryData asm("D_801E4128");
/* 0 while the route prop is not running; the seeder sets it to 1 and
 * GameUpdateRouteScenery increments it every frame, so it is both the enable
 * and the frame count since the seed. */
extern volatile s32 g_RouteSceneryClock asm("D_801E4330");
extern volatile s32 g_RouteSceneryFrame asm("D_801E4338");
/* Deliberately raw: the seeder's `= 1` is its only appearance in the image;
 * nothing ever reads it. */
extern volatile s16 D_801E433C;
extern volatile s16 g_RouteSceneryKeyIndex asm("D_801E433E");
extern s32 g_RouteSceneryX asm("D_801E4340");
extern s32 g_RouteSceneryRotX asm("D_801E4350");
extern s32 g_RouteSceneryRotY asm("D_801E4354");
extern s32 g_RouteSceneryRotZ asm("D_801E4358");
extern u8 *g_RouteSceneryKeyframe asm("D_801E6C88");

void GameSeedRouteScenery(void) asm("func_8003EBCC");
void GameSeedRouteScenery(void) {
    register s32 index0 asm("$2");
    register s32 index1 asm("$3");
    register u8 *base asm("$4");
    register s32 *dst asm("$8");
    register s32 word0 asm("$5");
    register s32 word1 asm("$6");
    register s32 word2 asm("$7");
    register u8 *src asm("$2");
    register u8 *record asm("$4");
    register s32 value asm("$2");

    D_801E433C = 1;
    g_RouteSceneryClock = 1;

    index0 = g_RaceSeries;
    base = g_RouteSceneryData;
    index1 = g_RaceSeries;
    src = (u8 *)((index0 << 5) + (s32)base);
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
    value = index1 << 1;
    value += index1;
    value <<= 2;
    value += 0x50;
    record = base + value;

    value = *(s16 *)(record + 0);
    g_RouteSceneryRotX = value;
    value = *(s16 *)(record + 2);
    g_RouteSceneryRotY = value;
    value = *(s16 *)(record + 4);
    g_RouteSceneryKeyframe = record;
    g_RouteSceneryRotZ = value;
}
