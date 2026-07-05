#include "common.h"

extern s16 D_801E4DAC;
extern s16 D_801E6C90;

void func_8005D6EC(s32 arg0);

void func_800410BC(s32 arg0) {
    if (arg0 < 0x110) {
        if (arg0 == 0x79) {
            func_8005D6EC(0x1E);
        } else if (arg0 == 0x97) {
            func_8005D6EC(0x1F);
        } else if (arg0 == 0xB5) {
            func_8005D6EC(0x20);
        } else if (arg0 == 0xD3) {
            func_8005D6EC(0x21);
        } else if ((arg0 == 0x10F) && (D_801E4DAC == 1)) {
            func_8005D6EC(0x22);
            D_801E6C90 = 0x5A;
        }
    }
}
