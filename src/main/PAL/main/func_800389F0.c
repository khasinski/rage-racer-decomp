#include "common.h"

extern u8 D_801F1854[];

void func_80017A10(s32 arg0);
void func_8001DFC0(void *arg0);

void func_800389F0(void) {
    register u8 *base asm("s1");
    register s32 i asm("s2");
    register u8 *entry asm("s0");
    register s32 one asm("s3");
    register s32 minus_one asm("s4");

    base = D_801F1854;
    func_80017A10(1);

    i = 0;
    minus_one = -1;
    one = 1;
    entry = base + 0xBC;
    do {
        if (*(s16 *)(entry - 0x10) != (i++, minus_one)) {
            if (*(s32 *)entry == one) {
                func_8001DFC0(base);
            }
        }
        entry += 0x19C;
        base += 0x19C;
    } while (i < 11);
}
