#include "common.h"
extern u16 D_801E436E;
extern s32 D_801E4D68, D_801E4D6C, D_8019CB14;
void func_80024420(void);
void func_8005D6EC(s32 arg0);
void func_80023FE8(void);
void func_80024820(void) {
    s32 oldCursor;
    s32 oldFlag;
    u16 b;
    func_80024420();
    oldCursor = D_801E4D68;
    oldFlag = D_801E4D6C;
    if ((D_801E436E & 0x1000) && oldFlag == 1) {
        D_801E4D6C = 0;
    }
    if ((D_801E436E & 0x4000) && D_801E4D6C == 0) {
        D_801E4D6C = 1;
    }
    b = D_801E436E;
    if (b & 0x8000) {
        D_801E4D68 = D_801E4D68 - 1;
    }
    if (b & 0x2000) {
        D_801E4D68 = D_801E4D68 + 1;
    }
    {
        register s32 c asm("$4");
        c = D_801E4D68 + 6;
        c = c % 6;
        D_801E4D68 = c;
        if (c == 5) {
            D_801E4D6C = 0;
        }
        if (oldCursor != c || oldFlag != D_801E4D6C) {
            func_8005D6EC(1);
        }
    }
    if (D_801E436E & 0x8F0) {
        func_8005D6EC(2);
        D_8019CB14 = 2;
    }
    func_80023FE8();
}
