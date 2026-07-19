#include "common.h"

extern u8 *D_8019C900;
extern u8 D_8007D7BC[];
extern u8 D_8007D7BD[];
extern u8 D_8007D87C[];
extern u8 D_8007BED0[];

void func_80064FA8(u8 *prim);
void func_80064EB8(u8 *prim, s32 enabled);
void func_80064DDC(void *ot, void *prim);
void *func_800666F4(void *prim, s32 a, s32 b, s32 c, void *d);

void func_80027874(s32 x, s32 y, u8 *str, s32 arg3) {
    register u8 *packet __asm("$18");
    s32 idx;
    register u8 *next __asm("$20");
    register u8 *sr __asm("$21");
    register u8 *tableA __asm("$23");
    register s32 ga __asm("$16");
    register s32 gb __asm("$17");
    register s32 w __asm("$3");
    register u8 *oldPacket __asm("$5");
    register u8 *otv __asm("$4");
    s16 a3;
    u8 *tableB;

    sr = str;
    next = *(u8 **) 0x1F800000;
    if (*sr != 0) {
        tableA = D_8007D7BC;
        tableB = D_8007D7BD;
        packet = next;
        do {
            idx = *sr++ - 0x20;
            if (idx != 0) {
                ga = tableA[idx * 2];
                gb = tableB[idx * 2];
                func_80064FA8(next);
                func_80064EB8(next, 1);
                next += 0x14;
                oldPacket = packet;
                __asm__ __volatile__("" : : "r"(oldPacket));
                *(volatile s16 *)(packet + 0x8) = x;
                *(volatile s16 *)(packet + 0xA) = y;
                *(volatile u8 *)(packet + 0xC) = ga;
                *(volatile u8 *)(packet + 0xD) = gb;
                w = D_8007D87C[idx];
                *(volatile s16 *)(packet + 0x12) = 0x18;
                a3 = arg3;
                __asm__ __volatile__(
                    "lui %0, %%hi(D_8019C900)\n\t"
                    "lw %0, %%lo(D_8019C900)(%0)\n\t"
                    "sh %2, 0xE(%1)"
                    : "=&r"(otv) : "r"(packet), "r"(a3) : "memory");
                *(volatile s16 *)(packet + 0x10) = w;
                packet += 0x14;
                __asm__ __volatile__("" : "=r"(otv) : "0"(otv));
                func_80064DDC(otv + 0xCC, oldPacket);
            }
            x += D_8007D87C[idx];
        } while (*sr != 0);
    }
    func_800666F4(next, 0, 1, 0x1D, D_8007BED0);
    func_80064DDC(D_8019C900 + 0xCC, next);
    *(u8 **) 0x1F800000 = next + 0xC;
    __asm__ __volatile__("" : : "r"(next));
}
