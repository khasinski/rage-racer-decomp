#include "common.h"

extern volatile u32 *D_80083164;
extern volatile u32 *D_8008316C;
extern volatile u32 *D_80083170;
extern volatile u32 *D_80083178;
extern volatile u32 *D_80083198;
extern u8 D_80013304[];
extern u8 D_8001332C[];
extern u8 D_80013364[];

void func_8001674C(u8 *arg0, ...);

s32 func_80064424(u8 *arg0) {
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
