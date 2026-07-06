#include "common.h"

extern s32 D_8009B280;
extern u8 D_8009B284;

void func_80047024(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);

void func_8004A17C(s32 delta) {
    u8 *scratch;
    register s32 value asm("$2");
    s32 limit;
    s32 alpha;

    scratch = *(u8 **)0x1F800004;
    if (delta > 0) {
        value = D_8009B280 + delta;
        D_8009B280 = value;
        if (0xFFFF < value) {
            D_8009B280 = 0xFFFF;
        }
    } else {
        value = D_8009B280 + delta;
        D_8009B280 = value;
        if (value < 0) {
            D_8009B280 = 0;
        }
    }

    limit = 0x1E0;
    D_8009B284 = D_8009B280 >> 8;
    alpha = D_8009B284;
    func_80047024(scratch + 0x18, 0x48, 0, 0xF8, limit, alpha, alpha, alpha, 0x40);
}
