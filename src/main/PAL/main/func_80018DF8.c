#include "common.h"

extern s32 D_8007BED8;
extern s32 D_8009E6A4;
extern s16 D_8019CABC;
extern s32 D_8019C754;
extern s32 D_801E4B30;
extern s16 D_801E4DAC;
extern s32 D_801E4D70;
extern s32 D_801E8AB0;
extern s32 D_801F17A8;

s32 func_80017C78(s32, s32);

void func_80018DF8(void) {
    s32 state;
    s32 kind;
    s32 result;

    state = D_8007BED8;
    switch (state) {
    case 1:
        kind = 0x55;
        if (D_801E4DAC != 0) {
            register s32 index asm("$2") = D_8019CABC;
            register s32 scaled asm("$3");
            register s32 base asm("$2");

            __asm__ volatile("");
            scaled = index << 1;
            scaled += index;
            base = D_8009E6A4;
            scaled <<= 1;
            base += 0x4A;
            kind = scaled + base;
        }

        result = func_80017C78(kind, D_801E4B30);
        if (result != 0) {
            D_8007BED8 = 2;
            D_8019C754 = result + D_801E4B30;
        }
        break;
    case 2:
        if (func_80017C78(0x56, D_8019C754) != 0) {
            register s32 ptr asm("$4") = D_8019C754;
            register s32 first asm("$2");
            register s32 second asm("$3");
            register s32 third asm("$2");

            first = *(s32 *)(ptr + 4);
            second = *(s32 *)(ptr + 8);
            first = ptr + first;
            second = ptr + second;
            D_801F17A8 = first;
            D_801E8AB0 = second;
            third = *(s32 *)ptr;
            D_8007BED8 = 0;
            D_801E4D70 = third;
        }
        break;
    }
}
