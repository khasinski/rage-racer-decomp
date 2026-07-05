#include "common.h"
#include "psyq/cd.h"

extern u8 D_80099318;
extern volatile u8 D_8009931A;
extern u8 *D_80099300;
extern u32 *D_80099310;
extern u8 *D_80099308;
extern u8 *D_8009930C;

void CD_flush(void) {
    register u8 *v1_p asm("$3");
    register u32 v0 asm("$2");
    register u32 v1 asm("$3");
    register u32 a0 asm("$4");

    *D_80099300 = 1;

    if ((*D_8009930C & 7) != 0) {
        v1 = 7;
        a0 = 1;
        do {
            *D_80099300 = a0;
            *D_8009930C = v1;
            *D_80099308 = v1;
        } while ((*D_8009930C & 7) != 0);
    }

    D_8009931A = 0;
    asm("lui $3,%hi(D_8009931A)");
    asm("lbu $3,%lo(D_8009931A)($3)");
    asm("lui $2,%hi(D_80099319)");
    asm("addiu $2,$2,%lo(D_80099319)");
    asm("sb $3,0($2)");
    v1_p = D_80099300;
    v0 = 2;
    D_80099318 = v0;
    *v1_p = 0;
    *D_8009930C = 0;
    asm("lui $3,%hi(D_80099310)");
    asm("lw $3,%lo(D_80099310)($3)");
    asm("ori $2,$0,0x1325");
    asm("sw $2,0($3)");
}
