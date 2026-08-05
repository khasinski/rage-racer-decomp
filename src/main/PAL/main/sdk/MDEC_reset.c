#include <sys/types.h>

#include "common.h"
#include "psyq/cd.h"

extern u_char D_800132C8[];
extern u_char D_800132E4[];
extern u_char D_800132F4[];
extern u_char D_80013304[];
extern u_char D_8001332C[];
extern u_char D_80013364[];

long MDEC_in_sync(void);
long MDEC_out_sync(void);

void MDEC_reset(long arg0) {
    register long option asm("$5") = arg0;
    register long zero asm("$0");
    volatile u_long *inBuffer = (volatile u_long *)g_MdecQuantCmd;

    switch (option) {
    case 0:
    *g_MdecCtrlReg = 0x80000000;
    *g_MdecInDmaChcr = zero;
    *g_MdecOutDmaChcr = zero;
    *g_MdecCtrlReg = 0x60000000;
    MDEC_in(inBuffer, 0x20);
    MDEC_in((volatile u_long *)g_MdecIdctCmd, 0x20);
    return;

    case 1:
    *g_MdecCtrlReg = 0x80000000;
    *g_MdecInDmaChcr = 0;
    *g_MdecOutDmaChcr = 0;
    *g_MdecOutDmaChcr;
    *g_MdecCtrlReg = 0x60000000;
    return;

    }
    DebugPrintf(D_800132C8);
}

void MDEC_in(volatile u_long *arg0, long arg1) {
    MDEC_in_sync();
    *g_MdecDpcr |= 0x88;
    *g_MdecInDmaMadr = (u_long)(arg0 + 1);
    *g_MdecInDmaBcr = ((u_long)arg1 >> 5 << 16) | 0x20;
    *g_MdecCmdReg = *arg0;
    *g_MdecInDmaChcr = 0x01000201;
}

void MDEC_out(volatile u_long *arg0, long arg1) {
    MDEC_out_sync();
    *g_MdecDpcr |= 0x88;
    *g_MdecOutDmaChcr = 0;
    *g_MdecOutDmaMadr = (u_long)arg0;
    *g_MdecOutDmaBcr = ((u_long)arg1 >> 5 << 16) | 0x20;
    *g_MdecOutDmaChcr = 0x01000200;
}

long MDEC_in_sync(void) {
    volatile long timeout;

    timeout = 0x100000;
    if (*g_MdecCtrlReg & 0x20000000) {
        do {
            if (--timeout == -1) {
                MDEC_timeout(D_800132E4);
                return -1;
            }
        } while (*g_MdecCtrlReg & 0x20000000);
    }
    return 0;
}

long MDEC_out_sync(void) {
    volatile long timeout;

    timeout = 0x100000;
    if (*g_MdecOutDmaChcr & 0x01000000) {
        do {
            if (--timeout == -1) {
                MDEC_timeout(D_800132F4);
                return -1;
            }
        } while (*g_MdecOutDmaChcr & 0x01000000);
    }
    return 0;
}

long MDEC_timeout(u_char *arg0) {
    u_long status;
    register long ret;

    DebugPrintf(D_80013364, arg0);
    status = *g_MdecCtrlReg;
    DebugPrintf(D_80013304, (*g_MdecInDmaChcr >> 24) & 1, (*g_MdecOutDmaChcr >> 24) & 1, *g_MdecInDmaMadr, *g_MdecOutDmaMadr);
    DebugPrintf(D_8001332C,
                  (~status >> 31) & 1,
                  (status >> 30) & 1,
                  (status >> 29) & 1,
                  (status >> 28) & 1,
                  (status >> 27) & 1,
                  (status >> 25) & 1,
                  (status >> 23) & 1);

    *g_MdecCtrlReg = 0x80000000;
    *g_MdecInDmaChcr = 0;
    *g_MdecOutDmaChcr = 0;

    ret = 0;
    *g_MdecOutDmaChcr;
    *g_MdecCtrlReg = 0x60000000;

    return ret;
}

/*
 * HANDWRITTEN_ASM - excluded from progress (see docs/ASM_AND_GTE_POLICY.md).
 *
 * Symbol:   D_80064554  (0x54D54, 13 words)
 * Reason:   a mutable data word embedded in .text immediately followed by the
 *           function code that reads/writes it via its own absolute address
 *           (t0 = 0x80064554; lw/sw 0(t0)). The leading word (init 0x00FFFFFF)
 *           lives inside the code section, before the routine.
 * Evidence: standard C cannot place a mutable global inline in .text ahead of a
 *           function (globals go to .data/.sdata/.bss, const to .rodata - never
 *           interleaved with code). The self-referential absolute load/store of
 *           its own symbol address confirms a hand-assembled data+code block.
 * C view:   not expressible as compiler C; effectively `static long state =
 *           0x00FFFFFF;` plus a small accessor, but the .text layout is not
 *           reproducible by GCC.
 * Revisit:  only if a linker/section arrangement can reproduce the exact layout.
 */

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/sdk/MDEC_reset", D_80064554);

/*
 * HANDWRITTEN_ASM - excluded from progress (see docs/ASM_AND_GTE_POLICY.md).
 *
 * Symbol:   MdecUnpackStatus  (0x54D88, cc region 2.6.3, TU range 217 words)
 * Reason:   hand-written PSY-Q MDEC/DCT bitstream-decoder inner loop with a
 *           COP0 Status-register epilogue - not expressible as compiler C.
 * Evidence: (1) COP0 ops `mfc0 t1,$12` / `mtc0 t1,$12` read/modify/write the
 *           Status register - GCC emits no COP0 ops from ordinary C and these
 *           are not GTE/COP2 macros. (2) `b` encoded as `bgez $zero`
 *           (0x0401....) - the traditional/SN MIPS assembler encoding; the
 *           bundled GNU as emits `beq $zero,$zero`. (3) No stack frame / custom
 *           ABI: all working regs loaded from and stored to a global state
 *           struct at D_800648C8 - a resumable state machine, not the C ABI.
 *           (4) Signed `add` for pointer/index math where GCC emits `addu`.
 *           (5) hand `.set noreorder` delay-slot packing.
 * Revisit:  only with proof it was compiler-generated C.
 */

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/sdk/MDEC_reset", MdecUnpackStatus);
