#include "common.h"
#include "psyq/gpu.h"
#include "psyq/kernel.h"

extern char D_80013478[];
extern char D_80013498[];
extern u8 D_800941A0[];
extern GpuCallbacks *D_800941E0;
extern void (*D_800941E4)(char *, ...);
extern u8 D_800941E8[];
extern u8 D_800941E9;
extern u8 D_800941EA;
extern u16 D_800941EC;
extern u16 D_800941EE;
extern u8 D_80094268[];
extern u8 D_8009427C[];

void func_8001674C(char *fmt, ...);
void func_80068180(u8 *dst, s32 value, s32 count);
void func_800681AC(void *arg0);
s32 func_80067C80(s32 arg0);

void func_80065460(s32 mode) {
    register s32 maskedMode asm("$17");
    register u8 *graphState asm("$16");
    register s32 graphType asm("$2");
    register u8 *clearEnv asm("$4");
    register s32 fillValue asm("$5");

    maskedMode = mode & 7;
    if ((maskedMode == 0) || (maskedMode == 3)) {
        graphState = D_800941E8;
        func_8001674C(D_80013478, D_800941A0, graphState);
        func_80068180(graphState, 0, 0x80);
        KernelCallbackSlot3();
        func_800681AC((void *)((u32)D_800941E0 & 0xFFFFFF));
        graphType = func_80067C80(maskedMode != 0);
        clearEnv = graphState + 0x10;
        asm("" : "=r"(clearEnv) : "0"(clearEnv));
        asm volatile(
            ".set noat\n\t"
            "sb %2,0(%1)\n\t"
            "lbu $2,0(%1)\n\t"
            "li $3,1\n\t"
            "lui $1,%%hi(D_800941E9)\n\t"
            "sb $3,%%lo(D_800941E9)($1)\n\t"
            "sll $2,$2,2\n\t"
            "lui $1,%%hi(D_80094268)\n\t"
            "addu $1,$1,$2\n\t"
            "lhu $3,%%lo(D_80094268)($1)\n\t"
            "lbu $2,0(%1)\n\t"
            "li %0,-1\n\t"
            "sll $2,$2,2\n\t"
            "lui $1,%%hi(D_800941EC)\n\t"
            "sh $3,%%lo(D_800941EC)($1)\n\t"
            "lui $1,%%hi(D_8009427C)\n\t"
            "addu $1,$1,$2\n\t"
            "lhu $2,%%lo(D_8009427C)($1)\n\t"
            "lui $1,%%hi(D_800941EE)\n\t"
            "sh $2,%%lo(D_800941EE)($1)\n\t"
            ".set at"
            : "=r"(fillValue)
            : "r"(graphState), "r"(graphType)
            : "memory");
        func_80068180(clearEnv, fillValue, 0x5C);
        func_80068180(graphState + 0x6C, -1, 0x14);
        graphType = *(volatile u8 *)graphState;
    } else {
        if (D_800941EA >= 2) {
            D_800941E4(D_80013498, mode);
        }
        D_800941E0->resetGraph(1);
    }
}
