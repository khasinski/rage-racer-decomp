#include "common.h"
extern s32 D_801E40B8, D_801E42E0, D_801E42A0, D_801E42E4;
extern u16 D_801E436E;
void func_80065860(s32 arg0);
void func_800218A0(s32 arg0);
void func_80022FAC(void) {
    s32 v0, v1;
    if ((D_801E40B8 = D_801E40B8 + 1) == 2) {
        func_80065860(1);
    }
    v0 = D_801E42E0 + D_801E42A0;
    D_801E42E0 = v0;
    v1 = D_801E42A0;
    if (v1 > 0) {
        if (v0 >= 257) {
            D_801E42E0 = 0x100;
            D_801E42A0 = 0;
        }
    } else if (v1 == 0) {
        if (D_801E40B8 == 0x12C || (D_801E436E & 0x860)) {
            D_801E42E0 = 0x100;
            D_801E42A0 = -4;
        }
    } else if (v0 == 0) {
        D_801E42E4 = 2;
    }
    func_800218A0(D_801E42E0);
}
