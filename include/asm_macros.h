#ifndef RAGE_PC_ASM_MACROS_H
#define RAGE_PC_ASM_MACROS_H

/*
 * Matching helper macros.
 *
 * These wrap the small amounts of inline assembly that are occasionally
 * required to reproduce the retail compiler's exact instruction ordering
 * where the rebuilt cc1-psx-263 differs from the original PSY-Q ccpsx.
 * They emit ordinary instructions with well-defined C semantics -- they are
 * documented here (rather than left as raw __asm__ in the function body) so
 * the intent is clear and the idiom is reusable.
 */

/*
 * LA_ORDERED(dst, sym, dep)
 *   Semantically: dst = &sym;
 *
 * Loads the address of `sym` (a `la` -> lui/addiu pair) into `dst`, forcing
 * GCC to schedule that address load AFTER `dep` has been materialized.
 *
 * Why: GCC 2.6.3 canonicalizes commutative address arithmetic and will hoist
 * a bare `&sym` load ahead of a preceding shift/index computation, which
 * diverges from the retail ordering. Threading `dep` through as an input
 * operand reintroduces the data dependency so the original instruction order
 * (index shift first, then the address load) is reproduced -- while staying
 * non-volatile so a following induction increment still fills its delay slot.
 *
 * `dst` should be a `register ... asm("$N")` pinned to the target register.
 */
#define LA_ORDERED(dst, sym, dep) __asm__("la %0, " #sym : "=r"(dst) : "r"(dep))

/*
 * MATCH_REGISTER(type, name, reg)
 *   Pins a C value to the register selected by the retail compiler.
 *
 * KEEP_REGISTER(value)
 *   Preserves that register choice across an optimization boundary without
 *   emitting an instruction.  This is useful for paired cursor pointers when
 *   GCC would otherwise replace one cursor with a second induction variable.
 */
#define MATCH_REGISTER(type, name, reg) register type name asm(reg)
#define KEEP_REGISTER(value) asm("" : "=r"(value) : "0"(value))
#define MOVE_REGISTER(dst, src) asm("move %0, %1" : "=r"(dst) : "r"(src))

#endif
