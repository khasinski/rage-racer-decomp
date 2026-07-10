#include "common.h"
#include "game/car.h"
#include "game/track.h"

extern s32 D_801E408C;
extern s32 D_8009E6A8;
extern GameTrackPoint *D_8009E688;

s32 func_8001A6AC(s32 arg0, s32 arg1);
void func_8002FC84(s32 arg0, s32 *out, s32 weight);
s32 func_8002FD9C(s32 arg0, s32 arg1);
s32 func_8002A7C4(s32 arg0, s32 arg1);
s32 func_80068568(s32 arg0);
s32 func_80068634(s32 arg0);

void func_8003AEBC(GameCarRuntime *car) {
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

    if (D_801E408C != 0) {
        index = offset + 2;
    } else {
        index = offset - 2;
    }

    rem = index;
    if (index < 0) {
        rem = index + D_8009E6A8;
    }
    index = rem % D_8009E6A8;

    point = &D_8009E688[index];
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

    angle = 0x400 - func_8001A6AC(coords[0] - car->x, coords[2] - car->z);

    callArg = D_801E408C;
    value = car->field_B4;
    callArg = (callArg << 11) + 0xC00;
    value = -func_8002A7C4(callArg - value, angle);
    car->field_44 = value * 3;

    if (car->field_98 == 0) {
        value = func_8002A7C4(car->headingAngle, angle);
        value += car->headingAngle;
        car->headingAngle = value;
        route->trackPointIndex = value;
        car->field_24 = value;
    }
}
