#include "common.h"

extern s32 D_8009B334;
extern s32 D_8009B340;
extern s32 D_8009B34C;
extern s32 D_8009B350;
extern s32 D_8009B358;
extern s32 D_8009B35C;
extern s32 D_8009B360;
extern s32 D_8009B364;
extern s32 D_8009B368;
extern s32 D_8019C9F0;
extern s32 D_8019C9F8;
extern s32 D_801E8268;
extern s32 D_801E428C;
extern u8 *D_8009E67C;
extern s32 D_8009E6D4;
extern s32 D_8009E6D8;
extern s32 D_8009E6DC;
extern s32 D_8009E6F4;
extern s32 D_8009E6F8;
extern s32 D_8009E6FC;
extern s32 D_8009E718;
extern s32 D_8009E71C;
extern s32 D_8009E744;
extern u8 D_8007F45C;
extern u8 D_8007F460[];
extern u8 D_8007BEDC[];
extern u8 D_8007BEE4[];
extern u8 D_801E444C[];
extern u8 D_801E6F2C[];

void func_80052738(void);
s32 func_80018530(void);
void func_8005E88C(void);
void func_80049418(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void func_80065B24(void *arg0, void *arg1);
void func_8001D530(void *arg0, s32 arg1);

void func_80052778(void) {
    register s32 one asm("$16");
    register s32 initValue asm("$3");
    register s32 mode asm("$4");
    register s32 largeValue asm("$5");
    register u8 *table asm("$2");
    register s32 eight asm("$2");

    func_80052738();
    if (func_80018530() != 0) {
        return;
    }

    one = 1;
    func_8005E88C();
    D_8009B340 = one;
    D_8019C9F8 = one;
    func_80049418(0, 0, 0, 0);

    initValue = 0x7A120;
    mode = 0x3D090;
    largeValue = 0x1F0000;
    asm volatile("" : "=r"(largeValue) : "0"(largeValue));
    D_8009B358 = mode;
    mode = D_801E428C;
    eight = 8;
    D_801E8268 = eight;
    table = D_8009E67C;
    largeValue |= 0x4000;
    D_8019C9F0 = 0;
    D_8009E6D4 = 0;
    D_8009E6D8 = 0;
    D_8009E6DC = 0;
    D_8009E6F4 = 0;
    D_8009E6F8 = 0;
    D_8009E6FC = 0;
    D_8009E744 = 0;
    D_8009E718 = 0;
    D_8009E71C = 0;
    D_8009B350 = 0x7A120;
    D_8009B34C = initValue;
    D_8009B35C = 0;
    D_8009B360 = largeValue;
    D_8009B364 = 0;
    D_8009B368 = table[mode & 3];

    if (mode >= 4) {
        D_8009B334 = one;
    } else {
        D_8009B334 = -1;
    }

    func_80065B24(D_8007BEE4, D_801E6F2C);
    func_80065B24(D_8007BEDC, D_801E444C);
    func_8001D530(D_8007F460, D_8007F45C);
}
