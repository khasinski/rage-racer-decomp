#include "common.h"
extern s32 *D_801E4150;
extern s32 D_801E40CC, D_8009E66C, D_8009E870, D_8009E694, D_801E40B8, D_8019C768;
extern u8 D_801F18CC[];
extern s32 *D_8019C9A8;
void func_80017884(s32 arg0);
void func_8001D30C(void);
void func_8001D210(void);
void func_80038844(void);
void func_80019E84(s32 arg0);
void func_800458CC(s32 arg0);
void func_8003F0F8(void);
void func_80025940(void) {
    func_80017884(5);
    func_8001D30C();
    func_8001D210();
    D_801E40CC = *D_801E4150;
    func_80038844();
    func_80019E84(*(s16 *)&D_801F18CC[D_8009E66C * 412]);
    func_800458CC(D_8019C9A8[2]);
    D_8009E870 = 2;
    D_8009E694 = 0;
    D_801E40B8 = 0;
    D_8019C768 = 0x180;
    func_8003F0F8();
}
