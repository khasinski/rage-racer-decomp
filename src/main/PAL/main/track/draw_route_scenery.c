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


void DrawRouteScenery(void) asm("func_8003F02C");
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
    func_80017794((void *)0x1F80011C, &g_RouteSceneryX, &mtx0);
    frameValue = g_ModelBankCount;
    *(s32 *)0x1F800084 = 0;
    drawId = 1;
    if (frameValue >= 0x26) {
        drawId = 0x25;
    }
    SubmitModel((void *)0x1F800000, drawId);
}

/*
 * g_ShuttleScenery[1]'s fields, at +0x34 of the array in game/track.h, which
 * documents what each one does: dwellCounter +0x00, travelStep +0x08,
 * startEndpoint +0x0C, pathIndex +0x0E, x/y/z +0x10..0x18, unk1C +0x1C and
 * angleX/Y/Z +0x20..0x28.
 *
 * They CANNOT be written as g_ShuttleScenery[1].field here. As struct member
 * references GCC 2.6.3 marks the loads and stores MEM_IN_STRUCT_P, stops
 * treating them as aliasing the g_ShuttlePath* table reads interleaved with
 * them, hoists one base register for the whole block and reorders the stores
 * (0x34-relative offsets 52/60/64/66/84/88/92 appear, and the branch layout
 * moves). Eleven separate symbols are what retail's codegen requires.
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
typedef struct ShuttleEndpoint {
    s32 x;
    s32 y;
    s32 z;
    s32 unk;
} ShuttleEndpoint;

typedef struct ShuttlePath {
    ShuttleEndpoint endpoint[2];
} ShuttlePath;

typedef struct ShuttleAngles {
    s16 x;
    s16 y;
    s16 z;
    s16 pad;
} ShuttleAngles;

extern ShuttlePath g_ShuttlePathPoints[] asm("D_8007E360");
extern ShuttleAngles g_ShuttlePathAngles[] asm("D_8007E3C0");

/* Byte-offset views. These stay macros because the retail code keeps the
 * scaled index in a register and re-derives the address at every field; a
 * pointer variable would let the compiler hold the base instead. RAW() is
 * required on each read: a plain member access is marked as living in an
 * aggregate, which stops it aliasing the neighbouring state-> loads and
 * changes what the surrounding barriers do -- see common.h. */
#define PATH(byteOffset) (*(ShuttlePath *)((s32)g_ShuttlePathPoints + (byteOffset)))
#define ANGLES(byteOffset) (*(ShuttleAngles *)((s32)g_ShuttlePathAngles + (byteOffset)))
extern s16 g_ShuttlePathDwellMax[] asm("D_8007E3E0");

void InitShuttleScenery(void) asm("func_8003F0F8");

void InitShuttleScenery(void) {
    GameShuttleScenery *state;
    s32 *src;
    /* These pins are load-bearing: removing any one changes .text. */
    register s32 *dst asm("$4");
    s32 index;
    register s32 value asm("$2");
    register s32 v1 asm("$3");
    s32 a4;
    s32 a5;
    /* This pin is load-bearing: removing it changes .text. */
    register s32 a6 asm("$6");

    state = &g_ShuttleScenery[0];
    if ((g_CourseIndex & 3) == 2) {
        g_Shuttle1PathIndex = 2;
        /* This barrier is load-bearing: removing it changes .text. */
        asm("" ::: "memory");
        src = g_ShuttlePath2Points;
        dst = &g_Shuttle1X;
        /* This barrier is load-bearing: removing it changes .text. */
        asm("" : "=r"(src), "=r"(dst) : "0"(src), "1"(dst));
        value = src[0];
        v1 = src[1];
        dst[0] = value;
        dst[1] = v1;
        value = src[2];
        v1 = src[3];
        dst[2] = value;
        dst[3] = v1;

        asm(".globl func_8003F1D0\nfunc_8003F1D0 = func_8003F0F8 + 0xD8");
        index = g_Shuttle1PathIndex;
        v1 = index << 3;
        g_Shuttle1AngleX = RAW(ANGLES(v1).x);
        g_Shuttle1AngleY = RAW(ANGLES(v1).y);
        value = RAW(ANGLES(v1).z);
        index <<= 1;
        g_Shuttle1StartEndpoint = 0;
        g_Shuttle1TravelStep = 0;
        g_Shuttle1AngleZ = value;
        v1 = *(s16 *)((s32)g_ShuttlePathDwellMax + index);
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
    a6 = RAW(PATH(value).endpoint[0].unk);
    state->x = v1;
    state->y = a4;
    state->z = a5;
    state->unk1C = a6;
    /* This barrier is load-bearing: removing it changes .text. */
    asm("" ::: "memory");
    value = state->pathIndex;
    value <<= 3;
    v1 = RAW(ANGLES(value).x);
    /* This barrier is load-bearing: removing it changes .text. */
    asm("" ::: "memory");
    value = state->pathIndex;
    value <<= 3;
    state->angleX = v1;
    v1 = RAW(ANGLES(value).y);
    value = state->pathIndex;
    value <<= 3;
    state->angleY = v1;
    v1 = RAW(ANGLES(value).z);
    value = state->pathIndex;
    state->startEndpoint = 0;
    state->travelStep = 0;
    value <<= 1;
    state->angleZ = v1;
    value = *(s16 *)((s32)g_ShuttlePathDwellMax + value);
    state->dwellCounter = value;
}
