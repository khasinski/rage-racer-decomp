#include "common.h"
#include "game/car.h"

extern s32 D_801E8AA0;

s32 func_8002A7C4(s32 arg0, s32 arg1);
void func_8002F4E4(GameCarRuntime *car, s32 arg1);
s32 func_80068568(s32 arg0);
s32 func_80068634(s32 arg0);
void func_8005C104(s32 index, s32 phase, s32 volume);

/*
 * Car motion handler for state98 == 2 (airborne / jump): decays velocity and
 * spin, advances the car (func_8002F4E4), and lands it when it returns to the
 * ground. The drive sub-block is the GameCarDrive view of car->field_BC.
 */
void func_80030814(GameCarRuntime *car) {
    GameCarDrive *r = (GameCarDrive *)&car->field_BC;
    s32 sinF24;
    s32 cosF24;
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
        func_8005C104(0, phase, r->unk38 * 2 + 80);
    } else {
        func_8005C104(0, 0x1800, flag + 25);
    }

    {
        s32 rr = func_8002A7C4(car->field_24, r->unk90);
        s32 base = car->field_24;
        car->field_24 = rr / 5 + base;
        func_8002F4E4(car, base);
    }

    sinF24 = func_80068568(car->field_24);
    cosF24 = func_80068634(car->field_24);

    r->accelPos = func_80068568(car->headingAngle + r->unk60) * car->field_A4 / 256;
    r->brakePos = func_80068634(car->headingAngle + r->unk60) * car->field_A4 / 256;

    coords[0] = (cosF24 * r->accelPos - sinF24 * r->brakePos) / 4096;
    coords[2] = (sinF24 * r->accelPos + cosF24 * r->brakePos) / 4096;

    r->accelPos =
        func_80068568(r->unk58) * r->unk5C / 256 + sinF24 * coords[2] / 4096;
    r->brakePos =
        func_80068634(r->unk58) * r->unk5C / 256 + cosF24 * coords[2] / 4096;

    if (r->unk9C != 1 && r->unk9E != 1 && r->accelBtn < 128) {
        r->unk44 += 1;
    } else {
        r->unk44 = 0;
    }

    r->unk50 = r->unk50 * 31 / 32;
    r->unk5C = r->unk5C * 31 / 32;
    r->unk60 = r->unk60 * 31 / 32;

    *(s16 *)&r->unk3E = *(s16 *)&r->unk3E * 2 / 3;
    if (r->unk60 >= 1537) {
        car->field_A4 = car->field_A4 * 4 / 5;
    }

    if (r->unk38 <= 0) {
        func_8005C104(-1, 0, 0);
        car->field_24 -= r->unk50;
        D_801E8AA0 = 0;
        r->unk3C = 0;
        r->unk60 = 0;
        r->unk5C = 0;
        r->state98 = 0;
        *(s16 *)&r->unk3E = 0;
    }
}
