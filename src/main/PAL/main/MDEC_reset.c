#include "common.h"

extern volatile u32 *D_80083164;
extern volatile u32 *D_80083168;
extern volatile u32 *D_8008316C;
extern volatile u32 *D_80083170;
extern volatile u32 *D_80083174;
extern volatile u32 *D_80083178;
extern volatile u32 *D_80083194;
extern volatile u32 *D_80083198;
extern volatile u32 *D_8008319C;
extern u8 D_800132C8[];
extern u8 D_800132E4[];
extern u8 D_800132F4[];
extern u8 D_80013304[];
extern u8 D_8001332C[];
extern u8 D_80013364[];
extern u8 D_8008305C[];
extern u8 D_800830E0[];

s32 MDEC_in_sync(void) asm("func_800642F4");
s32 MDEC_out_sync(void) asm("func_8006438C");
s32 MDEC_timeout(u8 *arg0) asm("func_80064424");
void func_8001674C(u8 *arg0, ...);

void MDEC_in(volatile u32 *arg0, s32 arg1) asm("func_800641D0");

void MDEC_reset(s32 arg0) asm("func_800640D4");
void MDEC_reset(s32 arg0) {
    register s32 option asm("$5") = arg0;
    register s32 zero asm("$0");
    volatile u32 *inBuffer = (volatile u32 *)D_8008305C;

    if (option == 0) {
        goto zero;
    }
    if (option == 1) {
        goto one;
    }
    goto bad;

zero:
    *D_80083198 = 0x80000000;
    *D_8008316C = zero;
    *D_80083178 = zero;
    *D_80083198 = 0x60000000;
    MDEC_in(inBuffer, 0x20);
    MDEC_in((volatile u32 *)D_800830E0, 0x20);
    return;

one:
    *D_80083198 = 0x80000000;
    *D_8008316C = 0;
    *D_80083178 = 0;
    *D_80083178;
    *D_80083198 = 0x60000000;
    return;

bad:
    func_8001674C(D_800132C8);
}

void MDEC_in(volatile u32 *arg0, s32 arg1) {
    MDEC_in_sync();
    *D_8008319C |= 0x88;
    *D_80083164 = (u32)(arg0 + 1);
    *D_80083168 = ((u32)arg1 >> 5 << 16) | 0x20;
    *D_80083194 = *arg0;
    *D_8008316C = 0x01000201;
}

void MDEC_out(volatile u32 *arg0, s32 arg1) asm("func_80064264");
void MDEC_out(volatile u32 *arg0, s32 arg1) {
    MDEC_out_sync();
    *D_8008319C |= 0x88;
    *D_80083178 = 0;
    *D_80083170 = (u32)arg0;
    *D_80083174 = ((u32)arg1 >> 5 << 16) | 0x20;
    *D_80083178 = 0x01000200;
}

s32 MDEC_in_sync(void) {
    volatile s32 timeout;

    timeout = 0x100000;
    if (*D_80083198 & 0x20000000) {
        do {
            if (--timeout == -1) {
                MDEC_timeout(D_800132E4);
                return -1;
            }
        } while (*D_80083198 & 0x20000000);
    }
    return 0;
}

s32 MDEC_out_sync(void) {
    volatile s32 timeout;

    timeout = 0x100000;
    if (*D_80083178 & 0x01000000) {
        do {
            if (--timeout == -1) {
                MDEC_timeout(D_800132F4);
                return -1;
            }
        } while (*D_80083178 & 0x01000000);
    }
    return 0;
}

s32 MDEC_timeout(u8 *arg0) {
    u32 status;
    register s32 ret asm("$2");

    func_8001674C(D_80013364, arg0);
    status = *D_80083198;
    func_8001674C(D_80013304, (*D_8008316C >> 24) & 1, (*D_80083178 >> 24) & 1, *D_80083164, *D_80083170);
    func_8001674C(D_8001332C,
                  (~status >> 31) & 1,
                  (status >> 30) & 1,
                  (status >> 29) & 1,
                  (status >> 28) & 1,
                  (status >> 27) & 1,
                  (status >> 25) & 1,
                  (status >> 23) & 1);

    *D_80083198 = 0x80000000;
    *D_8008316C = 0;
    *D_80083178 = 0;

    asm volatile(
        "addu  $2,$0,$0\n"
        "lui   $3,%%hi(D_80083178)\n"
        "lw    $3,%%lo(D_80083178)($3)\n"
        "lui   $4,%%hi(D_80083198)\n"
        "lw    $4,%%lo(D_80083198)($4)\n"
        "lw    $3,0($3)\n"
        "lui   $3,0x6000\n"
        "sw    $3,0($4)"
        : "=r"(ret)
        :
        : "$3", "$4", "memory");

    return ret;
}
