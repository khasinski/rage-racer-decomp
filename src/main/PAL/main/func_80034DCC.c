#include "common.h"
#include "game/track.h"
#include "game/car.h"

extern u8 D_80011438[];

void func_8001674C(u8 *arg0);


extern u8 D_80011440[];

extern u8 *volatile D_8019CAF8;

extern u8 *volatile D_801E4128;


extern u8 *volatile D_801E4174;

extern u8 *volatile D_801E4448;

extern u8 *volatile D_801E4BA4;

extern u8 D_8001144C;

extern u8 D_8001145C;

s32 func_8005B5C4(void);

extern u8 D_8001146C;

void func_8005BB1C(void);

void func_8005B468(s32 arg0);

void func_8005B204(s32 arg0, s32 arg1, s32 arg2);

void func_80034DCC(void) {
    func_8001674C(D_80011438);
}

void func_80034DF4(u32 arg0) {
    g_CarSpec = (GameCarSpec *)arg0;
}

void func_80034E04(u8 *arg0) {
    register s32 offset0 asm("$2");
    register s32 offset1 asm("$3");
    register u8 *callArg asm("$4");
    register u8 *base asm("$5");

    offset0 = *(s32 *)(arg0 + 0xB78);
    offset1 = *(s32 *)(arg0 + 0xB68);
    base = arg0 + 0xB64;
    g_TrackEventData = arg0;
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

void func_80034E88(void) {
    if (func_8005B5C4() != 0) {
        func_8001674C(&D_8001144C);
    }
    func_8001674C(&D_8001145C);
}

void func_80034ED0(void) {
    func_8005BB1C();
    func_8005B468(1);
    func_8005B204(2, 0, 0);
    func_8001674C(&D_8001146C);
}

s32 func_80034F18(s32 arg0, s32 arg1) {
    s32 quotient;
    s32 quotientPart;

    quotient = arg0 / 25;
    arg0 -= quotient * 25;
    arg0 = (arg0 * 40) + arg1;
    quotientPart = quotient * 1000;
    return quotientPart + arg0;
}
