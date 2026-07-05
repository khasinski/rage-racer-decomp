#include "psyq/spu.h"
#include "psyq/kernel.h"

extern s32 D_8009A714;
extern s32 D_8009A768;
extern s32 D_8009ABAC;

s32 SpuIsTransferCompleted(s32 arg0) {
    register s32 one asm("$16");
    register s32 saved_arg asm("$17");
    register s32 ret asm("$2");

    saved_arg = arg0;
    one = 1;

    if (D_8009A714 == one) {
        ret = one;
    } else if (D_8009ABAC == one) {
        ret = one;
    } else {
        ret = TestEvent(D_8009A768);

        if (saved_arg == one) {
            if (ret == 0) {
                do {
                    ret = TestEvent(D_8009A768);
                } while (ret == 0);
            }

            ret = 1;
            D_8009ABAC = ret;
        } else if (ret == one) {
            D_8009ABAC = ret;
        }
    }

    return ret;
}
