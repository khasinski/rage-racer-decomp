#include "common.h"

extern volatile s32 D_8007D7AC;

s32 func_8006DD30(s32 arg0);
s32 func_8002745C(s32 arg0);
void func_8006A554(s32 arg0, s32 arg1);

s32 func_80027790(s32 arg0, s32 arg1) {
    register s32 savedArg0 asm("$18");
    register s32 savedArg1 asm("$19");
    register volatile s32 *state asm("$17");
    register s32 result asm("$16");

    savedArg0 = arg0;
    savedArg1 = arg1;
    state = &D_8007D7AC;

    do {
        register s32 now asm("$2");

        now = func_8006DD30(-1);
        if (state[0] + 0x4B0 < now) {
            result = -1;
            goto loop_check;
        }

        if (state[-2] < 0) {
            goto reset_read;
        }

        now = func_8006DD30(-1);
        if (state[-1] + 0x3C < now) {
reset_read:
            func_8002745C(1);
            result = state[-7];
            goto loop_check;
        }

        result = state[-2];

loop_check:
        if (savedArg0 != 0) {
            break;
        }
    } while (result > 0);

    func_8006A554(1, savedArg1);
    return result;
}

asm(".globl func_80027808\nfunc_80027808 = func_80027790 + 0x78");
