#include "common.h"

void SetRotMatrix(void *arg0) asm("func_80069858");

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
