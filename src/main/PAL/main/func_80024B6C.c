#include "common.h"

extern s32 D_8019C868;
extern u8 *D_8019C900;
extern s32 D_801E6C70;
extern s32 D_8019C704;
extern s32 D_801E8A50;
extern s32 D_8019CB14;

void func_800236C8(s32 arg0, s32 arg1);
s32 func_80017138(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
void func_80023750(s32 arg0);
s32 func_8001705C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9);
s32 func_80032F34(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
void func_800249A4(s32 arg0, s32 arg1);

void func_80024B6C(void) {
    s32 base;
    register s32 *scratch asm("$22");
    register s32 color asm("$18");
    register s32 s0 asm("$16");
    register s32 s3 asm("$19");
    s32 n;


    {
        s32 a1v = D_8019C868;
        asm("" : : "r"(a1v));
        {
            register s32 a0v asm("$4") = 0x14;
            asm("" : : "r"(a0v));
            color = 0x7F;
            func_800236C8(a0v, (a1v << 5) + 56);
        }
    }

    scratch = (s32 *)0x1F800000;
    s3 = 0x18;
    s0 = 0x78;
    base = (s32)D_8019C900 + 0xCC;

    n = *scratch;
    n = func_80017138(base, n, 0x24, 0x38, 0x2C, s3, 0x9C, s0, 0x7F40);
    n = func_80017138(base, n, 0x24, 0x58, s3, s3, 0xC8, s0, 0x7F40);
    n = func_80017138(base, n, 0x24, 0x78, 0x38, s3, 0, 0x90, 0x7F40);
    n = func_80017138(base, n, 0x24, 0x98, 0x1C, s3, 0xD0, 0x60, 0x7F40);
    *scratch = n;

    func_80023750(2);

    n = *scratch;
    if (D_801E6C70 != 0) {
        color = 0x20;
    }

    n = func_8001705C(base, n, 0x66, 0x12A, s3, 0xC, 0xD4, 0xC4, 0x7F40, color);
    n = func_80032F34(base, n, 0x47, 0x122, 0x56, 0x1C, 0x85, 0x15, 0xE);
    n = func_80032F34(base, n, 0x46, 0x120, 0x58, 0x20, color << 1, color << 1, color << 1);
    asm("" : : "r"(color));

    color = 0x20;
    if (D_801E6C70 != 0) {
        color = 0x7F;
    }

    n = func_8001705C(base, n, 0xBA, 0x12A, 0x28, 0xC, 0xB4, 0xD0, 0x7F40, color);
    n = func_80032F34(base, n, 0xA3, 0x122, 0x56, 0x1C, 0x85, 0x15, 0xE);
    n = func_80032F34(base, n, 0xA2, 0x120, 0x58, 0x20, color << 1, color << 1, color << 1);
    asm("" : : "r"(color));
    {
        s32 a0v = D_8019C704;
        *scratch = n;
        func_800249A4(a0v, 0xD0);
    }
    func_800249A4(D_801E8A50, 0xF8);

    if (D_8019CB14 != 5) {
        return;
    }

    n = *scratch;
    switch (D_8019C868) {
    case 0:
        n = func_80032F34(base, n, 0x44, 0xCC, 0xB8, 0x28, 0x89, 0xFF, 0x76);
        break;
    case 1:
        n = func_80032F34(base, n, 0x44, 0xF4, 0xB8, 0x28, 0x89, 0xFF, 0x76);
        break;
    case 2:
        n = func_80032F34(base, n, (D_801E6C70 != 0) ? 0xA0 : 0x44, 0x11C, 0x5C, 0x28, 0x89, 0xFF, 0x76);
        break;
    }
    *(s32 *)0x1F800000 = n;
}
