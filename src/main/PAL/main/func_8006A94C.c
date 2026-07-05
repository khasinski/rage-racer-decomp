#include "common.h"
#include "psyq/cd.h"

void func_8006BA68(void);
s32 func_8006C06C(void);
void func_8006DF94(s32 arg0, s32 arg1);
void func_8006BF00(void);

s32 func_8006A94C(void) {
    func_8006BA68();
    return 1;
}

s32 func_8006A970(void) {
    return func_8006C06C() == 0;
}

void func_8006A994(s32 arg0) {
    func_8006DF94(3, arg0);
}

void func_8006A9B8(void) {
    func_8006BF00();
}

CdlLOC *CdIntToPos(s32 i, CdlLOC *p) {
    inline int ENCODE_BCD(n) { return (((n / 10) << 4) + (n % 10)); };

    i += 150;
    p->sector = ENCODE_BCD(i % 75);
    p->second = ENCODE_BCD(i / 75 % 60);
    p->minute = ENCODE_BCD(i / 75 / 60);
    return p;
}

s32 CdPosToInt_Local(CdlLOC *arg0) {
    register s32 v0 asm("$2");
    register s32 v1 asm("$3") = arg0->minute;
    register s32 a0 asm("$4");
    register s32 a1 asm("$5");
    register s32 a2 asm("$6") = arg0->second;

    a1 = (u32) v1 >> 4;
    v0 = a1 << 2;
    v0 += a1;
    v0 <<= 1;
    v1 &= 0xF;
    v0 += v1;
    a1 = v0 << 4;
    a1 -= v0;
    a1 <<= 2;

    v1 = (u32) a2 >> 4;
    v0 = v1 << 2;
    v0 += v1;
    v0 <<= 1;
    a2 &= 0xF;
    v0 += a2;
    a1 += v0;
    v1 = a1 << 2;
    v1 += a1;
    v0 = v1 << 4;

    a1 = arg0->sector;
    v0 -= v1;
    a0 = (u32) a1 >> 4;
    v1 = a0 << 2;
    v1 += a0;
    v1 <<= 1;
    a1 &= 0xF;
    v1 += a1;
    v0 += v1;

    return v0 - 150;
}
