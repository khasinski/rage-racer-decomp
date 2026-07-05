#include "common.h"

extern s32 D_801E40B8;
extern s32 D_801E42A0;
extern s32 D_801E42E0;
extern s32 D_801E428C;
extern s32 D_801E4178;
extern u8 D_80011010[];

void func_80065860(s32 arg0);
void func_80033AA0(s32 arg0, s32 arg1);
void func_80025940(void);
void func_80042BF0(void);
void func_80016EA0(s32 arg0, s32 arg1, void *arg2, s32 arg3);

void func_80026C0C(void) {
    register s32 delta asm("$3");

    if (D_801E40B8 == 2) {
        func_80065860(1);
    }

    delta = D_801E42A0;
    if (delta < 0) {
        register s32 value asm("$2");

        value = D_801E42E0;
        __asm__("addu %0,%1,%2" : "=r"(value) : "0"(value), "r"(delta));
        D_801E42E0 = value;

        if (D_801E42E0 < 0) {
            D_801E42E0 = 0;
            D_801E42A0 = 0;
        }

        func_80033AA0(D_801E42E0, 0x49);
    } else if (delta > 0) {
        register s32 value asm("$4");

        value = D_801E42E0;
        __asm__("addu %0,%1,%2" : "=r"(value) : "0"(value), "r"(delta));
        D_801E42E0 = value;

        func_80033AA0(D_801E42E0, 0x49);

        if (D_801E42E0 >= 0x101) {
            func_80065860(0);
            D_801E428C = 0;
            func_80025940();
            func_80042BF0();
            D_801E4178 = 3;
            D_801E42E0 = 0x100;
            D_801E42A0 = 0;
        }
    }

    func_80016EA0(0x5E, 0x72, D_80011010, 0x7812);
}
