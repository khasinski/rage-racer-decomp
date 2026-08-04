#include "common.h"
#include "psyq/cd.h"

void func_8006BA68(void);
long func_8006C06C(void);
void DMACallback(long arg0, long arg1) asm("func_8006DF94");
void CD_datasync(void) asm("func_8006BF00");

long CdMix(void) asm("func_8006A94C");
long CdMix(void) {
    func_8006BA68();
    return 1;
}

long CdGetSector2(void) asm("func_8006A970");
long CdGetSector2(void) {
    return func_8006C06C() == 0;
}

void CdDataCallback(long arg0) asm("func_8006A994");
void CdDataCallback(long arg0) {
    DMACallback(3, arg0);
}

void func_8006A9B8(void) {
    CD_datasync();
}

CdlLOC *CdIntToPos(long i, CdlLOC *p) {
    inline long ENCODE_BCD(n) { return (((n / 10) << 4) + (n % 10)); };

    i += 150;
    p->sector = ENCODE_BCD(i % 75);
    p->second = ENCODE_BCD(i / 75 % 60);
    p->minute = ENCODE_BCD(i / 75 / 60);
    return p;
}

long CdPosToInt_Local(CdlLOC *arg0) {
    long v0;
    long v1 = arg0->minute;
    register long a0 asm("$4");
    long a1;
    register long a2 asm("$6") = arg0->second;

    a1 = (u_long) v1 / 16;
    v0 = a1 * 4;
    v0 += a1;
    v0 <<= 1;
    v1 &= 0xF;
    v0 += v1;
    a1 = v0 << 4;
    a1 -= v0;
    a1 <<= 2;

    v1 = (u_long) a2 / 16;
    v0 = v1 * 4;
    v0 += v1;
    v0 <<= 1;
    a2 &= 0xF;
    v0 += a2;
    a1 += v0;
    v1 = a1 << 2;
    v1 += a1;
    v0 = v1 * 16;

    a1 = arg0->sector;
    v0 -= v1;
    a0 = (u_long) a1 / 16;
    v1 = a0 << 2;
    v1 += a0;
    v1 <<= 1;
    a1 &= 0xF;
    v1 += a1;
    v0 += v1;

    return v0 - 150;
}
