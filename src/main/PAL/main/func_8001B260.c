#include "common.h"
extern u16 D_801E436E;
extern s32 D_8019CB70;
extern s32 D_8009F098;
extern s32 D_801E8260;
extern s32 D_801E4184;
void func_8005D6EC(s32 arg0);
void func_80042CCC(s32 arg0);
void func_8001B170(void);
void func_8001B260(void) {
    if (D_801E436E & 0x800) {
        func_8005D6EC(2);
        D_8009F098 = 1;
        D_801E8260 = 0;
        D_801E4184 = 0;
        if (D_8019CB70 > 0) {
            D_8019CB70 = 0;
            func_80042CCC(1);
        }
    }
    func_8001B170();
}
