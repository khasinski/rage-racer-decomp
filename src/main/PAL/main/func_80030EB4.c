#include "common.h"
#include "game/car.h"
#include "game/track.h"

typedef struct { s16 vx; s16 vy; } DVEC;


s32 func_80068568(s32 arg0);
s32 func_80068634(s32 arg0);
s32 func_80069C98(s32 arg0, s32 arg1, s32 arg2);

/*
 * Finds the track segment whose (rotated, half-width) quad currently contains
 * the car. Starting at `idx` it spirals outward over neighbouring segments
 * (k alternately added/subtracted), and for each builds the segment quad from
 * the two endpoints' angle + left/right half-widths (field_10/field_12) and
 * runs four half-plane cross-product tests (func_80069C98). Returns the
 * containing segment index, or -1 (snapping the car onto the track) if none.
 * pts[0] is the car-relative point; pts[1..4] are the quad corners.
 */
s32 GameFindTrackSegment(GameCarRuntime *car, s32 idx) asm("func_80030EB4");
s32 GameFindTrackSegment(GameCarRuntime *car, s32 idx) {
    DVEC pts[5];
    s32 i;
    s32 k;
    s32 nxt;
    s32 ni;
    s32 carx;
    s32 carz;
    s32 sx;
    s32 sz;
    s32 cos_c;
    s32 sin_c;
    s32 cos_n;
    s32 sin_n;
    s32 pax;
    s32 paz;
    s32 f10a;
    s32 f12a;
    s32 f10b;
    s32 f12b;
    GameTrackPoint *pa;
    GameTrackPoint *pb;

    k = 0;
    carx = car->x;
    carz = car->z;
    i = idx;

    do {
        nxt = (i + 1) % g_TrackPointCount;
        pa = &g_TrackPoints[i];
        pb = &g_TrackPoints[nxt];

        pax = pa->x;
        paz = pa->z;
        sx = pb->x - pax;
        sz = pb->z - paz;
        pts[0].vx = carx - pax;
        pts[0].vy = carz - paz;

        cos_c = func_80068634(0xC00 - pa->angle);
        sin_c = func_80068568(0xC00 - pa->angle);
        cos_n = func_80068634(0xC00 - pb->angle);
        sin_n = func_80068568(0xC00 - pb->angle);

        f10a = pa->field_10;
        f12a = pa->field_12;
        f12b = pb->field_12;
        f10b = pb->field_10;

        pts[1].vx =  (s16)(f10a << 1) * (s16)cos_c / 4096;
        pts[1].vy = -(s16)(f10a << 1) * (s16)sin_c / 4096;
        pts[2].vx = -(s16)(f12a << 1) * (s16)cos_c / 4096;
        pts[2].vy =  (s16)(f12a << 1) * (s16)sin_c / 4096;
        pts[3].vx = sx + (s16)(f10b << 1) * (s16)cos_n / 4096;
        pts[3].vy = sz - (s16)(f10b << 1) * (s16)sin_n / 4096;
        pts[4].vx = sx - (s16)(f12b << 1) * (s16)cos_n / 4096;
        pts[4].vy = sz + (s16)(f12b << 1) * (s16)sin_n / 4096;

        if (func_80069C98(*(s32 *)&pts[1], *(s32 *)&pts[2], *(s32 *)&pts[0]) >= 0 &&
            func_80069C98(*(s32 *)&pts[2], *(s32 *)&pts[4], *(s32 *)&pts[0]) >= 0 &&
            func_80069C98(*(s32 *)&pts[4], *(s32 *)&pts[3], *(s32 *)&pts[0]) > 0 &&
            func_80069C98(*(s32 *)&pts[3], *(s32 *)&pts[1], *(s32 *)&pts[0]) >= 0) {
            return i;
        }

        k++;
        if (k & 1) {
            i += k;
        } else {
            i -= k;
        }
        if (i >= 0) {
            ni = i % g_TrackPointCount;
        } else {
            ni = (i + g_TrackPointCount) % g_TrackPointCount;
        }
        i = ni;
    } while (i != idx);

    car->x = g_TrackPoints[i].x;
    car->z = g_TrackPoints[i].z;
    i = -1;
    asm volatile("" : "=r"(i) : "0"(i));
    return i;
}
