#include "common.h"

typedef struct {
    u32 unk0;
    u32 unk4;
} UnkCopyChunk;

extern u8 D_8007BE68;

void func_80021288(void *arg0, s32 *arg1) {
    register s32 *out asm("t1") = arg1;

    asm volatile("" : "=r"(out) : "0"(out));
    asm volatile(
        "move  $t0,$zero\n"
        "move  $a3,%0\n"
        "lui   $a2,%%hi(D_8007BE68)\n"
        "addiu $a2,$a2,%%lo(D_8007BE68)\n"
        "1:\n"
        "lwl   $v0,3($a2)\n"
        "lwr   $v0,0($a2)\n"
        "lwl   $v1,7($a2)\n"
        "lwr   $v1,4($a2)\n"
        "swl   $v0,3($a3)\n"
        "swr   $v0,0($a3)\n"
        "swl   $v1,7($a3)\n"
        "swr   $v1,4($a3)\n"
        "addiu $a3,$a3,8\n"
        "addiu $t0,$t0,1\n"
        "slti  $v0,$t0,13\n"
        "bnez  $v0,1b\n"
        "addiu $a2,$a2,8\n"
        :
        : "r"(arg0)
        : "$2", "$3", "$6", "$7", "$8", "memory");

    out[1] = 3;
    out[0] = 0;
    out[2] = 0;
    out[3] = -1;
    out[4] = 0;
}
