#include "common.h"
#include "game/vector.h"
#include "psyq/gte.h"
#include "game/render.h"
#include "game/race.h"
#include "game/track.h"
void SetGteObjectMatrix(void *arg0, void *arg1, Matrix *mtx);

extern volatile s32 g_RouteSceneryRotY;

void DrawRouteScenery(void) {
    Matrix mtx0;
    Matrix mtx1;
    Matrix *mtx1Ptr;
    s32 frameValue;
    s32 drawId;

    BuildRotMatrixY(&mtx0, 0x800 - g_RouteSceneryRotY);
    mtx1Ptr = &mtx1;
    BuildRotMatrixX(mtx1Ptr, g_RouteSceneryRotX);
    MulMatrix2(&mtx0, mtx1Ptr);
    MulMatrix2((Matrix *)0x1F800028, mtx1Ptr);
    BuildRotMatrixZ(&mtx0, g_RouteSceneryRotZ);
    MulMatrix2(mtx1Ptr, &mtx0);
    SelectModelBank(1);
    SetGteObjectMatrix((void *)0x1F80011C, &g_RouteSceneryX, &mtx0);
    frameValue = g_ModelBankCount;
    *(s32 *)0x1F800084 = 0;
    drawId = 1;
    if (frameValue >= 0x26) {
        drawId = 0x25;
    }
    SubmitModel((void *)0x1F800000, drawId);
}

typedef struct ShuttlePath {
    Vec4 endpoint[2];
} ShuttlePath;

extern ShuttlePath g_ShuttlePathPoints[];
extern SVec g_ShuttlePathAngles[];

/* Byte-offset views. These stay macros because the retail code keeps the
 * scaled index in a register and re-derives the address at every field; a
 * pointer variable would let the compiler hold the base instead. RAW() is
 * required on each read: a plain member access is marked as living in an
 * aggregate, which stops it aliasing the neighbouring state-> loads and
 * changes what the surrounding barriers do -- see common.h. */
#define PATH(byteOffset) (*(ShuttlePath *)((u8 *)g_ShuttlePathPoints + (byteOffset)))
#define ANGLES(byteOffset) (*(SVec *)((u8 *)g_ShuttlePathAngles + (byteOffset)))

void InitShuttleScenery(void) {
    GameShuttleScenery *state;
    s32 *src;
    register s32 *dst asm("$4");
    s32 index;
    register s32 value asm("$2");
    register s32 v1 asm("$3");
    s32 a4;
    s32 a5;
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

        asm(".globl func_8003F1D0\nfunc_8003F1D0 = InitShuttleScenery + 0xD8");
        index = g_Shuttle1PathIndex;
        v1 = index * 8;
        g_Shuttle1AngleX = RAW(ANGLES(v1).vx);
        g_Shuttle1AngleY = RAW(ANGLES(v1).vy);
        value = RAW(ANGLES(v1).vz);
        index <<= 1;
        g_Shuttle1StartEndpoint = 0;
        g_Shuttle1TravelStep = 0;
        g_Shuttle1AngleZ = value;
        v1 = *(s16 *)((u8 *)g_ShuttlePathDwellMax + index);
        state->pathIndex = 1;
        g_Shuttle1DwellCounter = v1;
    } else {

    state->pathIndex = 0;
    }
    value = state->pathIndex;
    value <<= 5;
    v1 = RAW(PATH(value).endpoint[0].x);
    a4 = RAW(PATH(value).endpoint[0].y);
    a5 = RAW(PATH(value).endpoint[0].z);
    a6 = RAW(PATH(value).endpoint[0].w);
    state->x = v1;
    state->y = a4;
    state->z = a5;
    state->unk1C = a6;
    asm("" ::: "memory");
    value = state->pathIndex;
    value <<= 3;
    v1 = RAW(ANGLES(value).vx);
    asm("" ::: "memory");
    value = state->pathIndex;
    value <<= 3;
    state->angleX = v1;
    v1 = RAW(ANGLES(value).vy);
    value = state->pathIndex;
    value <<= 3;
    state->angleY = v1;
    v1 = RAW(ANGLES(value).vz);
    value = state->pathIndex;
    state->startEndpoint = 0;
    state->travelStep = 0;
    value <<= 1;
    state->angleZ = v1;
    value = *(s16 *)((u8 *)g_ShuttlePathDwellMax + value);
    state->dwellCounter = value;
}
