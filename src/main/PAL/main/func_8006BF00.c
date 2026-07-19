#include "common.h"

extern s32 D_80099060[];
extern u8 D_80099318[];
extern s32 D_800990E0[];
extern u8 D_8009905D;
extern s32 D_8009BB08;
extern s32 D_8009BB0C;
extern void *D_8009BB10;
extern volatile u32 *D_80099344;
extern char D_8001391C[];
extern char D_80013814[];
extern char D_80013824[];

s32 func_8006DD30(s32);
void func_80063C38(char *);
void func_8001674C(char *fmt, ...);
void func_8006BAF0(void);

s32 func_8006BF00(s32 arg) {
    register s32 *b60 asm("$19");
    register u8 *b318 asm("$17");
    register s32 *bE0 asm("$16");
    s32 result;

    /* Forces GCC to reserve the 6-word outgoing-arg frame slot without
       emitting the call (the printf below is issued from inline asm). */
    if (0) func_8001674C(D_80013824, 0, 0, 0, 0);

    D_8009BB08 = func_8006DD30(-1) + 0x3C0;
    b60 = D_80099060;
    b318 = D_80099318;
    bE0 = D_800990E0;
    D_8009BB0C = 0;
    D_8009BB10 = D_8001391C;
    do {
        s32 status;
        if (func_8006DD30(-1) > D_8009BB08 || D_8009BB0C++ > 0x3C0000) {
            register void *a1v asm("$5");
            register s32 a2v asm("$6");
            register s32 a3v asm("$7");
            register s32 t2 asm("$2");
            register s32 t3 asm("$3");
            register s32 t4 asm("$4");

            func_80063C38(D_80013814);
            /* func_8001674C(D_80013824, D_8009BB10,
                             D_80099060[D_8009905D],
                             D_800990E0[D_80099318[0]],
                             D_800990E0[D_80099318[1]]);
               Emitted as inline asm to reproduce the exact argument
               evaluation/scheduling order of the original build. */
            asm volatile("lbu $4, 0($17)\n\tlbu $2, 1($17)"
                         : "=r"(t4), "=r"(t2) : "r"(b318));
            asm volatile("lui $5, %%hi(D_8009BB10)\n\tlw $5, %%lo(D_8009BB10)($5)"
                         : "=r"(a1v));
            asm volatile("sll $2, $2, 2\n\taddu $2, $2, $16\n\tsll $4, $4, 2\n\tlw $3, 0($2)"
                         : "=r"(t3), "=r"(t2), "=r"(t4)
                         : "r"(bE0), "1"(t2), "2"(t4));
            asm volatile("lui $2, %%hi(D_8009905D)\n\tlbu $2, %%lo(D_8009905D)($2)\n\t"
                         "addu $4, $4, $16\n\tsll $2, $2, 2\n\taddu $2, $2, $19"
                         : "=r"(t2), "=r"(t4)
                         : "r"(bE0), "r"(b60), "1"(t4));
            asm volatile("sw $3, 16($sp)\n\tlw $6, 0($2)\n\tlw $7, 0($4)"
                         : "=r"(a2v), "=r"(a3v)
                         : "r"(t3), "r"(t2), "r"(t4) : "memory");
            func_8001674C(D_80013824, a1v, a2v, a3v);
            func_8006BAF0();
            status = -1;
        } else {
            status = 0;
        }
        if (status != 0) {
            result = -1;
            goto done;
        }
        if ((*D_80099344 & 0x1000000) == 0) {
            result = 0;
            goto done;
        }
    } while (arg == 0);
    result = 1;
done:
    return result;
}
