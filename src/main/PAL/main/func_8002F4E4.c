#include "common.h"
#include "game/car.h"

s32 func_8001A6AC(s32 arg0, s32 arg1);
s32 func_80068568(s32 arg0);
s32 func_80068634(s32 arg0);

void func_8002F4E4(GameCarRuntime *arg0) {
    register GameCarRuntime *car asm("$19") = arg0;
    volatile s32 coords[3];

    {
        register s32 angleSin asm("$16");
        register s32 otherSin asm("$4");

        angleSin = func_80068568(car->headingAngle);
        otherSin = func_80068568(car->field_24);
        coords[0] = (((angleSin * car->field_A4) / 4) + (otherSin * car->field_A8)) / 100;
    }

    {
        register s32 angleCos asm("$16");
        register s32 otherCos asm("$4");

        angleCos = func_80068634(car->headingAngle);
        otherCos = func_80068634(car->field_24);
        coords[2] = (((angleCos * car->field_A4) / 4) + (otherCos * car->field_A8)) / 100;
    }

    {
        register s32 angleSin asm("$18");
        register s32 otherSin asm("$17");
        register s32 angleCos asm("$16");
        s32 otherCos;

        angleSin = func_80068568(car->headingAngle);
        otherSin = func_80068568(car->field_24);
        angleCos = func_80068634(car->headingAngle);
        otherCos = func_80068634(car->field_24);

        car->field_A4 += ((((angleSin * otherSin) + (angleCos * otherCos)) / 4096) * car->field_A8) / 4096;
    }
    car->headingAngle = 0x400 - func_8001A6AC(coords[0], coords[2]);
}
