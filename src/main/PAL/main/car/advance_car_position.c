#include "common.h"
#include "game/car.h"
#include "game/render.h"

s32 rsin(s32 angle);
s32 rcos(s32 angle);

/*
 * Shared "advance car position/heading" helper. Integrates the car's speed
 * (field_A4) and lateral component (field_A8) along its heading (field_24) into
 * the world position, then recomputes headingAngle. Called by each of the
 * state98 motion handlers. Register pins and the single-param/two-arg call
 * mismatch are deliberate to match; do not "fix".
 */
void AdvanceCarPosition(GameCarRuntime *car) {
    volatile s32 coords[3];

    {
        s32 angleSin;
        s32 otherSin;

        angleSin = rsin(car->headingAngle);
        otherSin = rsin(car->field_24);
        coords[0] = (((angleSin * car->field_A4) / 4) + (otherSin * car->field_A8)) / 100;
    }

    {
        s32 angleCos;
        s32 otherCos;

        angleCos = rcos(car->headingAngle);
        otherCos = rcos(car->field_24);
        coords[2] = (((angleCos * car->field_A4) / 4) + (otherCos * car->field_A8)) / 100;
    }

    {
        s32 angleSin;
        s32 otherSin;
        s32 angleCos;
        s32 otherCos;

        angleSin = rsin(car->headingAngle);
        otherSin = rsin(car->field_24);
        angleCos = rcos(car->headingAngle);
        otherCos = rcos(car->field_24);

        car->field_A4 += ((((angleSin * otherSin) + (angleCos * otherCos)) / 4096) * car->field_A8) / 4096;
    }
    car->headingAngle = ANGLE_QUARTER_TURN - Atan2(coords[0], coords[2]);
}
