#include "common.h"
#include "game/render.h"
#include "game/track.h"

extern GameRenderObject D_801E3E14;
extern s32 D_801E3E44;
extern s32 D_801E3E1C;
extern s32 D_801E3EB4;
extern s32 D_801E3EB8;
extern s32 D_801E3ED8;
extern s32 D_801E3EE0;

s32 func_8001A6AC(s32 arg0, s32 arg1);
void func_8002FC84(s32 arg0, s32 *out, s32 weight);
s32 func_8002A7C4(s32 arg0, s32 arg1);
s32 func_80068568(s32 arg0);
s32 func_80068634(s32 arg0);
s32 func_8006888C(s32 arg0);
void func_8002C168(void *arg0);
void func_80031298(void *arg0, s32 arg1, void *arg2);
void func_8001A610(void);
void func_80017A10(s32 arg0);
void func_8001DAB0(GameRenderObject *obj);

typedef struct Block16 {
    s32 w0;
    s32 w1;
    s32 w2;
    s32 w3;
} Block16;

/*
 * Camera track-follower: advances a look-ahead track point, aims the eye object
 * D_801E3E14 (a GameRenderObject) toward the sampled centre-line point
 * (func_8002FC84 + atan2), nudges its position, then seeds the scratchpad view
 * state (view[2..4]=eye XYZ, view[6]=pitch, view[7]=yaw, view[8]=roll) from the
 * eye object and submits the render object (func_8001DAB0). markerClamp is the
 * zeroed clamp record passed to the track-marker builder func_80031298.
 */
void func_8003CB3C(GameRenderObject *obj) {
    s32 *view = (s32 *)0x1F800000;
    s32 delta[3];
    s32 coords[3];
    s16 markerClamp[2];
    s32 index;
    s32 rem;
    s32 offset;
    s32 angle;
    s32 value;
    s32 zValue;

    offset = D_801E3E44;
    if (*(s16 *)&obj->field_B8 != 0) {
        index = offset + 2;
    } else {
        index = offset - 2;
    }
    rem = index;
    if (index < 0) {
        rem = index + g_TrackPointCount;
    }
    index = rem % g_TrackPointCount;

    func_8002FC84(index, coords, D_801E3E14.field_38);
    angle = 0x400 - func_8001A6AC(coords[0] - D_801E3E14.x, coords[2] - D_801E3E1C);

    D_801E3EB4 += func_8002A7C4(D_801E3EB4, angle);

    value = func_80068568(D_801E3EB4) * D_801E3EB8;
    if (value < 0) {
        value += 0xFF;
    }
    D_801E3ED8 = value >> 8;

    zValue = func_80068634(D_801E3EB4) * D_801E3EB8;
    if (zValue < 0) {
        zValue += 0xFF;
    }
    D_801E3EE0 = zValue >> 8;

    D_801E3E14.x = D_801E3ED8 / 256 + D_801E3E14.x;
    D_801E3E1C = D_801E3EE0 / 256 + D_801E3E1C;

    func_8002C168(&D_801E3E14);
    markerClamp[0] = 0;
    markerClamp[1] = 0;
    func_80031298(&D_801E3E14, D_801E3E44, markerClamp);

    *(Block16 *)(view + 2) = *(Block16 *)&D_801E3E14;
    view[3] -= 64;

    delta[0] = obj->x - view[2];
    delta[1] = obj->y - view[3];
    delta[2] = obj->z - view[4];

    view[7] = 0x400 - func_8001A6AC(delta[0], delta[2]);
    value = func_8006888C(delta[0] * delta[0] + delta[2] * delta[2]);
    view[6] = 0x400 - func_8001A6AC(delta[1], value >> 6);
    view[8] = 0;

    func_8001A610();
    func_80017A10(0);
    func_8001DAB0(obj);
}
