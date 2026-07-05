#include "common.h"
extern s32 D_801E4BC8;
void func_800414F0(s32 arg0, s32 arg1);
void func_80069858(s32 arg0);
void func_80027FF4(s32 arg0, s32 arg1, s32 arg2);
void func_80041888(void) {
    func_800414F0(0xFFFF6000, 0x14000);
    func_80069858(0x1F800028);
    func_80027FF4(0x1F800000, D_801E4BC8, 0x40);
}
