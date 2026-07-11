#include "common.h"

extern u8 D_8007DF10;
extern u8 D_8007DF11[];
extern u8 D_8007DF13;
extern u8 D_8007DF14;

void func_80016754(s32 x, s32 y, void *str, s32 color);

void func_80033F30(s32 x, s32 y, s32 ticks, s32 color) {
    register s32 savedY asm("$5");
    s32 sec;
    s32 tmp;
    s32 min;
    u8 *p;
    s32 tens;
    s32 tens2;

    savedY = y;
    sec = ticks / 25;
    tmp = sec / 60;
    min = tmp;
    tmp = sec - min * 60;
    if (min < 10) {
        D_8007DF10 = ' ';
    } else {
        D_8007DF10 = min / 10 + '0';
    }
    tens = min / 10;
    p = D_8007DF11;
    tens2 = tmp / 10;
    *p = min - tens * 10 + '0';
    D_8007DF13 = tens2 + '0';
    D_8007DF14 = tmp - tens2 * 10 + '0';
    func_80016754(x, savedY, p - 1, color);
}
