#include "common.h"
#include "game/car.h"

typedef struct D8007Pair {
    s16 f0;
    s16 f2;
} D8007Pair;

extern u8 *D_801E42D8;
extern s16 D_801E6E74;
extern s16 D_801F17A4;
extern D8007Pair D_8007DAC0[];

s32 func_8002A7C4(s32 arg0, s32 arg1);
void func_8002F4E4(GameCarRuntime *car, s32 arg1);
s32 func_80068568(s32 arg0);
s32 func_80068634(s32 arg0);
void GameSetIndexedEffectVoice(s32 index, s32 phase, s32 volume) asm("func_8005C104");

void func_8002F690(GameCarRuntime *car) {
    GameCarRuntime *routeStruct = (GameCarRuntime *)&car->field_BC;
    u8 *route = (u8 *)routeStruct;
    s32 sinA;
    s32 cosA;
    s32 base;
    s32 r;
    s32 coords[3];
    register u8 *data1 asm("$3");
    u8 *data;
    s32 t;
    s32 idx;

    r = func_8002A7C4(car->field_24, *(s32 *)&car->field_14C);
    base = car->field_24;
    car->field_24 = r / 5 + base;
    func_8002F4E4(car, base);

    sinA = func_80068568(car->field_24);
    cosA = func_80068634(car->field_24);

    car->field_C4 = func_80068568(car->headingAngle) * car->field_A4 / 256;
    car->field_CC = func_80068634(car->headingAngle) * car->field_A4 / 256;

    coords[0] = (cosA * car->field_C4 - sinA * car->field_CC) / 4096;
    coords[2] = (sinA * car->field_C4 + cosA * car->field_CC) / 4096;
    car->field_C4 = sinA * coords[2] / 4096;
    car->field_CC = cosA * coords[2] / 4096;

    data1 = D_801E42D8;
    if (*(s16 *)(data1 + 0x100) + 2000 < car->field_134 && D_801E6E74 >= 2) {
        GameSetIndexedEffectVoice(0, 0x1800,
                                  (car->field_134 - *(s16 *)(data1 + 0x100)) / 100 + 128);
    } else {
        GameSetIndexedEffectVoice(-1, 0, 0);
    }

    data = D_801E42D8;
    if (*(s16 *)(data + 0x106) + 1000 < *(s32 *)(route + 0x78)) {
        s16 v = D_801F17A4;
        if (v >= 41 && *(s16 *)(route + 0x76) == *(s16 *)(data + 0x104) &&
            car->field_98 == 0) {
            idx = v + 24;
            if (idx >= 101) {
                idx = 100;
            }
            GameSetIndexedEffectVoice(2, 0x1500, idx);
        } else {
            GameSetIndexedEffectVoice(-1, 0, 0);
        }
    } else {
        GameSetIndexedEffectVoice(-1, 0, 0);
    }

    if (*(s16 *)(route + 0x9c) == 1) {
        *(s32 *)(route + 0x48) = car->field_A4 * *(s32 *)(route + 0x44);
        *(s32 *)(route + 0x44) = 0;
        if ((s32) D_8007DAC0[*(s32 *)(route + 0x28)].f0 < car->field_A4 &&
            *(s32 *)(route + 0x48) > *(s32 *)(route + 0x84)) {
            *(s32 *)(route + 0x98) = 1;
            *(s16 *)(route + 0x3e) = 0;
            GameSetIndexedEffectVoice(0, 0, 0);
            t = 1000 - (*(s32 *)(route + 0x88) - 1000) * 8;
            if (t < 1000) {
                t = 1000;
            }
            *(s32 *)(route + 0x50) = -coords[0] * t / 1000 * 2;
            *(s32 *)(route + 0x54) = car->routeRow;
        }
    } else {
        if (*(s16 *)(route + 0xa0) < 128) {
            s16 m9e = *(s16 *)(route + 0x9e);
            if (m9e == 1) {
                s32 av = coords[0] < 0 ? -coords[0] : coords[0];
                s32 aval = av * car->field_A4 / 64;
                *(s32 *)(route + 0x48) = aval;
                if ((s32) D_8007DAC0[*(s32 *)(route + 0x28)].f2 < car->field_A4 &&
                    *(s32 *)(route + 0x84) < aval) {
                    *(s32 *)(route + 0x98) = m9e;
                    *(s16 *)(route + 0x3e) = 0;
                    GameSetIndexedEffectVoice(0, 0, 0);
                    *(s32 *)(route + 0x50) = -coords[0];
                    *(s32 *)(route + 0x54) = car->routeRow;
                }
            } else {
                *(s32 *)(route + 0x44) = *(s32 *)(route + 0x44) + 1;
                *(s32 *)(route + 0x48) = 0;
            }
        } else {
            *(s32 *)(route + 0x44) = 0;
            *(s32 *)(route + 0x48) = 0;
        }
    }
}
