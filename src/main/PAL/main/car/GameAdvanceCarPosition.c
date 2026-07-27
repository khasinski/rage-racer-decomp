#include "common.h"
#include "game/car.h"
#include "game/render.h"

s32 func_80068568(s32 arg0);
s32 func_80068634(s32 arg0);

/*
 * Shared "advance car position/heading" helper. Integrates the car's speed
 * (field_A4) and lateral component (field_A8) along its heading (field_24) into
 * the world position, then recomputes headingAngle. Called by each of the
 * state98 motion handlers. Register pins and the single-param/two-arg call
 * mismatch are deliberate to match; do not "fix".
 */
void GameAdvanceCarPosition(GameCarRuntime *arg0) asm("func_8002F4E4");
void GameAdvanceCarPosition(GameCarRuntime *arg0) {
    /* This pin is load-bearing: removing it changes .text. */
    register GameCarRuntime *car asm("$19") = arg0;
    volatile s32 coords[3];

    {
        s32 angleSin;
        s32 otherSin;

        angleSin = func_80068568(car->headingAngle);
        otherSin = func_80068568(car->field_24);
        coords[0] = (((angleSin * car->field_A4) / 4) + (otherSin * car->field_A8)) / 100;
    }

    {
        s32 angleCos;
        s32 otherCos;

        angleCos = func_80068634(car->headingAngle);
        otherCos = func_80068634(car->field_24);
        coords[2] = (((angleCos * car->field_A4) / 4) + (otherCos * car->field_A8)) / 100;
    }

    {
        s32 angleSin;
        s32 otherSin;
        s32 angleCos;
        s32 otherCos;

        angleSin = func_80068568(car->headingAngle);
        otherSin = func_80068568(car->field_24);
        angleCos = func_80068634(car->headingAngle);
        otherCos = func_80068634(car->field_24);

        car->field_A4 += ((((angleSin * otherSin) + (angleCos * otherCos)) / 4096) * car->field_A8) / 4096;
    }
    car->headingAngle = 0x400 - GameAtan2(coords[0], coords[2]);
}
