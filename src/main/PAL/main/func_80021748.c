#include "common.h"

extern s32 D_801E40B8;
extern volatile u16 D_801E436E;
extern s32 D_801E3E0C;
extern s32 D_801E42E4;
extern u8 *D_8009E67C;

void func_8005D6EC(s32 arg0);
void func_80018410(void);
void func_80033AA0(s32 arg0, s32 arg1);
void func_80021654(void);

void func_80021748(void) {
    s32 timer;
    s32 old;
    s32 current;
    u8 *ptr;
    u16 value;

    timer = D_801E40B8;
    if (timer == -1) {
        old = D_801E3E0C;
        if ((D_801E436E & 0x1000) && (old == 1)) {
            D_801E3E0C = 0;
        }
        if ((D_801E436E & 0x4000) && (D_801E3E0C == 0)) {
            D_801E3E0C = 1;
        }
        current = D_801E3E0C;
        if (old != current) {
            func_8005D6EC(1);
        }
        if (D_801E436E & 0x800) {
            func_8005D6EC(2);
            if (D_801E3E0C != 0) {
                func_80018410();
            }
            ptr = D_8009E67C;
            value = *(u16 *)(ptr + 6);
            D_801E40B8 = 0;
            *(u16 *)(ptr + 6) = value - 1;
        }
    } else {
        timer += 2;
        D_801E40B8 = timer;
        func_80033AA0(timer, 0x49);
        if (D_801E40B8 == 0x100) {
            if (D_801E3E0C != 0) {
                D_801E42E4 = 6;
            } else {
                D_801E42E4 = 0xB;
            }
        }
    }

    func_80021654();
}
