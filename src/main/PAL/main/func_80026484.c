#include "common.h"

extern s32 D_8007BED8;
extern s32 D_801E42A0;
extern s32 D_801E42E0;
extern s32 D_801E42E4;
extern char D_80011010[];

void func_80033AA0(s32 arg0, s32 arg1);
void func_80016EA0(s32 arg0, s32 arg1, char *arg2, s32 arg3);

void func_80026484(void) {
    s32 delta;

    if (D_8007BED8 == 0) {
        D_801E42A0 = 4;
    }

    delta = D_801E42A0;
    if (delta < 0) {
        D_801E42E0 += delta;
        if (D_801E42E0 < 0) {
            D_801E42E0 = 0;
            D_801E42A0 = 0;
        }
        func_80033AA0(D_801E42E0, 0x49);
    } else if (delta > 0) {
        D_801E42E0 += delta;
        func_80033AA0(D_801E42E0, 0x49);
        if (D_801E42E0 >= 0x101) {
            D_801E42E4 = 0x16;
        }
    }

    func_80016EA0(0x5E, 0x72, D_80011010, 0x7812);
}
