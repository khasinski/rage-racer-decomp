#include "common.h"

extern volatile u32 *D_8009AF20[];
extern volatile u32 *D_8009AF2C[];
extern s32 D_8009AF28;
extern s32 D_8009AF34;
extern s32 D_8009AF74;
extern s32 D_8009F094;
extern s16 D_8009AF50;
extern s16 D_8009AF52;
extern s32 D_8019CA1C;
extern s32 D_801E40B8;
extern u16 D_801E436E;
extern s32 D_801E8A90;
extern char D_80010D30[];

void func_80065860(s32 arg0);
void DecDCTin(volatile u32 *arg0, s32 arg1) asm("func_80063FB0");
void func_8006402C(volatile u32 *arg0, s32 arg1);
s32 func_8001ED3C(volatile void *arg0);
s32 func_8006CF08(void *arg0);
void func_8001674C(char *arg0, ...);
void func_8001F018(void *arg0);
void func_8001EF54(volatile void *arg0, s32 arg1);
void func_80042CCC(s32 arg0);

void func_8001E8A4(void) {
    register s32 value asm("$16");
    u8 sp10[16];

    D_801E40B8++;
    if (D_801E40B8 == 4) {
        func_80065860(1);
    }

    DecDCTin(D_8009AF20[D_8009AF28], 3);
    func_8006402C(D_8009AF2C[D_8009AF34], (D_8009AF50 * D_8009AF52) / 2);

    {
        register s32 fail asm("$17");

        fail = -1;
        while (func_8001ED3C(D_8009AF20) == fail) {
            value = func_8006CF08(sp10);
            func_8001674C(D_80010D30, value);
            if ((D_8019CA1C < (u32)value) || (value < 0)) {
                func_8001F018((void *)D_801E8A90);
            } else {
                func_8001F018(sp10);
            }
        }
    }

    func_8001EF54(D_8009AF20, 0);
    if (D_8009AF74 == 1) {
        D_8009F094 = 2;
    }
    if (D_801E436E & 0x800) {
        func_80042CCC(1);
        D_8009F094 = 2;
    }
}
