#ifndef RAGE_PC_PSYQ_GTE_MACROS_H
#define RAGE_PC_PSYQ_GTE_MACROS_H

/*
 * PSY-Q GTE (COP2) inline-assembly macros.
 *
 * These are the genuine PSY-Q "inline_c.h" / gtemac.h API used by the original
 * source: each macro expands to a `__asm__ volatile` block emitting the exact
 * cop2 / lwc2 / swc2 / mfc2 / mtc2 / ctc2 encoding. They let GCC schedule the
 * coprocessor ops around genuine C (loop control, table lookups, pointer math)
 * while the surrounding logic is compiled normally.
 *
 * GTE data-register numbers referenced here:
 *   $0..$5   VXY0/VZ0 .. VXY2/VZ2 (input vertex regs)
 *   $7       OTZ            (average-Z result, gte_stotz)
 *   $12..$14 SXY0/SXY1/SXY2 (screen XY results, gte_stsxy*)
 *   $16..$18 SZ1..SZ3       (screen Z fifo)
 *   $24      MAC0           (nclip / scalar result)
 *
 * cop2 command words are emitted as `.word` (unambiguous encoding, matches the
 * repo's existing GAS `gte_macros.inc`); memory/register transfer ops use real
 * lwc2/swc2/mfc2/mtc2 mnemonics so GCC can bind the C operands.
 */

/* ---- vertex loads: VXYn (word 0) + VZn (word 1) from a SVECTOR pointer ---- */
#define gte_ldv0(r0) \
    __asm__ volatile("lwc2 $0, 0(%0)\n\tlwc2 $1, 4(%0)" :: "r"(r0))
#define gte_ldv1(r1) \
    __asm__ volatile("lwc2 $2, 0(%0)\n\tlwc2 $3, 4(%0)" :: "r"(r1))
#define gte_ldv2(r2) \
    __asm__ volatile("lwc2 $4, 0(%0)\n\tlwc2 $5, 4(%0)" :: "r"(r2))
/* load all three consecutive vertices (SVECTOR[3]) at once */
#define gte_ldv3(r0) \
    __asm__ volatile( \
        "lwc2 $0, 0(%0)\n\tlwc2 $1, 4(%0)\n\t" \
        "lwc2 $2, 8(%0)\n\tlwc2 $3, 12(%0)\n\t" \
        "lwc2 $4, 16(%0)\n\tlwc2 $5, 20(%0)" :: "r"(r0))

/* ---- cop2 command words ---- */
#define gte_rtps()  __asm__ volatile(".word 0x4A180001")  /* perspective xform, 1 vertex   */
#define gte_rtpt()  __asm__ volatile(".word 0x4A280030")  /* perspective xform, 3 vertices */
#define gte_nclip() __asm__ volatile(".word 0x4B400006")  /* normal clipping (backface)    */
#define gte_avsz3() __asm__ volatile(".word 0x4B58002D")  /* average of 3 Z                */
#define gte_avsz4() __asm__ volatile(".word 0x4B68002E")  /* average of 4 Z                */

/* ---- result reads (mfc2 from a GTE data register) ---- */
#define gte_stsxy0(v) __asm__ volatile("swc2 $12, %0" : "=m"(v)) /* store SXY0 */
#define gte_stsxy1(v) __asm__ volatile("swc2 $13, %0" : "=m"(v)) /* store SXY1 */
#define gte_stsxy2(v) __asm__ volatile("swc2 $14, %0" : "=m"(v)) /* store SXY2 */
/* store SXY0..2 to consecutive words at an address */
#define gte_stsxy3(p) \
    __asm__ volatile("swc2 $12, 0(%0)\n\tswc2 $13, 4(%0)\n\tswc2 $14, 8(%0)" :: "r"(p))

#define gte_stopz(r)  __asm__ volatile("mfc2 %0, $24" : "=r"(r)) /* read MAC0 (nclip result) */
#define gte_stmac0(r) __asm__ volatile("mfc2 %0, $24" : "=r"(r)) /* read MAC0                */
#define gte_stotz(r)  __asm__ volatile("mfc2 %0, $7"  : "=r"(r)) /* read OTZ (avsz result)   */
#define gte_stsz3(r)  __asm__ volatile("mfc2 %0, $18" : "=r"(r)) /* read SZ3                 */

/* ---- control-register transfers (COP2 control regs $0..$31) ---- */
#define gte_cfc2(r, n) __asm__ volatile("cfc2 %0, $" #n : "=r"(r))
#define gte_ctc2(r, n) __asm__ volatile("ctc2 %0, $" #n :: "r"(r))

/* ---- leading-zero count (LZCS/LZCR) ----
 * Write the source value to LZCS (cop2 data reg $30); after the two mandatory
 * COP2 latency nops the count of leading sign bits is available in LZCR ($31).
 * The hazard nops are part of the documented PSY-Q sequence for this op. */
#define gte_lzc(out, in) \
    __asm__ volatile("mtc2 %1, $30\n\tnop\n\tnop\n\tmfc2 %0, $31" \
                     : "=r"(out) : "r"(in))

/* ---- generic transfer forms (any register) ---- */
#define gte_mfc2(r, n) __asm__ volatile("mfc2 %0, $" #n : "=r"(r))
#define gte_mtc2(r, n) __asm__ volatile("mtc2 %0, $" #n :: "r"(r))
#define gte_lwc2(n, p) __asm__ volatile("lwc2 $" #n ", 0(%0)" :: "r"(p))
#define gte_swc2(n, p) __asm__ volatile("swc2 $" #n ", 0(%0)" :: "r"(p))

/* ---- GTE data-load hazard nop ----
 * A single delay slot required between the last lwc2/mtc2 that writes a GTE
 * data register and a COP2 command that consumes it. Emit only where the
 * retail code shows it (the SDK accessor routines below). */
#define gte_nop() __asm__ volatile("nop")

/* ---- matrix setters (PSY-Q SetRotMatrix / SetLightMatrix / SetColorMatrix /
 * SetTransMatrix) ----
 * Each loads the 3x3 short matrix (5 words) or translation vector (3 words)
 * from a MATRIX pointer into scratch GPRs and copies them to the GTE control
 * registers. Written as a single asm block so the five loads batch ahead of
 * the five ctc2 transfers exactly as the SDK routine does (plain C mis-orders
 * the loads and inserts a load-use nop). */
#define gte_SetRotMatrix(m) __asm__ volatile ( \
    "lw   $8, 0(%0)\n\t"  "lw   $9, 4(%0)\n\t"  "lw   $10, 8(%0)\n\t" \
    "lw   $11, 12(%0)\n\t""lw   $12, 16(%0)\n\t" \
    "ctc2 $8, $0\n\t"  "ctc2 $9, $1\n\t"  "ctc2 $10, $2\n\t" \
    "ctc2 $11, $3\n\t" "ctc2 $12, $4" \
    :: "r"(m) : "$8", "$9", "$10", "$11", "$12")
#define gte_SetLightMatrix(m) __asm__ volatile ( \
    "lw   $8, 0(%0)\n\t"  "lw   $9, 4(%0)\n\t"  "lw   $10, 8(%0)\n\t" \
    "lw   $11, 12(%0)\n\t""lw   $12, 16(%0)\n\t" \
    "ctc2 $8, $8\n\t"  "ctc2 $9, $9\n\t"  "ctc2 $10, $10\n\t" \
    "ctc2 $11, $11\n\t""ctc2 $12, $12" \
    :: "r"(m) : "$8", "$9", "$10", "$11", "$12")
#define gte_SetColorMatrix(m) __asm__ volatile ( \
    "lw   $8, 0(%0)\n\t"  "lw   $9, 4(%0)\n\t"  "lw   $10, 8(%0)\n\t" \
    "lw   $11, 12(%0)\n\t""lw   $12, 16(%0)\n\t" \
    "ctc2 $8, $16\n\t" "ctc2 $9, $17\n\t" "ctc2 $10, $18\n\t" \
    "ctc2 $11, $19\n\t""ctc2 $12, $20" \
    :: "r"(m) : "$8", "$9", "$10", "$11", "$12")
#define gte_SetTransMatrix(m) __asm__ volatile ( \
    "lw   $8, 20(%0)\n\t" "lw   $9, 24(%0)\n\t" "lw   $10, 28(%0)\n\t" \
    "ctc2 $8, $5\n\t"  "ctc2 $9, $6\n\t"  "ctc2 $10, $7" \
    :: "r"(m) : "$8", "$9", "$10")

/* ---- consecutive vector load/store into GTE data registers ---- */
/* load 3 consecutive words into IR1..IR3 ($9..$11) */
#define gte_ldir(p) __asm__ volatile( \
    "lwc2 $9, 0(%0)\n\tlwc2 $10, 4(%0)\n\tlwc2 $11, 8(%0)" :: "r"(p))
/* store IR1..IR3 ($9..$11) to 3 consecutive words */
#define gte_stir(p) __asm__ volatile( \
    "swc2 $9, 0(%0)\n\tswc2 $10, 4(%0)\n\tswc2 $11, 8(%0)" :: "r"(p))
/* store MAC1..MAC3 ($25..$27) to 3 consecutive words */
#define gte_stmac(p) __asm__ volatile( \
    "swc2 $25, 0(%0)\n\tswc2 $26, 4(%0)\n\tswc2 $27, 8(%0)" :: "r"(p))

/* ---- additional COP2 command words (see include/gte_macros.inc) ---- */
#define gte_lc()    __asm__ volatile(".word 0x4A4DA412") /* v0*color mtx, clamp lo  (mvmva) */
#define gte_dpcl()  __asm__ volatile(".word 0x4A680029") /* depth cue color light           */
#define gte_dpct()  __asm__ volatile(".word 0x4AF8002A") /* depth cue color RGB0/1/2        */
#define gte_intpl() __asm__ volatile(".word 0x4A980011") /* interpolate vector & far colour */
#define gte_sqr12() __asm__ volatile(".word 0x4AA80428") /* square of vector, sf=1 (1,19,12)*/
#define gte_sqr0()  __asm__ volatile(".word 0x4AA00428") /* square of vector, sf=0 (1,31, 0)*/
#define gte_op12()  __asm__ volatile(".word 0x4B78000C") /* outer product,     sf=1 (1,19,12)*/
#define gte_op0()   __asm__ volatile(".word 0x4B70000C") /* outer product,     sf=0 (1,31, 0)*/

/* ---- whole-routine GTE accessors that pure C cannot schedule ----
 * These SDK leaf routines are expressed as a single asm block because the
 * required instruction order (batched matrix loads, the odd IR load order,
 * control-register save/restore, and stack-argument loads issued after the
 * COP2 command) is not reproducible from plain C register allocation. Only
 * GTE transfers, matrix loads feeding ctc2, the COP2 command and the required
 * hazard nop appear; there is no ordinary algorithm hidden here. */

/* DPCT over three RGB inputs; results written to three output pointers passed
 * as the 5th..7th arguments (stack slots 16/20/24 of a leaf frame). */
#define gte_dpct3(v0, v1, v2, code) __asm__ volatile( \
    "lwc2 $20,0(%0)\n\tlwc2 $21,0(%1)\n\tlwc2 $22,0(%2)\n\t" \
    "lwc2 $6,0(%2)\n\tmtc2 %3,$8\n\tnop\n\t.word 0x4AF8002A\n\t" \
    "lw $8,16($sp)\n\tlw $9,20($sp)\n\tlw $10,24($sp)\n\t" \
    "swc2 $20,0($8)\n\tswc2 $21,0($9)\n\tswc2 $22,0($10)" \
    :: "r"(v0),"r"(v1),"r"(v2),"r"(code) : "$8","$9","$10","memory")

/* Outer product: save control regs $0/$2/$4, load the diagonal from a matrix
 * pointer, run OP, store MAC1..3, restore the control regs. `cmd` selects the
 * sf variant (op12 = 0x4B78000C, op0 = 0x4B70000C). */
#define gte_op_diag(m, v, out, cmd) __asm__ volatile( \
    "cfc2 $13,$0\n\tcfc2 $14,$2\n\tcfc2 $15,$4\n\t" \
    "lw $8,0(%0)\n\tlw $9,4(%0)\n\tlw $10,8(%0)\n\t" \
    "ctc2 $8,$0\n\tctc2 $9,$2\n\tctc2 $10,$4\n\t" \
    "lwc2 $11,8(%1)\n\tlwc2 $9,0(%1)\n\tlwc2 $10,4(%1)\n\t" \
    "nop\n\t" cmd "\n\t" \
    "swc2 $25,0(%2)\n\tswc2 $26,4(%2)\n\tswc2 $27,8(%2)\n\t" \
    "ctc2 $13,$0\n\tctc2 $14,$2\n\tctc2 $15,$4" \
    :: "r"(m),"r"(v),"r"(out) : "$8","$9","$10","$11","$13","$14","$15","memory")

#endif /* RAGE_PC_PSYQ_GTE_MACROS_H */
