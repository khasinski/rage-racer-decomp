#include "common.h"
#include "psyq/gte.h"
#include "game/render.h"
#include "game/race.h"
#include "game/track.h"

extern s32 g_ModelBankCount asm("D_801E4168");
extern s32 g_RouteSceneryX asm("D_801E4340");
extern s32 g_RouteSceneryRotX asm("D_801E4350");
extern volatile s32 g_RouteSceneryRotY asm("D_801E4354");
extern s32 g_RouteSceneryRotZ asm("D_801E4358");

void func_80017794(void *arg0, void *arg1, Matrix *mtx);


void GameDrawRouteScenery(void) asm("func_8003F02C");
void GameDrawRouteScenery(void) {
    Matrix mtx0;
    Matrix mtx1;
    register Matrix *mtx1Ptr asm("$16");
    s32 frameValue;
    s32 drawId;

    GameBuildRotMatrixY(&mtx0, 0x800 - g_RouteSceneryRotY);
    mtx1Ptr = &mtx1;
    GameBuildRotMatrixX(mtx1Ptr, g_RouteSceneryRotX);
    MulMatrix2(&mtx0, mtx1Ptr);
    MulMatrix2((Matrix *)0x1F800028, mtx1Ptr);
    GameBuildRotMatrixZ(&mtx0, g_RouteSceneryRotZ);
    MulMatrix2(mtx1Ptr, &mtx0);
    GameSelectModelBank(1);
    func_80017794((void *)0x1F80011C, &g_RouteSceneryX, &mtx0);
    frameValue = g_ModelBankCount;
    *(s32 *)0x1F800084 = 0;
    drawId = 1;
    if (frameValue >= 0x26) {
        drawId = 0x25;
    }
    GameSubmitModel((void *)0x1F800000, drawId);
}

/*
 * g_ShuttleScenery[1]'s fields, which retail reaches through their own split
 * symbols rather than by indexing the array. See GameShuttleScenery in
 * game/track.h for what each field does.
 */
extern s32 g_Shuttle1DwellCounter asm("D_801E4FEC");
extern s32 g_Shuttle1TravelStep asm("D_801E4FF4");
extern s16 g_Shuttle1StartEndpoint asm("D_801E4FF8");
extern s16 g_Shuttle1PathIndex asm("D_801E4FFA");
extern s32 g_Shuttle1X asm("D_801E4FFC");
extern s32 g_Shuttle1Y asm("D_801E5000");
extern s32 g_Shuttle1Z asm("D_801E5004");
extern s32 g_Shuttle1Unk1C asm("D_801E5008");
extern s32 g_Shuttle1AngleX asm("D_801E500C");
extern s32 g_Shuttle1AngleY asm("D_801E5010");
extern s32 g_Shuttle1AngleZ asm("D_801E5014");
/*
 * The shuttle's authored paths, indexed by GameShuttleScenery.pathIndex:
 * g_ShuttlePathPoints is 0x20 bytes per path, two 16-byte { x, y, z, unk }
 * endpoints; g_ShuttlePathAngles 8 bytes per path, three s16 Euler angles;
 * g_ShuttlePathDwellMax one s16 per path, the frames held at an endpoint.
 * g_ShuttlePath2Points is the split symbol for &g_ShuttlePathPoints[2].
 */
extern s32 g_ShuttlePath2Points[] asm("D_8007E3A0");
extern s32 g_ShuttlePathPoints[] asm("D_8007E360");
extern s16 g_ShuttlePathAngles[] asm("D_8007E3C0");
extern s16 g_ShuttlePathDwellMax[] asm("D_8007E3E0");

void GameInitShuttleScenery(void) asm("func_8003F0F8");

void GameInitShuttleScenery(void) {
    register GameShuttleScenery *state asm("$7");
    register s32 *src asm("$5");
    register s32 *dst asm("$4");
    register s32 index asm("$4");
    register s32 value asm("$2");
    register s32 v1 asm("$3");
    register s32 a4 asm("$4");
    register s32 a5 asm("$5");
    register s32 a6 asm("$6");

    state = &g_ShuttleScenery[0];
    if ((g_CourseIndex & 3) == 2) {
        g_Shuttle1PathIndex = 2;
        asm("" ::: "memory");
        src = g_ShuttlePath2Points;
        dst = &g_Shuttle1X;
        asm("" : "=r"(src), "=r"(dst) : "0"(src), "1"(dst));
        value = src[0];
        v1 = src[1];
        dst[0] = value;
        dst[1] = v1;
        value = src[2];
        v1 = src[3];
        dst[2] = value;
        dst[3] = v1;

        asm("" ::: "memory");
        index = g_Shuttle1PathIndex;
        v1 = index << 3;
        g_Shuttle1AngleX = *(s16 *)((s32)g_ShuttlePathAngles + v1);
        g_Shuttle1AngleY = *(s16 *)((s32)g_ShuttlePathAngles + v1 + 2);
        value = *(s16 *)((s32)g_ShuttlePathAngles + v1 + 4);
        index <<= 1;
        g_Shuttle1StartEndpoint = 0;
        g_Shuttle1TravelStep = 0;
        g_Shuttle1AngleZ = value;
        v1 = *(s16 *)((s32)g_ShuttlePathDwellMax + index);
        state->pathIndex = 1;
        g_Shuttle1DwellCounter = v1;
        asm(".globl func_8003F1D0\nfunc_8003F1D0 = func_8003F0F8 + 0xD8");
        goto updateState;
    }

    state->pathIndex = 0;
updateState:
    asm("" ::: "memory");
    asm("" : "=r"(state) : "0"(state));
    value = state->pathIndex;
    value <<= 5;
    v1 = *(s32 *)((s32)g_ShuttlePathPoints + value);
    a4 = *(s32 *)((s32)g_ShuttlePathPoints + value + 4);
    a5 = *(s32 *)((s32)g_ShuttlePathPoints + value + 8);
    a6 = *(s32 *)((s32)g_ShuttlePathPoints + value + 12);
    state->x = v1;
    state->y = a4;
    state->z = a5;
    state->unk1C = a6;
    asm("" ::: "memory");
    value = state->pathIndex;
    value <<= 3;
    v1 = *(s16 *)((s32)g_ShuttlePathAngles + value);
    asm("" ::: "memory");
    value = state->pathIndex;
    value <<= 3;
    state->angleX = v1;
    v1 = *(s16 *)((s32)g_ShuttlePathAngles + value + 2);
    asm("" ::: "memory");
    value = state->pathIndex;
    value <<= 3;
    state->angleY = v1;
    v1 = *(s16 *)((s32)g_ShuttlePathAngles + value + 4);
    asm("" ::: "memory");
    value = state->pathIndex;
    state->startEndpoint = 0;
    state->travelStep = 0;
    value <<= 1;
    state->angleZ = v1;
    value = *(s16 *)((s32)g_ShuttlePathDwellMax + value);
    state->dwellCounter = value;
}
