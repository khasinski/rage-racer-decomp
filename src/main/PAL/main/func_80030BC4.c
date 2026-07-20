#include "common.h"
#include "game/car.h"

extern s32 D_8019CA04;

s32 func_8002A7C4(s32 arg0, s32 arg1);
void func_8002F4E4(GameCarRuntime *car, s32 arg1);
s32 func_80068568(s32 arg0);
s32 func_80068634(s32 arg0);
s32 func_800632B0(void);
void GameSetIndexedEffectVoice(s32 index, s32 phase, s32 volume) asm("func_8005C104");

void func_80030BC4(GameCarRuntime *car) {
    GameCarRuntime *route = (GameCarRuntime *)&car->field_BC;
    s32 sinA;
    s32 cosA;
    s32 base;
    s32 r;
    s32 coords[3];

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
    car->field_C4 = sinA * coords[2] / 16384;
    car->field_CC = cosA * coords[2] / 16384;

    GameSetIndexedEffectVoice(0, 0x1A80, (0x60 - (D_8019CA04 & 0x1F) * 2) * car->field_15C / 256);

    car->field_A4 = car->field_A4 / 10;

    if (D_8019CA04 >= 11) {
        s32 f15c = car->field_15C;
        s32 f134 = car->field_134;

        sinA = f15c + 32;
        D_8019CA04 -= car->field_15E * 2;
        if (f134 < 2000) {
            sinA = f15c + 1032;
        }
        if (f15c < 127 && f134 >= 2001) {
            sinA += 127;
        }
        route->field_68 = (func_800632B0() & 3) * sinA / 256;
        route->field_6C = (func_800632B0() & 7) * sinA / 256;
        D_8019CA04 -= sinA;
        if (D_8019CA04 <= 0) {
            route->field_68 = 0;
            route->field_6C = 0;
            *(s32 *)&route->field_98 = 0;
            GameSetIndexedEffectVoice(-1, 0, 0);
        }
    } else {
        GameSetIndexedEffectVoice(-1, 0, 0);
        *(s32 *)((u8 *)car + 0x154) = 0;
        *(s32 *)&car->field_124 = 0;
        *(s32 *)&car->field_128 = 0;
    }
}
