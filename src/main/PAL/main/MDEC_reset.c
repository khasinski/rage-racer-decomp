#include "common.h"

extern volatile u32 *D_80083164;
extern volatile u32 *D_80083168;
extern volatile u32 *D_8008316C;
extern volatile u32 *D_80083170;
extern volatile u32 *D_80083174;
extern volatile u32 *D_80083178;
extern volatile u32 *D_80083194;
extern volatile u32 *D_80083198;
extern volatile u32 *D_8008319C;
extern u8 D_800132C8[];
extern u8 D_800132E4[];
extern u8 D_800132F4[];
extern u8 D_80013304[];
extern u8 D_8001332C[];
extern u8 D_80013364[];
extern u8 D_8008305C[];
extern u8 D_800830E0[];

s32 MDEC_in_sync(void) asm("func_800642F4");
s32 MDEC_out_sync(void) asm("func_8006438C");
s32 MDEC_timeout(u8 *arg0) asm("func_80064424");

void MDEC_in(volatile u32 *arg0, s32 arg1) asm("func_800641D0");

void MDEC_reset(s32 arg0) asm("func_800640D4");
void MDEC_reset(s32 arg0) {
    register s32 option asm("$5") = arg0;
    register s32 zero asm("$0");
    volatile u32 *inBuffer = (volatile u32 *)D_8008305C;

    if (option == 0) {
        goto zero;
    }
    if (option == 1) {
        goto one;
    }
    goto bad;

zero:
    *D_80083198 = 0x80000000;
    *D_8008316C = zero;
    *D_80083178 = zero;
    *D_80083198 = 0x60000000;
    MDEC_in(inBuffer, 0x20);
    MDEC_in((volatile u32 *)D_800830E0, 0x20);
    return;

one:
    *D_80083198 = 0x80000000;
    *D_8008316C = 0;
    *D_80083178 = 0;
    *D_80083178;
    *D_80083198 = 0x60000000;
    return;

bad:
    GameDebugPrintf(D_800132C8);
}

void MDEC_in(volatile u32 *arg0, s32 arg1) {
    MDEC_in_sync();
    *D_8008319C |= 0x88;
    *D_80083164 = (u32)(arg0 + 1);
    *D_80083168 = ((u32)arg1 >> 5 << 16) | 0x20;
    *D_80083194 = *arg0;
    *D_8008316C = 0x01000201;
}

void MDEC_out(volatile u32 *arg0, s32 arg1) asm("func_80064264");
void MDEC_out(volatile u32 *arg0, s32 arg1) {
    MDEC_out_sync();
    *D_8008319C |= 0x88;
    *D_80083178 = 0;
    *D_80083170 = (u32)arg0;
    *D_80083174 = ((u32)arg1 >> 5 << 16) | 0x20;
    *D_80083178 = 0x01000200;
}

s32 MDEC_in_sync(void) {
    volatile s32 timeout;

    timeout = 0x100000;
    if (*D_80083198 & 0x20000000) {
        do {
            if (--timeout == -1) {
                MDEC_timeout(D_800132E4);
                return -1;
            }
        } while (*D_80083198 & 0x20000000);
    }
    return 0;
}

s32 MDEC_out_sync(void) {
    volatile s32 timeout;

    timeout = 0x100000;
    if (*D_80083178 & 0x01000000) {
        do {
            if (--timeout == -1) {
                MDEC_timeout(D_800132F4);
                return -1;
            }
        } while (*D_80083178 & 0x01000000);
    }
    return 0;
}

s32 MDEC_timeout(u8 *arg0) {
    u32 status;
    register s32 ret asm("$2");

    GameDebugPrintf(D_80013364, arg0);
    status = *D_80083198;
    GameDebugPrintf(D_80013304, (*D_8008316C >> 24) & 1, (*D_80083178 >> 24) & 1, *D_80083164, *D_80083170);
    GameDebugPrintf(D_8001332C,
                  (~status >> 31) & 1,
                  (status >> 30) & 1,
                  (status >> 29) & 1,
                  (status >> 28) & 1,
                  (status >> 27) & 1,
                  (status >> 25) & 1,
                  (status >> 23) & 1);

    *D_80083198 = 0x80000000;
    *D_8008316C = 0;
    *D_80083178 = 0;

    asm volatile("" ::: "memory");
    ret = 0;
    asm volatile("" : : "r"(ret));
    *D_80083178;
    *D_80083198 = 0x60000000;

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
 * C view:   not expressible as compiler C; effectively `static s32 state =
 *           0x00FFFFFF;` plus a small accessor, but the .text layout is not
 *           reproducible by GCC.
 * Revisit:  only if a linker/section arrangement can reproduce the exact layout.
 */

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/MDEC_reset", D_80064554);

/*
 * HANDWRITTEN_ASM - excluded from progress (see docs/ASM_AND_GTE_POLICY.md).
 *
 * Symbol:   func_80064588  (0x54D88, cc region 2.6.3, TU range 217 words)
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

INCLUDE_ASM("asm/nonmatchings/PAL/main", func_80064588);
