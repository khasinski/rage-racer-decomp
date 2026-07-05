#include "common.h"

extern s32 D_801E42E0;
extern s32 D_801E42A0;
extern s32 D_8019CB14;
extern s32 D_8019C7B0;
extern s32 D_801E42E4;

void func_80023A60(s32 arg0, s32 arg1);
void func_80023BB4(void);

void func_80023B2C(void) {
    D_801E42E0 += D_801E42A0;

    if (D_801E42E0 < 0) {
        D_801E42A0 = 0;
        D_8019CB14 = 1;
    } else if (D_801E42E0 >= 0x101) {
        D_801E42E4 = D_8019C7B0;
    }

    func_80023A60(D_801E42E0, 0x49);
    func_80023BB4();
}
