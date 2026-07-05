#include "common.h"

void SetVertexTri(void *arg0, void *arg1, void *arg2) asm("func_80069938");

void SetVertexTri(void *arg0, void *arg1, void *arg2) {
    asm("lwc2 $0,0($4)");
    asm("lwc2 $1,4($4)");
    asm("lwc2 $2,0($5)");
    asm("lwc2 $3,4($5)");
    asm("lwc2 $4,0($6)");
    asm("lwc2 $5,4($6)");
}
