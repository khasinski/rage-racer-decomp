#include "common.h"
#include "game/car.h"
#include "game/render.h"

s32 rsin(s32 arg0) asm("func_80068568");
s32 rcos(s32 arg0) asm("func_80068634");

/*
 * Shared "advance car position/heading" helper. Integrates the car's speed
 * (field_A4) and lateral component (field_A8) along its heading (field_24) into
 * the world position, then recomputes headingAngle. Called by each of the
 * state98 motion handlers. Register pins and the single-param/two-arg call
 * mismatch are deliberate to match; do not "fix".
 */
void AdvanceCarPosition(GameCarRuntime *arg0);
void AdvanceCarPosition(GameCarRuntime *arg0) {
    volatile s32 coords[3];

    {
        s32 angleSin;
        s32 otherSin;

        angleSin = rsin(arg0->headingAngle);
        otherSin = rsin(arg0->field_24);
        coords[0] = (((angleSin * arg0->field_A4) / 4) + (otherSin * arg0->field_A8)) / 100;
    }

    {
        s32 angleCos;
        s32 otherCos;

        angleCos = rcos(arg0->headingAngle);
        otherCos = rcos(arg0->field_24);
        coords[2] = (((angleCos * arg0->field_A4) / 4) + (otherCos * arg0->field_A8)) / 100;
    }

    {
        s32 angleSin;
        s32 otherSin;
        s32 angleCos;
        s32 otherCos;

        angleSin = rsin(arg0->headingAngle);
        otherSin = rsin(arg0->field_24);
        angleCos = rcos(arg0->headingAngle);
        otherCos = rcos(arg0->field_24);

        arg0->field_A4 += ((((angleSin * otherSin) + (angleCos * otherCos)) / 4096) * arg0->field_A8) / 4096;
    }
    arg0->headingAngle = 0x400 - Atan2(coords[0], coords[2]);
}
