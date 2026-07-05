#include "common.h"

extern s32 D_8019C868;
extern s32 D_8019C704;
extern s32 D_801E8A50;
extern s32 D_801E6C70;
extern s32 D_8019CB14;
extern s32 D_801E4D68;
extern volatile u16 D_801E436E;

void func_80024B6C(void);
void func_80021224(void);
void func_8005D6EC(s32 arg0);

void func_800250BC(void) {
    s32 old;
    u16 pad;

    func_80024B6C();

    switch (D_8019C868) {
    case 0:
        old = D_8019C704;
        if ((D_801E436E & 0x8000) && old > 0) {
            D_8019C704 = old - 1;
        }
        if (D_801E436E & 0x2000) {
            s32 value = D_8019C704;
            if (value < 15) {
                D_8019C704 = value + 1;
            }
        }
        if (old != D_8019C704) {
            func_8005D6EC(1);
        }
        pad = D_801E436E;
        if (pad & 0x860) {
            D_8019CB14 = 4;
        } else if (pad & 0x90) {
            D_8019CB14 = 4;
            D_8019C704 = D_801E4D68;
        }
        break;

    case 1:
        old = D_801E8A50;
        if ((D_801E436E & 0x8000) && old > 0) {
            D_801E8A50 = old - 1;
        }
        if (D_801E436E & 0x2000) {
            s32 value = D_801E8A50;
            if (value < 15) {
                D_801E8A50 = value + 1;
            }
        }
        if (old != D_801E8A50) {
            func_8005D6EC(1);
        }
        pad = D_801E436E;
        if (pad & 0x860) {
            D_8019CB14 = 4;
        } else if (pad & 0x90) {
            D_8019CB14 = 4;
            D_801E8A50 = D_801E4D68;
        }
        break;

    case 2:
        old = D_801E6C70;
        if ((D_801E436E & 0x8000) && old > 0) {
            D_801E6C70 = old - 1;
        }
        if (D_801E436E & 0x2000) {
            s32 value = D_801E6C70;
            if (value <= 0) {
                D_801E6C70 = value + 1;
            }
        }
        if (old != D_801E6C70) {
            func_8005D6EC(1);
        }
        pad = D_801E436E;
        if (pad & 0x860) {
            D_8019CB14 = 4;
        } else if (pad & 0x90) {
            D_8019CB14 = 4;
            D_801E6C70 = D_801E4D68;
        }
        break;
    }

    func_80021224();
    pad = D_801E436E;
    if (pad & 0x860) {
        func_8005D6EC(2);
    } else if (pad & 0x90) {
        func_8005D6EC(3);
    }
}
