#include "common.h"
#include "game/car.h"
#include "game/track.h"
#include "game/race.h"
#include "game/render.h"
#include "psyq/gte.h"

void func_8002FC84(s32 arg0, s32 *out, s32 weight);
s32 func_8002FD9C(s32 arg0, s32 arg1);
s32 func_80068568(s32 arg0);
s32 func_80068634(s32 arg0);

/*
 * Car route-steering update. Samples a look-ahead track point (two ahead or two
 * behind depending on the lap-direction flag g_RaceSeries), clamps the lateral
 * offset to the track half-width (field_10/field_12), projects the target point
 * off the centre-line along the inward normal (0x1000 - smoothed track angle),
 * then nudges the car's headingAngle toward that target (GameGetAngleDelta). Writes
 * the steer value into field_44 and the route sub-block (field_BC).
 * Register-pinned locals are match-load-bearing.
 */
void GameSteerCarAlongRoute(GameCarRuntime *car) asm("func_8003AEBC");
void GameSteerCarAlongRoute(GameCarRuntime *car) {
    GameCarRuntime *route;
    GameTrackPoint *point;
    s32 index;
    s32 offset;
    s32 lateral;
    s32 rem;
    s32 coords[3];
    s32 angle;
    register s32 value asm("$2");
    register s32 zValue asm("$5");
    register s32 lowerLimit asm("$3");
    register s32 callArg asm("$4");

    lateral = car->field_11C;
    offset = car->trackPointIndex;
    route = (GameCarRuntime *)&car->field_BC;
    car->field_DC = 0;

    if (g_RaceSeries != 0) {
        index = offset + 2;
    } else {
        index = offset - 2;
    }

    rem = index;
    if (index < 0) {
        rem = index + g_TrackPointCount;
    }
    index = rem % g_TrackPointCount;

    point = &g_TrackPoints[index];
    if (point->field_12 < lateral) {
        value = point->field_12 * car->field_3C;
        if (value < 0) {
            value += 0x7FF;
        }
        lateral = value >> 11;
    } else {
        value = point->field_10;
        lowerLimit = -value;
        if (lateral < lowerLimit) {
            value = lowerLimit * car->field_3C;
            if (value < 0) {
                value += 0x7FF;
            }
            lateral = value >> 11;
        }
    }

    func_8002FC84(index, coords, car->field_38);
    angle = 0x1000 - func_8002FD9C(index, car->field_38);

    value = func_80068568(angle) * lateral;
    if (value < 0) {
        value += 0xFFF;
    }
    coords[0] += value >> 12;

    zValue = func_80068634(angle) * lateral;
    if (zValue < 0) {
        zValue += 0xFFF;
    }
    coords[2] += zValue >> 12;

    angle = 0x400 - GameAtan2(coords[0] - car->x, coords[2] - car->z);

    callArg = g_RaceSeries;
    value = car->field_B4;
    callArg = (callArg << 11) + 0xC00;
    value = -GameGetAngleDelta(callArg - value, angle);
    car->field_44 = value * 3;

    if (car->field_98 == 0) {
        value = GameGetAngleDelta(car->headingAngle, angle);
        value += car->headingAngle;
        car->headingAngle = value;
        route->trackPointIndex = value;
        car->field_24 = value;
    }
}

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/car/GameSteerCarAlongRoute", func_8003B0D4);

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/car/GameSteerCarAlongRoute", func_8003BB50);

typedef struct KE {
    u16 f0;
    u16 f2;
    u16 f4;
    u16 f6;
    u16 f8;
    u16 fA;
    s32 fC;
    s16 f10;
    s16 f12;
} KE;

typedef struct B16 {
    s32 a;
    s32 b;
    s32 c;
    s32 d;
} B16;

typedef struct Obj {
    s32 x;
    s32 y;
    s32 z;
    s32 f0C;
    s32 f10;
    s32 f14;
    s32 f18;
    s32 f1C;
    s32 f20;
    s32 f24;
    s32 f28;
    s32 f2C;
} Obj;

extern u8 *volatile g_RaceIntroCameraScript asm("D_8019CAF8");
extern KE *g_RaceIntroCameraCursor asm("D_801E4038");
extern s32 D_8009AFB4;
extern s16 D_8009AFBC;
extern s16 D_8009AFBE;
extern s16 D_8009AFC0;

s32 func_80068634(s32 angle);
s32 func_80068568(s32 angle);
void GameDrawPlayerCarModel(void *arg0) asm("func_8001DAB0");
void GameDrawFullscreenFadeTile(s32 arg0, s32 arg1) asm("func_80033AA0");
void GameUpdateCamera(void *arg0, s32 arg1) asm("func_80043BCC");

void GameRunRaceIntroCamera(Obj *obj, s32 mode) asm("func_8003C508");
void GameRunRaceIntroCamera(Obj *obj, s32 mode) {
    s32 *spad = (s32 *) 0x1F800000;
    register s32 s0v asm("$16");
    s32 delta[3];

    __asm__("" : "=r"(s0v) : "0"(28), "r"(spad));
    if (mode < 90) {
        if (mode < 2) {
            u8 *base = g_RaceIntroCameraScript;
            s16 n = *(s16 *) (base + 2 * g_RaceSeries);
            s32 off = n * 20 + 4;
            KE *p = (KE *) (off + (s32) base);
            KE *q;
            g_RaceIntroCameraCursor = p;
            *(B16 *) 0x1F800008 = *(B16 *) p;
            q = g_RaceIntroCameraCursor;
            D_8009AFBC = -q[0].f0 + q[1].f0;
            __asm__ volatile("");
            D_8009AFBE = -q[0].f4 + q[1].f4;
            __asm__ volatile("");
            D_8009AFC0 = -q[0].f8 + q[1].f8;
            __asm__ volatile("");
            D_8009AFB4 = q[0].f12;
        } else {
            register KE *a asm("$4") = g_RaceIntroCameraCursor;
            if (mode == a->f10) {
                g_RaceIntroCameraCursor = &a[1];
                D_8009AFB4 = a[1].f12;
                if (a[1].fC == 1) {
                    D_8009AFBC = -a[1].f0 + ((u16 *) obj)[0];
                    D_8009AFBE = -a[1].f4 - 28 + ((u16 *) obj)[2];
                    D_8009AFC0 = -a[1].f8 + ((u16 *) obj)[4];
                } else {
                    D_8009AFBC = -a[1].f0 + a[2].f0;
                    D_8009AFBE = -a[1].f4 + a[2].f4;
                    D_8009AFC0 = -a[1].f8 + a[2].f8;
                }
            }
        }

        D_8009AFB4 = D_8009AFB4 - 1;
        if (D_8009AFB4 <= 0) {
            D_8009AFB4 = 0;
        }

        if (g_RaceIntroCameraCursor->fC == 0) {
            spad[2] = ((s32 *) g_RaceIntroCameraCursor)[0]
                      + ((s32) D_8009AFBC * func_80068634((D_8009AFB4 << 10) / g_RaceIntroCameraCursor->f12)) / 4096;
            spad[3] = ((s32 *) g_RaceIntroCameraCursor)[1]
                      + ((s32) D_8009AFBE * func_80068634((D_8009AFB4 << 10) / g_RaceIntroCameraCursor->f12)) / 4096;
            spad[4] = ((s32 *) g_RaceIntroCameraCursor)[2]
                      + ((s32) D_8009AFC0 * func_80068634((D_8009AFB4 << 10) / g_RaceIntroCameraCursor->f12)) / 4096;

            delta[0] = func_80068568(obj->f24) / 128 + obj->x - spad[2];
            delta[1] = obj->y - s0v - spad[3];
            delta[2] = func_80068634(obj->f24) / 128 + obj->z - spad[4];
            s0v = 0x400;
            spad[7] = s0v - GameAtan2(delta[0], delta[2]);
            s0v = s0v - GameAtan2(delta[1], SquareRoot12(delta[0] * delta[0] + delta[2] * delta[2]) >> 6);
            spad[6] = s0v;
            spad[8] = 0;
            GameSetCameraRotMatrix();
            GameSelectModelBank(0);
            GameDrawPlayerCarModel(obj);
        } else {
            GameDrawFullscreenFadeTile(D_8009AFB4 * 26, 0x29);
            {
                s32 c0 = obj->x;
                s32 c1 = obj->y;
                s32 c2 = obj->z;
                s32 c3 = obj->f0C;
                spad[2] = c0;
                spad[3] = c1;
                spad[4] = c2;
                spad[5] = c3;
            }
            __asm__ volatile("");
            spad[3] -= s0v;
            {
                s32 c0 = obj->f20;
                s32 c1 = obj->f24;
                s32 c2 = obj->f28;
                s32 c3 = obj->f2C;
                spad[6] = c0;
                spad[7] = c1;
                spad[8] = c2;
                spad[9] = c3;
            }
            __asm__ volatile("");
            GameSetCameraRotMatrix();
        }
    } else {
        GameUpdateCamera(obj, 0);
    }
}

extern u32 g_CameraCar[] asm("D_801E3E14");
extern s32 g_CameraCarY asm("D_801E3E18");
extern s32 g_CameraCarZ asm("D_801E3E1C");
extern s32 g_CameraCarAngleY asm("D_801E3E38");
extern s32 g_CameraCarHeading asm("D_801E3EB4");
extern s32 g_CameraCarSpeed asm("D_801E3EB8");
extern s32 D_801E3F60;

typedef struct CopyBlock8003CA14 {
    u32 word0;
    u32 word1;
    u32 word2;
    u32 word3;
} CopyBlock8003CA14;

void GameSeedFinishCamera(void *arg0) asm("func_8003CA14");
void GameSeedFinishCamera(void *arg0) {
    register u32 word0 asm("$2");
    register u32 word1 asm("$3");
    register u32 word2 asm("$4");
    register CopyBlock8003CA14 *src asm("$6");
    register CopyBlock8003CA14 *dst asm("$7");
    register CopyBlock8003CA14 *end asm("$9");
    register u32 *base asm("$8");
    register GameTrackPoint *track asm("$5");
    register GameTrackPoint *point asm("$2");
    register s32 index asm("$3");
    register s32 lastIndex asm("$4");
    register s32 value asm("$3");

    base = arg0;
    asm("" : "=r"(base) : "0"(base));
    dst = (CopyBlock8003CA14 *)g_CameraCar;
    src = (CopyBlock8003CA14 *)base;
    end = (CopyBlock8003CA14 *)((s32)base + 0x190);
    do {
        *dst = *src;
        src++;
        dst++;
    } while (src != end);

    word0 = ((u32 *)src)[0];
    word1 = ((u32 *)src)[1];
    word2 = ((u32 *)src)[2];
    ((u32 *)dst)[0] = word0;
    ((u32 *)dst)[1] = word1;
    ((u32 *)dst)[2] = word2;

    index = *(s32 *)((s32)base + 0x30);
    track = g_TrackPoints;
    point = (GameTrackPoint *)((index * 3) << 3);
    point = (GameTrackPoint *)((s32)point + (s32)track);
    g_CameraCar[0] = point->x;

    index = *(s32 *)((s32)base + 0x30);
    point = (GameTrackPoint *)((index * 3) << 3);
    point = (GameTrackPoint *)((s32)point + (s32)track);
    g_CameraCarZ = point->z;

    index = *(s32 *)((s32)base + 0x30);
    point = (GameTrackPoint *)((index * 3) << 3);
    point = (GameTrackPoint *)((s32)point + (s32)track);
    value = g_CameraCarSpeed;
    word0 = point->y;
    value += 0x40;
    word0 -= 0x40;
    g_CameraCarSpeed = value;
    g_CameraCarY = word0;

    value = *(s16 *)((s32)base + 0xB8);
    lastIndex = *(s32 *)((s32)base + 0x30);
    value <<= 11;
    point = (GameTrackPoint *)((lastIndex * 3) << 3);
    point = (GameTrackPoint *)((s32)point + (s32)track);
    value += 0xC00;
    value -= point->angle;
    g_CameraCarHeading = value;
    D_801E3F60 = value;
    g_CameraCarAngleY = value;
}
