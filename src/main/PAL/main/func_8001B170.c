#include "common.h"

extern u8 *D_8019C900;
extern s32 D_801E6F28;
extern s32 D_8009E694;

s32 func_80068568(s32 arg0);
void func_8001B0F0(s32 arg0);
void *func_80016F8C(void *arg0, void *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9);
void *func_80017390(void *arg0, void *arg1, s32 arg2);

void func_8001B170(void) {
    void **scratch;
    u8 *base;
    void *next;
    s32 sinValue;
    s32 frame;

    if (D_801E6F28 > 0) {
        func_8001B0F0((u8)D_801E6F28);
        D_801E6F28 -= 2;
    }

    sinValue = func_80068568(((D_8009E694 * 3) << 5) & 0xFE0);
    frame = (sinValue / 64) + 0x80;

    scratch = (void **)0x1F800000;
    base = D_8019C900;
    base += 0xCC;
    next = *scratch;
    next = func_80016F8C(base, next, 0x68, 0xC8, 0x70, 0x10, 0x70, 0xA0, 0x7E84, frame);
    *scratch = func_80017390(base, next, 0x39);
}
