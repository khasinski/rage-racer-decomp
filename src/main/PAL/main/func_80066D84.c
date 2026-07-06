#include "common.h"

extern volatile u32 *D_800942CC;
extern volatile u32 *D_800942D0;
extern volatile u32 *D_800942D4;
extern volatile u32 *D_800942D8;

void func_80067F04(void);
s32 func_80067F38(void);

s32 func_80066D84(u32 *arg0, s32 arg1) {
    register s32 size asm("$16");
    register u32 mask asm("$17");
    register volatile u32 *status asm("$5");
    register s32 offset asm("$2");

    size = arg1;
    status = D_800942D8;
    *status |= 0x08000000;
    *D_800942D4 = 0;
    __asm__ volatile("" ::: "memory");
    offset = (size << 2) - 4;
    arg0 = (u32 *)((u8 *)arg0 + offset);
    *D_800942CC = (u32)arg0;
    *D_800942D0 = size;
    *D_800942D4 = 0x11000002;
    func_80067F04();

    if ((*D_800942D4 & 0x01000000) != 0) {
        mask = 0x01000000;
        while (1) {
            register u32 statusValue asm("$2");

            if (func_80067F38() != 0) {
                return -1;
            }
            statusValue = *D_800942D4;
            statusValue &= mask;
            if (statusValue == 0) {
                break;
            }
        }
    }
    return size;
}
