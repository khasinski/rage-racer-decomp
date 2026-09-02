#include "common.h"
#include "game/prim.h"
#include "game/race.h"
#include "game/save_internal.h"
#include "game/fmv_internal.h"
#include "game/render.h"
#include "game/scratchpad.h"
#include "game/screens.h"
#include "game/state.h"
#include "game/vector.h"
#include "psyq/gpu.h"

#ifndef RACE_CLOCK_TICKS_PER_SECOND
#define RACE_CLOCK_TICKS_PER_SECOND 25
#endif

void DrawTimeValue(s32 x, s32 y, s32 value, s32 color, s32 divisor) {
    s32 savedX;
    register s32 savedY asm("$10");
    s32 savedColor;
    register s32 localDivisor asm("$4");
    s32 whole;
    s32 fraction;
    s32 minutes;
    s32 seconds;
    s32 secondTens;
    s32 fractionHundreds;
    s32 fractionTens;
    s32 remainder;

    savedX = x;
    savedY = y;
    localDivisor = divisor;
    savedColor = color;
    if (value >= 0) {
        whole = value / localDivisor;
        remainder = value % localDivisor;

        minutes = whole / 60;
        fraction = (remainder * 1000) / localDivisor;
        seconds = whole % 60;
        g_TimeTextBuffer[0] = minutes + '0';

        secondTens = seconds / 10;
        g_TimeTextBuffer[2] = secondTens + '0';
        g_TimeTextBuffer[3] = (seconds - (secondTens * 10)) + '0';

        fractionHundreds = fraction / 100;
        g_TimeTextBuffer[5] = fractionHundreds + '0';

        fractionTens = fraction / 10;
        g_TimeTextBuffer[6] = (fractionTens - (fractionHundreds * 10)) + '0';
        g_TimeTextBuffer[7] = (fraction - (fractionTens * 10)) + '0';
    } else {
        g_TimeTextBuffer[0] = '-';
        g_TimeTextBuffer[2] = '-';
        g_TimeTextBuffer[3] = '-';
        g_TimeTextBuffer[5] = '-';
        g_TimeTextBuffer[6] = '-';
        g_TimeTextBuffer[7] = '-';
    }

    DrawText8x8(savedX, savedY, g_TimeTextBuffer, savedColor);
}

void DrawMinuteSecondTime(s32 x, s32 y, s32 ticks, s32 color) {
    s32 savedY;
    s32 sec;
    s32 tmp;
    s32 min;
    u8 *p;
    s32 tens;
    s32 tens2;

    savedY = y;
    sec = ticks / RACE_CLOCK_TICKS_PER_SECOND;
    tmp = sec / 60;
    min = tmp;
    tmp = sec - min * 60;
    if (min < 10) {
        g_ClockTextBuffer = ' ';
    } else {
        g_ClockTextBuffer = min / 10 + '0';
    }
    tens = min / 10;
    p = g_ClockTextMinUnits;
    tens2 = tmp / 10;
    *p = min - tens * 10 + '0';
    g_ClockTextSecTens = tens2 + '0';
    g_ClockTextSecUnits = tmp - tens2 * 10 + '0';
    DrawText8x8(x, savedY, p - 1, color);
}
