#include "common.h"

extern s32 D_8019C704;
extern s32 D_8019C868;
extern s32 D_8019CB14;
extern u16 D_801E436E;
extern s32 D_801E4D68;
extern s32 D_801E6C70;
extern s32 D_801E8A50;

void func_80024B6C(void);
void func_8005D6EC(s32 arg0);

void func_80024F28(void) {
    u16 *buttonsPtr;
    u16 buttons;
    s32 old;
    s32 index;

    func_80024B6C();
    buttonsPtr = &D_801E436E;
    buttons = *buttonsPtr;
    old = D_8019C868;
    if (buttons & 0x1000) {
        D_8019C868 = old - 1;
    }
    if (buttons & 0x4000) {
        D_8019C868++;
    }

    index = (D_8019C868 + 4) % 4;
    D_8019C868 = index;
    if (old != index) {
        func_8005D6EC(1);
    }

    buttons = *buttonsPtr;
    if (buttons & 0x860) {
        func_8005D6EC(2);
        D_8019CB14 = 5;
        switch (D_8019C868) {
        case 0:
            D_801E4D68 = D_8019C704;
            break;
        case 1:
            D_801E4D68 = D_801E8A50;
            break;
        case 2:
            D_801E4D68 = D_801E6C70;
            break;
        case 3:
            D_8019CB14 = 1;
            break;
        }
    } else if (buttons & 0x90) {
        func_8005D6EC(3);
        D_8019CB14 = 1;
    }
}
