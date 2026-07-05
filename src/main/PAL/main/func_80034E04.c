#include "common.h"

extern u8 D_80011440[];
extern u8 *volatile D_8019CAF8;
extern u8 *volatile D_801E4128;
extern u8 *volatile D_801E4150;
extern u8 *volatile D_801E4174;
extern u8 *volatile D_801E4448;
extern u8 *volatile D_801E4BA4;

void func_8001674C(u8 *arg0);

void func_80034E04(u8 *arg0) {
    register s32 offset0 asm("$2");
    register s32 offset1 asm("$3");
    register u8 *callArg asm("$4");
    register u8 *base asm("$5");

    offset0 = *(s32 *)(arg0 + 0xB78);
    offset1 = *(s32 *)(arg0 + 0xB68);
    base = arg0 + 0xB64;
    D_801E4150 = arg0;
    D_801E4448 = base + offset0;
    offset0 = *(s32 *)(arg0 + 0xB64);
    D_8019CAF8 = base + offset1;
    offset1 = *(s32 *)(arg0 + 0xB70);
    D_801E4128 = base + offset0;
    offset0 = *(s32 *)(arg0 + 0xB6C);
    callArg = D_80011440;
    offset0 = (s32)(base + offset0);
    base += offset1;
    D_801E4BA4 = (u8 *)offset0;
    D_801E4174 = base;
    func_8001674C(callArg);
}
