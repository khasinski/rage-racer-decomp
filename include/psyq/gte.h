#ifndef RAGE_PC_PSYQ_GTE_H
#define RAGE_PC_PSYQ_GTE_H

#include "common.h"

typedef struct Matrix {
    s16 m[3][3];
    s32 t[3];
} Matrix;

/*
 * libgte leaf routines. These load GTE (COP2) control/data registers or run a
 * single COP2 command; the bodies live in src/main/PAL/main/func_80069858.c and
 * are written with the gte_* macros from psyq/gte_macros.h.
 */
void SetRotMatrix(void *m) asm("func_80069858");
void SetLightMatrix(void *m) asm("func_80069888");
void SetColorMatrix(void *m) asm("func_800698B8");
void SetTransMatrix(void *m) asm("func_800698E8");
/* Perspective divide constants (control regs 27 / 28). */
void SetDQA(s32 dqa) asm("func_80069A00");
void SetDQB(s32 dqb) asm("func_80069A0C");
/* Fog: derives dqa from the near distance `a` and projection distance `h`. */
void SetFogNear(s32 a, s32 h) asm("func_800686D4");
/* Background / far colours; each component is shifted left by 4 on the way in. */
void SetBackColor(s32 rbk, s32 gbk, s32 bbk) asm("func_80069A18");
void SetFarColor(s32 rfc, s32 gfc, s32 bfc) asm("func_80069A38");
void SetGeomOffset(s32 ofx, s32 ofy) asm("func_80069A58");
void SetGeomScreen(s32 h) asm("func_80069A78");
/* Single COP2 commands. */
void LightColor(void *v0, void *v1) asm("func_80069A88");
void DpqColor3(
    void *v0,
    void *v1,
    void *v2,
    s32 p,
    void *o0,
    void *o1,
    void *o2) asm("func_80069AD8");
void Intpl(void *v0, s32 p, void *v1) asm("func_80069B14");
void *Square12(void *v0, void *v1) asm("func_80069B38");
void *Square0(void *v0, void *v1) asm("func_80069B60");
s32 AverageZ3(s32 sz1, s32 sz2, s32 sz3) asm("func_80069B88");
s32 AverageZ4(s32 sz0, s32 sz1, s32 sz2, s32 sz3) asm("func_80069BA8");
void OuterProduct12(void *m, void *v0, void *v1) asm("func_80069BCC");
void OuterProduct0(void *m, void *v0, void *v1) asm("func_80069C24");
s32 NormalClip(s32 sxy0, s32 sxy1, s32 sxy2) asm("func_80069C98");
s32 Lzc(s32 data) asm("func_80069C7C");
/* Transposes the 3x3 rotation part of m0 into m1, leaving the translation. */
Matrix *TransposeMatrix(Matrix *m0, Matrix *m1) asm("func_80069CC8");

/*
 * Matrix multiply family. Each loads m0 into the GTE rotation control registers
 * (ctc2 $0..$4) and pushes the three columns of m1 through MVMVA
 * (cop2 0x486012, mx=rotation, v=V0, no translation, sf=12), so only the 3x3
 * rotation part is touched. They differ only in where the product lands:
 * MulMatrix  -> m0, MulMatrix2 -> m1, MulMatrix0 -> m2. Each returns it.
 */
void *MulMatrix(void *m0, void *m1) asm("func_80069458");
void *MulMatrix2(void *m0, void *m1) asm("func_80069568");
void *MulMatrix0(void *m0, void *m1, void *m2) asm("func_80068B98");
/* v1 = m * v0 through the same MVMVA; v0 is a short vector, v1 gets MAC1..3. */
void *ApplyMatrix(void *m, void *v0, void *v1) asm("func_80069678");
/* SVECTOR in, SVECTOR out; returns v1. LibRef47 8-23. */
s16 *ApplyMatrixSV(void *m, void *v0, s16 *v1) asm("func_800696C8");

/* Matrix scaling. ScaleMatrix does m[i][j] *= v[j] (column j scaled by
 * v[j]); ScaleMatrixL does m[i][j] *= v[i]. LibRef47 8-150 / 8-151. */
void *ScaleMatrix(void *m, void *v) asm("func_80069728");
void *ScaleMatrixL(void *m, void *v) asm("func_80069110");
/* Rotation matrix from an SVECTOR of Z/Y/X Euler angles (raw asm sibling). */
void *RotMatrix(void *r, void *m) asm("func_80069D18");
/* Square root in 12-bit fixed point: returns sqrt(a << 12), i.e. 64*sqrt(a).
 * Normalises with Lzc, then runs the hyperbolic CORDIC in func_80068738. */
s32 SquareRoot12(s32 a) asm("func_8006888C");

#endif
