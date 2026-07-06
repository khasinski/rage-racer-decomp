#include "common.h"

extern u8 *D_8019C900;

void AddPrim(void *ot, void *prim) asm("func_80064DDC");

void func_80033308(s32 arg0, s32 arg1) {
    register u8 *base __asm("$16");
    register u8 *prim __asm("$17");
    register s32 mode __asm("$18");
    register s32 firstOffset __asm("$5");
    register s32 value __asm("$4");
    register s32 temp __asm("$2");
    register u8 *ot __asm("$4");

    mode = arg1;
    firstOffset = 0x237AC;
    temp = 0x237C0;
    value = arg0 << 3;
    base = D_8019C900;
    value += 0x50;
    prim = base + temp;

    *(base + 0x237B8) = value;
    AddPrim(D_8019C900 + 0xCC, base + firstOffset);
    firstOffset = (s32)prim;

    if (mode > 0) {
        *(volatile u8 *)(base + 0x237CC) = 0x88;
        __asm__ volatile("" : : : "memory");
        ot = D_8019C900;
        __asm__ volatile("" : : "r"(ot));
        temp = 0x7810;
        goto add_second;
    }

    if (mode >= 0) {
        return;
    }

    *(volatile u8 *)(base + 0x237CC) = 0x78;
    __asm__ volatile("" : : : "memory");
    ot = D_8019C900;
    __asm__ volatile("" : : "r"(ot));
    temp = 0x780F;

add_second:
    *(u16 *)(base + 0x237CE) = temp;
    AddPrim(ot + 0xCC, (void *)firstOffset);
}
