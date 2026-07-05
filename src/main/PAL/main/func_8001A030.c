#include "common.h"

extern s32 D_8019CA6C;
extern s32 D_801E8AF8;

void func_80019F24(void);
s32 func_8006DD30(s32 arg0);

void func_8001A030(void) {
    while (D_8019CA6C != D_801E8AF8) {
        if (func_8006DD30(1) >= 471) {
            break;
        }

        if (D_8019CA6C < D_801E8AF8) {
            func_80019F24();
            D_8019CA6C++;
        } else {
            D_8019CA6C--;
            func_80019F24();
        }
    }
}
