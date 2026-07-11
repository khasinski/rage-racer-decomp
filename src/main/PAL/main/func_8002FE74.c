#include "common.h"
#include "game/car.h"

extern u8 *D_801E42D8;
extern s32 D_8009E6A8;

s32 func_8001A6AC(s32 arg0, s32 arg1);
s32 func_8002A7C4(s32 arg0, s32 arg1);
void func_8002FC84(s32 arg0, s32 *out, s32 weight);
s32 func_8002FD9C(s32 arg0, s32 arg1);
s32 func_80068568(s32 arg0);
s32 func_80068634(s32 arg0);

void func_8002FE74(GameCarRuntime *car) {
    register u8 *timerBase asm("$2");
    register s32 timer asm("$19");
    s32 index;
    register s32 lateral asm("$18");
    register s32 baseIndex asm("$4");
    s32 finalAngle;
    s32 coords[3];
    s32 angle;
    s32 value;
    register s32 xValue asm("$2");
    register s32 headingDelta asm("$3");
    s32 rawIndex;
    s32 trackCount;
    s32 directionFlag;
    register s32 divisor asm("$16");

    timerBase = D_801E42D8;
    lateral = car->field_34;
    timer = *(u16 *)(timerBase + 0x10A);
    asm volatile("" : "=r"(timer) : "0"(timer));

    directionFlag = *(s32 *)((u8 *)car + 0x110);
    asm volatile("" : "=r"(directionFlag) : "0"(directionFlag));
    baseIndex = car->trackPointIndex;
    index = baseIndex + 2;
    if (directionFlag == 0) {
        index = baseIndex - 2;
    }

    rawIndex = index;
    if (index < 0) {
        rawIndex = index + D_8009E6A8;
    }
    trackCount = D_8009E6A8;
    index = rawIndex % trackCount;

    func_8002FC84(index, coords, car->field_38);
    angle = 0x1000 - func_8002FD9C(index, car->field_38);

    xValue = func_80068568(angle) * lateral;
    if (xValue < 0) {
        xValue += 0xFFF;
    }
    coords[0] += xValue >> 12;

    value = func_80068634(angle) * lateral;
    if (value < 0) {
        value += 0xFFF;
    }
    coords[2] += value >> 12;

    finalAngle = 0x400 - func_8001A6AC(coords[0] - car->x, coords[2] - car->z);

    if (car->field_98 == 0) {
        xValue = timer << 16;
        divisor = xValue >> 16;
        if (divisor <= 0) {
            divisor = 1;
        }

        headingDelta = func_8002A7C4(car->headingAngle, finalAngle);
        headingDelta = ((headingDelta * 5) << 2) / divisor;
        car->headingAngle += headingDelta;
    }
}
