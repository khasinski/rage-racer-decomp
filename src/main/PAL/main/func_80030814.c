#include "common.h"
#include "game/car.h"

extern s32 D_801E8AA0;

s32 func_8002A7C4(s32 arg0, s32 arg1);
void func_8002F4E4(GameCarRuntime *car, s32 arg1);
s32 func_80068568(s32 arg0);
s32 func_80068634(s32 arg0);
void GameSetIndexedEffectVoice(s32 index, s32 phase, s32 volume) asm("func_8005C104");

void func_80030814(GameCarRuntime *car) {
    u8 *r = (u8 *)car + 188;
    register s32 sinF24 asm("$18");
    register s32 cosF24 asm("$19");
    volatile s32 coords[3];
    s32 flag = D_801E8AA0;

    if (flag == 0) {
        s32 phase;
        s32 f11c = *(s32 *)((u8 *)car + 0x11C);
        if (f11c < 513) {
            phase = f11c * 3 + 6144;
        } else {
            phase = 0x1e00;
        }
        GameSetIndexedEffectVoice(0, phase, *(s16 *)(r + 0x38) * 2 + 80);
    } else {
        GameSetIndexedEffectVoice(0, 0x1800, flag + 25);
    }

    {
        s32 rr = func_8002A7C4(car->field_24, *(s32 *)(r + 0x90));
        s32 base = car->field_24;
        car->field_24 = rr / 5 + base;
        func_8002F4E4(car, base);
    }

    sinF24 = func_80068568(car->field_24);
    cosF24 = func_80068634(car->field_24);

    *(s32 *)(r + 0x08) = func_80068568(car->headingAngle + *(s32 *)(r + 0x60)) * car->field_A4 / 256;
    *(s32 *)(r + 0x10) = func_80068634(car->headingAngle + *(s32 *)(r + 0x60)) * car->field_A4 / 256;

    coords[0] = (cosF24 * *(s32 *)(r + 0x08) - sinF24 * *(s32 *)(r + 0x10)) / 4096;
    coords[2] = (sinF24 * *(s32 *)(r + 0x08) + cosF24 * *(s32 *)(r + 0x10)) / 4096;

    *(s32 *)(r + 0x08) =
        func_80068568(*(s32 *)(r + 0x58)) * *(s32 *)(r + 0x5C) / 256 + sinF24 * coords[2] / 4096;
    *(s32 *)(r + 0x10) =
        func_80068634(*(s32 *)(r + 0x58)) * *(s32 *)(r + 0x5C) / 256 + cosF24 * coords[2] / 4096;

    if (*(s16 *)(r + 0x9C) != 1 && *(s16 *)(r + 0x9E) != 1 && *(s16 *)(r + 0xA0) < 128) {
        *(s32 *)(r + 0x44) += 1;
    } else {
        *(s32 *)(r + 0x44) = 0;
    }

    *(s32 *)(r + 0x50) = *(s32 *)(r + 0x50) * 31 / 32;
    *(s32 *)(r + 0x5C) = *(s32 *)(r + 0x5C) * 31 / 32;
    *(s32 *)(r + 0x60) = *(s32 *)(r + 0x60) * 31 / 32;

    *(s16 *)(r + 0x3E) = *(s16 *)(r + 0x3E) * 2 / 3;
    if (*(s32 *)(r + 0x60) >= 1537) {
        car->field_A4 = car->field_A4 * 4 / 5;
    }

    if (*(s16 *)(r + 0x38) <= 0) {
        GameSetIndexedEffectVoice(-1, 0, 0);
        car->field_24 -= *(s32 *)(r + 0x50);
        D_801E8AA0 = 0;
        *(s16 *)(r + 0x3C) = 0;
        *(s32 *)(r + 0x60) = 0;
        *(s32 *)(r + 0x5C) = 0;
        *(s32 *)(r + 0x98) = 0;
        *(s16 *)(r + 0x3E) = 0;
    }
}
