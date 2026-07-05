#include "common.h"
#include "psyq/cd.h"

s32 func_8006A5A4(s32 arg0, void *arg1, s32 arg2);
void func_80027688(s32 arg0, void *arg1, s32 arg2);
s32 func_80027790(s32 arg0, s32 arg1);

s32 func_8006CB88(s32 arg0, s32 arg1, void *arg2) {
    s32 scratch[2];

    CdIntToPos(arg1, (CdlLOC *)scratch);
    func_8006A5A4(2, scratch, 0);
    func_80027688(arg0, arg2, 0x80);
    return func_80027790(0, 0) < 1U;
}
