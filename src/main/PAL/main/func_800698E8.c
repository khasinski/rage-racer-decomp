#include "common.h"

void SetTransMatrix(void *arg0) asm("func_800698E8");

void SetTransMatrix(void *arg0) {
    asm("lw $8,20($4)");
    asm("lw $9,24($4)");
    asm("lw $10,28($4)");
    asm("ctc2 $8,$5");
    asm("ctc2 $9,$6");
    asm("ctc2 $10,$7");
}
