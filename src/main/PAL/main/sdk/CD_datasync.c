#include <sys/types.h>

#include "common.h"

extern long g_CdCommandNames[] asm("D_80099060");
extern u_char g_CdSyncStatus[] asm("D_80099318");
extern long g_CdIntrNames[] asm("D_800990E0");
extern u_char D_8009905D;
extern long g_CdTimeoutDeadline asm("D_8009BB08");
extern long g_CdTimeoutCounter asm("D_8009BB0C");
extern void *D_8009BB10;
extern volatile u_long *g_CdDmaChcr asm("D_80099344");
extern char D_8001391C[];
extern char D_80013814[];
extern char D_80013824[];

long VSync(long mode) asm("func_8006DD30");
void func_80063C38(char *);
void func_8006BAF0(void);

/* Waits for the CD data transfer to finish, with a 0x3C0-vblank / 0x3C0000-
 * spin watchdog that prints "CD timeout: " and flushes. Named by the same
 * self-naming idiom as CD_sync / CD_ready / CD_cw: it stores its own name
 * string D_8001391C = "CD_datasync" into the D_8009BB10 slot the timeout
 * message prints as its first %s. arg 0 blocks, non-zero polls. */
long CD_datasync(long mode) asm("func_8006BF00");
long CD_datasync(long arg) {
    register long *b60 asm("$19");
    register u_char *b318 asm("$17");
    register long *bE0 asm("$16");
    long result;

    /* Forces GCC to reserve the 6-word outgoing-arg frame slot without
       emitting the call (the printf below is issued from inline asm). */
    if (0) GameDebugPrintf(D_80013824, 0, 0, 0, 0);

    g_CdTimeoutDeadline = VSync(-1) + 0x3C0;
    b60 = g_CdCommandNames;
    b318 = g_CdSyncStatus;
    bE0 = g_CdIntrNames;
    g_CdTimeoutCounter = 0;
    D_8009BB10 = D_8001391C;
    do {
        long status;
        if (VSync(-1) > g_CdTimeoutDeadline || g_CdTimeoutCounter++ > 0x3C0000) {
            register void *a1v asm("$5");
            register long a2v asm("$6");
            register long a3v asm("$7");
            register long t2 asm("$2");
            register long t3 asm("$3");
            register long t4 asm("$4");

            func_80063C38(D_80013814);
            /* GameDebugPrintf(D_80013824, D_8009BB10,
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
            GameDebugPrintf(D_80013824, a1v, a2v, a3v);
            func_8006BAF0();
            status = -1;
        } else {
            status = 0;
        }
        if (status != 0) {
            result = -1;
            goto done;
        }
        if ((*g_CdDmaChcr & 0x1000000) == 0) {
            result = 0;
            goto done;
        }
    } while (arg == 0);
    result = 1;
done:
    return result;
}
