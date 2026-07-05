#include "common.h"
extern s32 D_8007BED8, D_8019C768, D_801E4B30, D_8019CACC, D_801E42E4, D_801E40B8;
extern s32 D_8009F0A0, D_801E42E0, D_8019CB14, D_801E42A0;
void func_80065860(s32 arg0);
void func_8001A3C0(s32 arg0);
void func_80017884(s32 arg0);
void func_8001C088(s32 arg0, s32 arg1, s32 arg2);
void func_800234DC(void);
void func_8001A610(void);
void func_800235D8(void) {
    func_80065860(0);
    D_8019C768 = 0x80;
    if (D_8007BED8 == 0) {
        func_8001A3C0(D_801E4B30);
        D_8019CACC = 0;
        func_80017884(5);
        func_8001C088(0, 0, 0);
        D_801E42E4 = 0x17;
        D_801E40B8 = 0;
        func_800234DC();
        *(s32 *)0x1F800008 = 0;
        *(s32 *)0x1F80000C = 0;
        *(s32 *)0x1F800010 = -3520;
        *(s32 *)0x1F800018 = 0;
        *(s32 *)0x1F80001C = 0;
        *(s32 *)0x1F800020 = 0;
        func_8001A610();
        D_8009F0A0 = 0xF0;
        D_801E42E0 = 0x100;
        D_8019CB14 = 0;
        D_801E42A0 = -8;
    }
}
