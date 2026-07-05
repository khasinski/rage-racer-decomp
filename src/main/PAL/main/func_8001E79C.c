#include "common.h"

extern s32 D_8009AF6C;
extern s32 D_8009AF70;
extern s32 D_8009AF74;
extern s32 D_8009F094;
extern s32 D_8009E678;
extern volatile u8 D_8009AF20;
extern u8 D_801C0638;
extern u8 D_8019CE50;
extern u8 D_801C068D;
extern u8 D_8019CEA5;
extern s32 D_801E40B8;
extern s32 D_801E8A90;

void func_80065860(s32 arg0);
void func_8004310C(void);
void func_8001BE9C(s32 arg0, s32 arg1, s32 arg2);
void func_8001EB14(s32 arg0);
void func_8001EA7C(volatile void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void func_8001EBC8(void);
void func_8001EB5C(void (*arg0)(void));
s32 func_8001ED3C(volatile void *arg0);
void func_8001F018(s32 arg0);
void func_8006DD30(s32 arg0);

void func_8001E79C(s32 arg0) {
    register s32 fail asm("$16");
    char frame_pad[8];

    __asm__("" : : "m"(frame_pad[7]));
    func_80065860(0);
    D_8009AF6C = 0;
    D_8009AF70 = 0;
    func_8004310C();
    func_8001BE9C(0, 0, 0);
    D_801C0638 = 0;
    D_8019CE50 = 0;
    D_801C068D = 1;
    D_8019CEA5 = 1;
    func_8001EB14(arg0);
    {
        register volatile void *buf asm("$4") = &D_8009AF20;
        func_8001EA7C(buf, 0, 0x18, 0, 0x108);
    }
    func_8001EB5C(func_8001EBC8);
    fail = -1;
    while (1) {
        register volatile void *buf asm("$4") = &D_8009AF20;
        if (func_8001ED3C(buf) != fail) {
            break;
        }
        func_8001F018(D_801E8A90);
    }
    D_8009AF74 = 0;
    D_801E40B8 = 0;
    D_8009F094 = 1;
    func_8006DD30(0);
    D_8009E678 = 0;
}
