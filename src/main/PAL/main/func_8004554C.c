#include "common.h"

void func_80069A38(s32 arg0, s32 arg1, s32 arg2);
void func_80069B14(void *arg0, s32 arg1, void *arg2);

void func_8004554C(u8 *arg0, u8 *arg1, u8 *out, s32 arg3) {
    s32 local[3];
    u8 result[4];

    local[0] = arg0[0] << 4;
    local[1] = arg0[1] << 4;
    local[2] = arg0[2] << 4;
    func_80069A38(arg1[0], arg1[1], arg1[2]);
    func_80069B14(local, arg3, result);
    out[0] = result[0];
    out[1] = result[1];
    out[2] = result[2];
}
