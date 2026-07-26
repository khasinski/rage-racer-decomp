#include "common.h"
#include "game/track.h"
#include "psyq/gte.h"
#include "game/render.h"
#include "game/car.h"
#include "psyq/gpu.h"

typedef struct TrackPointWindow {
    s32 x;
    s32 z;
    s16 y;
    s16 angle;
    u8 padC[2];
    s16 field_E;
    s16 field_10;
    s16 field_12;
    u8 pad14[2];
    u16 segmentLength;
} TrackPointWindow;

typedef struct Unk32098 {
    u16 field_0;
    u8 pad2[6];
    u16 field_8;
    u8 padA[0x26];
    s32 field_30;
} Unk32098;


s32 GameFindTrackSegment(Unk32098 *arg0, s32 arg1) asm("func_80030EB4");

s32 func_80032098(Unk32098 *arg0) {
    s32 index;
    register s32 nextIndex asm("$7");
    register TrackPointWindow *base asm("$6");
    register TrackPointWindow *cur asm("$17");
    register TrackPointWindow *next asm("$16");
    register u32 segmentLengthRaw asm("$18");
    register s32 curOff asm("$3");
    register s32 nextOff asm("$2");
    Matrix mtx;
    s16 vec[4];
    s32 out[3];
    register s32 distance asm("$5");
    register s32 segmentLength asm("$4");
    s32 segmentLengthCompare;
    register s32 invDistance asm("$6");
    register s32 outZ asm("$7");
    s32 fieldE;
    s32 y;
    s32 argX;
    s32 curX;
    s32 argZ;
    s32 curZ;
    s32 angle;

    index = GameFindTrackSegment(arg0, arg0->field_30);
    nextIndex = (index + 1) % g_TrackPointCount;

    base = (TrackPointWindow *)g_TrackPoints;
    asm("" : "=r"(base) : "0"(base)); /* Match note: materialize base in $a2 before current offset. */

    curOff = (index * 3) << 3;
    cur = (TrackPointWindow *)(curOff + (s32)base);

    argX = arg0->field_0;
    curX = (u16)cur->x;
    segmentLengthRaw = cur->segmentLength;
    vec[0] = argX - curX;

    argZ = arg0->field_8;
    curZ = (u16)cur->z;
    vec[1] = 0;
    vec[2] = argZ - curZ;

    angle = cur->angle;
    nextOff = (nextIndex * 3) << 3;
    next = (TrackPointWindow *)(nextOff + (s32)base);
    GameBuildRotMatrixY(&mtx, (0x1000 - angle) & 0xFFF);

    ApplyMatrix((s32 *)&mtx, (s32 *)vec, out);

    segmentLengthCompare = (s16)segmentLengthRaw;
    distance = out[0];
    outZ = out[2];
    if (segmentLengthCompare < distance) {
        distance = segmentLengthCompare;
    } else if (distance < 0) {
        distance = 0;
    }

    segmentLength = (s16)segmentLengthRaw;
    invDistance = segmentLength - distance;
    fieldE = ((next->field_E * distance) + (cur->field_E * invDistance)) / segmentLength;
    y = ((next->y * distance) + (cur->y * invDistance)) / segmentLength;

    return y + (((s16)fieldE * outZ) >> 7);
}

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/func_80032098", func_80032280);

extern s16 D_8019C7D4;
extern s16 D_8019C7D6;
extern s16 D_8019C7D8;
extern s16 D_8019C7DA;
extern s16 D_8019C7DC;
extern s16 D_8019C7DE;
extern s16 D_8019C7E0;
extern s16 D_8019C7E2;
extern GameSpriteDesc D_8007DAE0;
extern u8 D_801C051C[];
extern u8 D_801E3D04[];
extern u8 D_801E3CEC[];
extern u8 D_801E3CF8[];

void func_80032FF0(SPRT *prim, GameSpriteDesc *src);
void func_80064EB8(u8 *arg0, s32 arg1);
void func_800666F4(u8 *arg0, s32 arg1, s32 arg2, u16 arg3, void *arg4);

/*
 * Rebuilds the tachometer needle quad from the car spec block. The spec is
 * read through a plain `u8 *` on purpose: as GameCarSpec member reads, gcc
 * 2.6.3 stops treating the loads as aliasing the D_8019C7Dx globals this
 * function stores to and reorders the two against each other.
 *   +0x14C..0x14F  spec->needleQuad
 *   +0x138/+0x13A  spec->tachoNeedleX / Y
 *   +0x13C/+0x13E  spec->tachoFaceDX / DY
 */
void func_80032BD0(void) {
    u8 *data = (u8 *)g_CarSpec;
    u8 *prim0 = D_801C051C;
    u8 *prim1 = D_801E3D04;
    GameSpriteDesc *src = &D_8007DAE0;

    D_8019C7D4 = -data[0x14F];
    D_8019C7D6 = data[0x14E];
    D_8019C7D8 = -data[0x14D];
    D_8019C7DA = -data[0x14C];
    D_8019C7DC = data[0x14F];
    D_8019C7DE = data[0x14E];
    D_8019C7E0 = data[0x14D];
    D_8019C7E2 = -data[0x14C];

    src->x = *(u16 *)(data + 0x13C) + *(u16 *)(data + 0x138);
    src->y = *(u16 *)(data + 0x13E) + *(u16 *)(data + 0x13A);

    func_80032FF0((SPRT *)prim0, src);
    func_80032FF0((SPRT *)prim1, src);
    func_80064EB8(prim0, 0);
    func_80064EB8(prim1, 0);
    func_800666F4(prim0 - 0x18, 0, 1, 9, 0);
    func_800666F4(prim0 - 0x0C, 0, 1, 0xA, 0);
    func_800666F4(D_801E3CEC, 0, 1, 9, 0);
    func_800666F4(D_801E3CF8, 0, 1, 0xA, 0);
}
