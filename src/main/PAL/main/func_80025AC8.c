#include "common.h"
extern s32 D_801E40B8, D_801E42A0, D_801E42E0, D_8019C99C;
extern u8 D_80011010[];
void func_80065860(s32 arg0);
void func_80033AA0(s32 arg0, s32 arg1);
void func_80025940(void);
void func_80016EA0(s32 a0, s32 a1, void *a2, s32 a3);
void func_80025AC8(void) {
    s32 d;
    s32 v;
    if (D_801E40B8 == 0xF) {
        func_80065860(1);
    }
    d = D_801E42A0;
    if (d < 0) {
        s32 e = D_801E42E0;
        __asm__("addu %0,%1,%2" : "=r"(e) : "0"(e), "r"(d));
        D_801E42E0 = e;
        if (D_801E42E0 < 0) {
            D_801E42E0 = 0;
            D_801E42A0 = 0;
        }
        func_80033AA0(D_801E42E0, 0x49);
    } else if (d > 0) {
        s32 e = D_801E42E0;
        __asm__("addu %0,%1,%2" : "=r"(e) : "0"(e), "r"(d));
        v = e;
        D_801E42E0 = v;
        func_80033AA0(v, 0x49);
        if (D_801E42E0 >= 257) {
            func_80065860(0);
            func_80025940();
            D_801E42A0 = 0;
            D_801E42E0 = 0;
            D_8019C99C = 2;
        }
    }
    func_80016EA0(0x5E, 0x72, D_80011010, 0x7812);
}
