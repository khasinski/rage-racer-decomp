#include "common.h"

void SetRotMatrix(void *arg0) asm("func_80069858");
void SetLightMatrix(void *arg0) asm("func_80069888");
void SetColorMatrix(void *arg0) asm("func_800698B8");
void SetTransMatrix(void *arg0) asm("func_800698E8");
void SetVertexTri(void *arg0, void *arg1, void *arg2) asm("func_80069938");

void SetRotMatrix(void *arg0) {
    asm("lw $8,0($4)");
    asm("lw $9,4($4)");
    asm("lw $10,8($4)");
    asm("lw $11,12($4)");
    asm("lw $12,16($4)");
    asm("ctc2 $8,$0");
    asm("ctc2 $9,$1");
    asm("ctc2 $10,$2");
    asm("ctc2 $11,$3");
    asm("ctc2 $12,$4");
}

void SetLightMatrix(void *arg0) {
    asm("lw $8,0($4)");
    asm("lw $9,4($4)");
    asm("lw $10,8($4)");
    asm("lw $11,12($4)");
    asm("lw $12,16($4)");
    asm("ctc2 $8,$8");
    asm("ctc2 $9,$9");
    asm("ctc2 $10,$10");
    asm("ctc2 $11,$11");
    asm("ctc2 $12,$12");
}

void SetColorMatrix(void *arg0) {
    asm("lw $8,0($4)");
    asm("lw $9,4($4)");
    asm("lw $10,8($4)");
    asm("lw $11,12($4)");
    asm("lw $12,16($4)");
    asm("ctc2 $8,$16");
    asm("ctc2 $9,$17");
    asm("ctc2 $10,$18");
    asm("ctc2 $11,$19");
    asm("ctc2 $12,$20");
}

void SetTransMatrix(void *arg0) {
    asm("lw $8,20($4)");
    asm("lw $9,24($4)");
    asm("lw $10,28($4)");
    asm("ctc2 $8,$5");
    asm("ctc2 $9,$6");
    asm("ctc2 $10,$7");
}

void func_80069908(void *arg0) {
    asm("lwc2 $0,0($4)");
    asm("lwc2 $1,4($4)");
}

void func_80069918(void *arg0) {
    asm("lwc2 $2,0($4)");
    asm("lwc2 $3,4($4)");
}

void func_80069928(void *arg0) {
    asm("lwc2 $4,0($4)");
    asm("lwc2 $5,4($4)");
}

void SetVertexTri(void *arg0, void *arg1, void *arg2) {
    asm("lwc2 $0,0($4)");
    asm("lwc2 $1,4($4)");
    asm("lwc2 $2,0($5)");
    asm("lwc2 $3,4($5)");
    asm("lwc2 $4,0($6)");
    asm("lwc2 $5,4($6)");
}
