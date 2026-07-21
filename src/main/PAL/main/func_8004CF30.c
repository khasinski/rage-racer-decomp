#include "common.h"

extern s32 D_8007FB20;

void func_800468FC(void *ot, s32 x, s32 y, s32 w, s32 h);
void func_80047330(void *ot, s32 x0, s32 y0, s32 x1, u16 y1, u32 r0, u32 g0, u32 b0, u8 r1, u8 g1, u8 b1, u8 alpha);
void func_80047024(void *ot, s32 x0, s32 y0, s32 x1, u16 y1, u32 r, u32 g, u32 b, u8 alpha);
void func_80064F58(u8 *prim);
void func_80064E90(u8 *prim, s32 arg1);
void func_80064DDC(void *ot, void *prim);

typedef struct {
    s16 b[33];
} Blob;

extern Blob D_80011898;
extern Blob D_800118DC;

void func_8004CF30(s32 arg) {
    void *s3;
    Blob l1;
    Blob l2;

    s3 = (void *)(*(s32 *)0x1F800004 + 0xAFC);
    l1 = D_80011898;
    l2 = D_800118DC;

    if (arg == 0) {
        D_8007FB20 = 0;
        return;
    }
    if (arg < 0) {
        D_8007FB20 += arg;
        if (D_8007FB20 < 0) {
            D_8007FB20 = 0;
        }
    }
    if (D_8007FB20 > 0) {
        s32 s0;
        s32 s1;
        s32 s2;
        u8 *prim;

        func_800468FC(s3, 0, 0, 0x140, 0x1E0);

        s0 = 0;
        s2 = 0;
        s1 = 0x00300000;
        do {
            register s32 cnt asm("$3") = D_8007FB20;
            register s32 m11 asm("$2") = cnt * 11;
            register u32 u0 asm("$8");
            register u8 c1 asm("$3");
            register u32 c0 asm("$2");
            __asm__("" : "=r"(m11) : "0"(m11));
            u0 = (u32)m11 >> 8;
            __asm__("" : "=r"(u0) : "0"(u0));
            c1 = (u32)(cnt * 75) >> 8;
            __asm__("andi %0, %1, 0xFF" : "=r"(c0) : "r"(u0));
            func_80047330(s3, s1 >> 16, 0xAA, s2 >> 16, 0x1E0, c0, c0, c0, c1, c1, c1, 0x60);
            s2 += 0x000A0000;
            s1 += 0x00070000;
            s0++;
        } while (s0 < 0x21);

        s0 = 0;
        do {
            s32 x0 = l1.b[s0];
            s32 y0 = l2.b[s0];
            s16 x1 = (0xA0 - (u16)l1.b[s0]) * 2;
            s32 v = (((s32)((u16)l2.b[s0] - 0xAA) << 7) / 309 + 0x16) * D_8007FB20;
            register s32 vv asm("$2") = v;
            register u32 sh asm("$8");
            register u32 col asm("$2");
            __asm__("" : "=r"(vv) : "0"(vv));
            sh = (u32)vv >> 9;
            __asm__("" : "=r"(sh) : "0"(sh));
            __asm__("andi %0, %1, 0xFF" : "=r"(col) : "r"(sh));
            func_80047024(s3, x0, y0, x1, 2, col, col, col, 0x60);
            s0++;
        } while (s0 < 0x21);

        prim = *(u8 **)0x1F800000;
        func_80064F58(prim);
        func_80064E90(prim, 0);
        {
            register s32 x asm("$3") = D_8007FB20;
            register s32 q0 asm("$2") = x / 5 + (x >> 31);
            register u8 col asm("$8");
            __asm__("" : "=r"(q0) : "0"(q0));
            col = q0 - (x >> 31);
            *(s16 *)(prim + 0x20) = 0x13F;
            *(s16 *)(prim + 0x10) = 0x13F;
            *(s16 *)(prim + 0x12) = 0x28;
            *(s16 *)(prim + 0x0A) = 0x28;
            *(s16 *)(prim + 0x18) = 0;
            *(s16 *)(prim + 0x08) = 0;
            *(s16 *)(prim + 0x22) = 0x1DF;
            *(s16 *)(prim + 0x1A) = 0x1DF;
            prim[0x0C] = 0;
            prim[0x04] = 0;
            prim[0x0D] = 0;
            prim[0x05] = 0;
            prim[0x0E] = 0;
            prim[0x06] = 0;
            prim[0x1C] = col;
            prim[0x14] = col;
            prim[0x1D] = col;
            prim[0x15] = col;
            prim[0x1E] = col;
            prim[0x16] = col;
        }
        {
            u8 *oldPrim = prim;
            prim += 0x24;
            func_80064DDC(s3, oldPrim);
        }
        *(u8 **)0x1F800000 = prim;
        func_800468FC(s3, 0x48, 0, 0x140, 0x1E0);
    }
    if (arg > 0) {
        D_8007FB20 += arg;
        if (D_8007FB20 >= 0x201) {
            D_8007FB20 = 0x200;
        }
    }
}
