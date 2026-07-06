#include "common.h"

void func_80069858(s32 *matrix) {
    register s32 r8 asm("$8") = matrix[0];
    register s32 r9 asm("$9") = matrix[1];
    register s32 r10 asm("$10") = matrix[2];
    register s32 r11 asm("$11") = matrix[3];
    register s32 r12 asm("$12") = matrix[4];

    asm volatile(
        "ctc2 $8,$0\n"
        "ctc2 $9,$1\n"
        "ctc2 $10,$2\n"
        "ctc2 $11,$3\n"
        "ctc2 $12,$4"
        :
        : "r"(r8), "r"(r9), "r"(r10), "r"(r11), "r"(r12));
}

void func_80069888(s32 *matrix) {
    register s32 r8 asm("$8") = matrix[0];
    register s32 r9 asm("$9") = matrix[1];
    register s32 r10 asm("$10") = matrix[2];
    register s32 r11 asm("$11") = matrix[3];
    register s32 r12 asm("$12") = matrix[4];

    asm volatile(
        "ctc2 $8,$8\n"
        "ctc2 $9,$9\n"
        "ctc2 $10,$10\n"
        "ctc2 $11,$11\n"
        "ctc2 $12,$12"
        :
        : "r"(r8), "r"(r9), "r"(r10), "r"(r11), "r"(r12));
}

void func_800698B8(s32 *matrix) {
    register s32 r8 asm("$8") = matrix[0];
    register s32 r9 asm("$9") = matrix[1];
    register s32 r10 asm("$10") = matrix[2];
    register s32 r11 asm("$11") = matrix[3];
    register s32 r12 asm("$12") = matrix[4];

    asm volatile(
        "ctc2 $8,$16\n"
        "ctc2 $9,$17\n"
        "ctc2 $10,$18\n"
        "ctc2 $11,$19\n"
        "ctc2 $12,$20"
        :
        : "r"(r8), "r"(r9), "r"(r10), "r"(r11), "r"(r12));
}

void func_800698E8(s32 *matrix) {
    register s32 r8 asm("$8") = matrix[5];
    register s32 r9 asm("$9") = matrix[6];
    register s32 r10 asm("$10") = matrix[7];

    asm volatile(
        "ctc2 $8,$5\n"
        "ctc2 $9,$6\n"
        "ctc2 $10,$7"
        :
        : "r"(r8), "r"(r9), "r"(r10));
}

void func_80069908(void *vec) {
    asm volatile(
        "lwc2 $0,0($4)\n"
        "lwc2 $1,4($4)"
        :
        : "r"(vec)
        : "memory");
}

void func_80069918(void *vec) {
    asm volatile(
        "lwc2 $2,0($4)\n"
        "lwc2 $3,4($4)"
        :
        : "r"(vec)
        : "memory");
}

void func_80069928(void *vec) {
    asm volatile(
        "lwc2 $4,0($4)\n"
        "lwc2 $5,4($4)"
        :
        : "r"(vec)
        : "memory");
}

void func_80069938(void *vec0, void *vec1, void *vec2) {
    asm volatile(
        "lwc2 $0,0($4)\n"
        "lwc2 $1,4($4)\n"
        "lwc2 $2,0($5)\n"
        "lwc2 $3,4($5)\n"
        "lwc2 $4,0($6)\n"
        "lwc2 $5,4($6)"
        :
        : "r"(vec0), "r"(vec1), "r"(vec2)
        : "memory");
}

void func_80069958(void *arg0, void *arg1, void *arg2) {
    asm volatile(
        "lwc2 $20,0($4)\n"
        "lwc2 $21,0($5)\n"
        "lwc2 $22,0($6)"
        :
        : "r"(arg0), "r"(arg1), "r"(arg2)
        : "memory");
}

void func_8006996C(s32 arg0, s32 arg1, s32 arg2) {
    asm volatile(
        "mtc2 $4,$9\n"
        "mtc2 $5,$10\n"
        "mtc2 $6,$11"
        :
        : "r"(arg0), "r"(arg1), "r"(arg2));
}

void func_80069980(s32 arg0) {
    asm volatile(
        "mtc2 $4,$8"
        :
        : "r"(arg0));
}

void func_8006998C(s32 arg0, s32 arg1, s32 arg2) {
    asm volatile(
        "mtc2 $4,$17\n"
        "mtc2 $5,$18\n"
        "mtc2 $6,$19"
        :
        : "r"(arg0), "r"(arg1), "r"(arg2));
}

void func_800699A0(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    asm volatile(
        "mtc2 $4,$16\n"
        "mtc2 $5,$17\n"
        "mtc2 $6,$18\n"
        "mtc2 $7,$19"
        :
        : "r"(arg0), "r"(arg1), "r"(arg2), "r"(arg3));
}

void func_800699B8(s32 arg0, s32 arg1, s32 arg2) {
    asm volatile(
        "mtc2 $4,$12\n"
        "mtc2 $5,$13\n"
        "mtc2 $6,$14"
        :
        : "r"(arg0), "r"(arg1), "r"(arg2));
}

void func_800699CC(s32 arg0, s32 arg1, s32 arg2) {
    asm volatile(
        "ctc2 $4,$0\n"
        "ctc2 $5,$2\n"
        "ctc2 $6,$4"
        :
        : "r"(arg0), "r"(arg1), "r"(arg2));
}

void func_800699E0(s32 arg0, s32 arg1, s32 arg2) {
    asm volatile(
        "mtc2 $4,$25\n"
        "mtc2 $5,$26\n"
        "mtc2 $6,$27"
        :
        : "r"(arg0), "r"(arg1), "r"(arg2));
}

void func_800699F4(s32 arg0) {
    asm volatile(
        "mtc2 $4,$30"
        :
        : "r"(arg0));
}

void func_80069A00(s32 arg0) {
    asm volatile(
        "ctc2 $4,$27"
        :
        : "r"(arg0));
}

void func_80069A0C(s32 arg0) {
    asm volatile(
        "ctc2 $4,$28"
        :
        : "r"(arg0));
}

void func_80069A18(s32 arg0, s32 arg1, s32 arg2) {
    register s32 r4 asm("$4") = arg0 << 4;
    register s32 r5 asm("$5") = arg1 << 4;
    register s32 r6 asm("$6") = arg2 << 4;

    asm volatile(
        "ctc2 $4,$13\n"
        "ctc2 $5,$14\n"
        "ctc2 $6,$15"
        :
        : "r"(r4), "r"(r5), "r"(r6));
}

void func_80069A38(s32 arg0, s32 arg1, s32 arg2) {
    register s32 r4 asm("$4") = arg0 << 4;
    register s32 r5 asm("$5") = arg1 << 4;
    register s32 r6 asm("$6") = arg2 << 4;

    asm volatile(
        "ctc2 $4,$21\n"
        "ctc2 $5,$22\n"
        "ctc2 $6,$23"
        :
        : "r"(r4), "r"(r5), "r"(r6));
}

void func_80069A58(s32 arg0, s32 arg1) {
    register s32 r4 asm("$4") = arg0 << 16;
    register s32 r5 asm("$5") = arg1 << 16;

    asm volatile(
        "ctc2 $4,$24\n"
        "ctc2 $5,$25"
        :
        : "r"(r4), "r"(r5));
}

asm(".globl func_80069A70\n"
    "func_80069A70:\n"
    "nop\n"
    "nop");

void func_80069A78(s32 arg0) {
    asm volatile(
        "ctc2 $4,$26"
        :
        : "r"(arg0));
}

asm(".globl func_80069A84\n"
    "func_80069A84:\n"
    "nop");

void func_80069A88(void *in, void *out) {
    asm volatile(
        "lwc2 $9,0($4)\n"
        "lwc2 $10,4($4)\n"
        "lwc2 $11,8($4)\n"
        "nop\n"
        "cop2 0x4da412\n"
        "swc2 $9,0($5)\n"
        "swc2 $10,4($5)\n"
        "swc2 $11,8($5)"
        :
        : "r"(in), "r"(out)
        : "memory");
}

void func_80069AB0(void *arg0, void *arg1, s32 arg2, void *arg3) {
    asm volatile(
        "lwc2 $9,0($4)\n"
        "lwc2 $10,4($4)\n"
        "lwc2 $11,8($4)\n"
        "lwc2 $6,0($5)\n"
        "mtc2 $6,$8\n"
        "nop\n"
        "cop2 0x680029\n"
        "swc2 $22,0($7)"
        :
        : "r"(arg0), "r"(arg1), "r"(arg2), "r"(arg3)
        : "memory");
}

void func_80069AD8(void *arg0, void *arg1, void *arg2, s32 arg3, void *out0, void *out1, void *out2) {
    (void)out0;
    (void)out1;
    (void)out2;

    asm volatile(
        "lwc2 $20,0($4)\n"
        "lwc2 $21,0($5)\n"
        "lwc2 $22,0($6)\n"
        "lwc2 $6,0($6)\n"
        "mtc2 $7,$8\n"
        "nop\n"
        "cop2 0xf8002a\n"
        "lw $8,16($sp)\n"
        "lw $9,20($sp)\n"
        "lw $10,24($sp)\n"
        "swc2 $20,0($8)\n"
        "swc2 $21,0($9)\n"
        "swc2 $22,0($10)"
        :
        : "r"(arg0), "r"(arg1), "r"(arg2), "r"(arg3)
        : "memory");
}

void func_80069B14(void *arg0, s32 arg1, void *arg2) {
    asm volatile(
        "lwc2 $9,0($4)\n"
        "lwc2 $10,4($4)\n"
        "lwc2 $11,8($4)\n"
        "mtc2 $5,$8\n"
        "nop\n"
        "cop2 0x980011\n"
        "swc2 $22,0($6)"
        :
        : "r"(arg0), "r"(arg1), "r"(arg2)
        : "memory");
}

asm(".globl func_80069B38\n"
    "func_80069B38:\n"
    "lwc2 $9,0($4)\n"
    "lwc2 $10,4($4)\n"
    "lwc2 $11,8($4)\n"
    "nop\n"
    ".word 0x4AA80428\n"
    "swc2 $25,0($5)\n"
    "swc2 $26,4($5)\n"
    "swc2 $27,8($5)\n"
    "jr $31\n"
    "move $2,$5");

asm(".globl func_80069B60\n"
    "func_80069B60:\n"
    "lwc2 $9,0($4)\n"
    "lwc2 $10,4($4)\n"
    "lwc2 $11,8($4)\n"
    "nop\n"
    ".word 0x4AA00428\n"
    "swc2 $25,0($5)\n"
    "swc2 $26,4($5)\n"
    "swc2 $27,8($5)\n"
    "jr $31\n"
    "move $2,$5");

s32 func_80069B88(s32 arg0, s32 arg1, s32 arg2) {
    register s32 ret asm("$2");

    asm volatile(
        "mtc2 $4,$17\n"
        "mtc2 $5,$18\n"
        "mtc2 $6,$19\n"
        "nop\n"
        "cop2 0x158002d\n"
        "mfc2 $2,$7"
        : "=r"(ret)
        : "r"(arg0), "r"(arg1), "r"(arg2));

    return ret;
}

s32 func_80069BA8(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    register s32 ret asm("$2");

    asm volatile(
        "mtc2 $4,$16\n"
        "mtc2 $5,$17\n"
        "mtc2 $6,$18\n"
        "mtc2 $7,$19\n"
        "nop\n"
        "cop2 0x168002e\n"
        "mfc2 $2,$7"
        : "=r"(ret)
        : "r"(arg0), "r"(arg1), "r"(arg2), "r"(arg3));

    return ret;
}

void func_80069BCC(void *matrix, void *in, void *out) {
    asm volatile(
        "cfc2 $13,$0\n"
        "cfc2 $14,$2\n"
        "cfc2 $15,$4\n"
        "lw $8,0($4)\n"
        "lw $9,4($4)\n"
        "lw $10,8($4)\n"
        "ctc2 $8,$0\n"
        "ctc2 $9,$2\n"
        "ctc2 $10,$4\n"
        "lwc2 $11,8($5)\n"
        "lwc2 $9,0($5)\n"
        "lwc2 $10,4($5)\n"
        "nop\n"
        "cop2 0x178000c\n"
        "swc2 $25,0($6)\n"
        "swc2 $26,4($6)\n"
        "swc2 $27,8($6)\n"
        "ctc2 $13,$0\n"
        "ctc2 $14,$2\n"
        "ctc2 $15,$4"
        :
        : "r"(matrix), "r"(in), "r"(out)
        : "memory");
}

void func_80069C24(void *matrix, void *in, void *out) {
    asm volatile(
        "cfc2 $13,$0\n"
        "cfc2 $14,$2\n"
        "cfc2 $15,$4\n"
        "lw $8,0($4)\n"
        "lw $9,4($4)\n"
        "lw $10,8($4)\n"
        "ctc2 $8,$0\n"
        "ctc2 $9,$2\n"
        "ctc2 $10,$4\n"
        "lwc2 $11,8($5)\n"
        "lwc2 $9,0($5)\n"
        "lwc2 $10,4($5)\n"
        "nop\n"
        "cop2 0x170000c\n"
        "swc2 $25,0($6)\n"
        "swc2 $26,4($6)\n"
        "swc2 $27,8($6)\n"
        "ctc2 $13,$0\n"
        "ctc2 $14,$2\n"
        "ctc2 $15,$4"
        :
        : "r"(matrix), "r"(in), "r"(out)
        : "memory");
}
