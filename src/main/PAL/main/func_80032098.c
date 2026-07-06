#include "common.h"
#include "game/track.h"
#include "psyq/gte.h"

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

extern s32 D_8009E6A8;
extern GameTrackPoint *D_8009E688;

s32 func_80030EB4(Unk32098 *arg0, s32 arg1);
void func_8001A530(Matrix *mtx, s32 angle);
s32 *func_80069678(s32 *matrix, s32 *vec, s32 *out);

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

    index = func_80030EB4(arg0, arg0->field_30);
    nextIndex = (index + 1) % D_8009E6A8;

    base = (TrackPointWindow *)D_8009E688;
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
    func_8001A530(&mtx, (0x1000 - angle) & 0xFFF);

    func_80069678((s32 *)&mtx, (s32 *)vec, out);

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
