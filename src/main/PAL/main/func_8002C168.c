#include "common.h"
#include "game/car.h"
#include "game/track.h"

extern s32 D_801E408C;
extern s32 D_8009E6A8;
extern GameTrackPoint *D_8009E688;

s32 func_80030EB4(GameCarRuntime *car, s32 idx);

void func_8002C168(GameCarRuntime *car) {
    s32 r;
    s32 n;
    s32 i;
    s32 j;
    s32 idx;
    s32 fwd;
    s32 back;
    register s32 bv asm("$2");
    s32 count;
    GameTrackPoint *array;

    n = 1;
    r = func_80030EB4(car, car->trackPointIndex);
    if (r < 0) {
        car->activeFlag = -1;
        return;
    }

    if (D_801E408C == 0) {
        if (r != car->trackPointIndex) {
            count = D_8009E6A8;
            array = D_8009E688;
            do {
                j = car->trackPointIndex - n;
                back = j;
                if (j < 0) {
                    back = j + count;
                }
                fwd = (car->trackPointIndex + n) % count;
                if (r == back) {
                    register s32 ir asm("$3");
                    for (i = 0; i < n; i++) {
                        j = car->trackPointIndex - i;
                        ir = j;
                        if (j < 0) {
                            ir = j + count;
                        }
                        car->field_68 += array[ir].segmentLength;
                    }
                    break;
                }
                if (r == fwd) {
                    for (i = 1; i <= n; i++) {
                        car->field_68 -= array[(car->trackPointIndex + i) % count].segmentLength;
                    }
                    break;
                }
                n++;
            } while (r != car->trackPointIndex);
        }
    } else {
        if (r != car->trackPointIndex) {
            count = D_8009E6A8;
            array = D_8009E688;
            do {
                j = car->trackPointIndex - n;
                back = j;
                if (j < 0) {
                    back = j + count;
                }
                fwd = (car->trackPointIndex + n) % count;
                bv = back;
                if (r == fwd) {
                    for (i = 0; i < n; i++) {
                        car->field_68 += array[(car->trackPointIndex + i) % count].segmentLength;
                    }
                    break;
                }
                if (r == bv) {
                    register s32 ir asm("$3");
                    for (i = 1; i <= n; i++) {
                        j = car->trackPointIndex - i;
                        ir = j;
                        if (j < 0) {
                            ir = j + count;
                        }
                        car->field_68 -= array[ir].segmentLength;
                    }
                    break;
                }
                n++;
            } while (r != car->trackPointIndex);
        }
    }
    car->trackPointIndex = r;
}
