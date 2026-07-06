#include "common.h"

extern s32 D_8007BED8;
extern s32 D_8007C704;

void func_80042C94(void);

s32 func_80018B98(void) {
    s32 state;

    if (D_8007BED8 != 0) {
        return 1;
    }

    state = 7;
    if (D_8007C704 == state) {
        D_8007C704 = 0;
        return 0;
    }

    func_80042C94();
    D_8007C704 = state;
    D_8007BED8 = 1;
    return 1;
}
