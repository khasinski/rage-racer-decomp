#include "common.h"
#include "game/car.h"
#include "game/track.h"

typedef struct { s16 vx; s16 vy; } DVEC;

extern GameTrackPoint *D_8009E688;
extern s32 D_8009E6A8;

s32 func_80068568(s32 arg0);
s32 func_80068634(s32 arg0);
s32 func_80069C98(s32 arg0, s32 arg1, s32 arg2);

s32 func_80030EB4(GameCarRuntime *car, s32 idx) {
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
        nxt = (i + 1) % D_8009E6A8;
        pa = &D_8009E688[i];
        pb = &D_8009E688[nxt];

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
            ni = i % D_8009E6A8;
        } else {
            ni = (i + D_8009E6A8) % D_8009E6A8;
        }
        i = ni;
    } while (i != idx);

    car->x = D_8009E688[i].x;
    car->z = D_8009E688[i].z;
    i = -1;
    asm volatile("" : "=r"(i) : "0"(i));
    return i;
}
