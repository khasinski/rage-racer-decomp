#include "common.h"

extern u8 *D_8019C900;
extern s16 D_8019CAC0;
extern s32 D_801E4184;
extern s32 D_8009F098;
extern s32 D_801E429C;
extern s32 D_801E6F1C;

void *func_800175A4(void *arg0, void *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9, s32 arg10, s32 arg11);

void func_8001B2D4(void) {
    void *scratch;
    u8 *base;
    s32 row;
    s32 i;
    s32 one;
    s32 width;
    s32 y;

    base = D_8019C900 + 0xCC;
    scratch = *(void **)0x1F800000;
    row = 0;
    i = 0;
    one = 1;
    width = 0x70;
    y = 0x64;

    while (i < 5) {
        s32 code;
        s32 frame;
        s32 delta;

        code = 0x7E85;

        if ((D_8019CAC0 == 0) && (i == one)) {
            i = 2;
        }

        if (i == D_801E4184) {
            code = 0x7E86;
        }

        if (D_8009F098 == one) {
            code = 0x7E85;
        }

        if ((D_801E429C & 2) != 0) {
            code = 0x7E85;
        }

        delta = D_801E6F1C - (row * 8);
        if (delta >= 0) {
            frame = delta;
            if (frame >= 0x11) {
                frame = 0x10;
            }
        } else {
            frame = 0;
        }

        scratch = func_800175A4(base, scratch, 0x68, y, width, frame, 0, (i << 4) + 0xA0, width, 0x10, code, 0x39);
        y += 0x18;
        i++;
        row++;
    }

    *(void **)0x1F800000 = scratch;
}
