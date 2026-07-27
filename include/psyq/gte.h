#ifndef RAGE_PC_PSYQ_GTE_H
#define RAGE_PC_PSYQ_GTE_H

#include <sys/types.h>

#include "common.h"

typedef struct Matrix {
    short m[3][3];
    long t[3];
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
void SetDQA(long dqa) asm("func_80069A00");
void SetDQB(long dqb) asm("func_80069A0C");
/* Fog: derives dqa from the near distance `a` and projection distance `h`. */
void SetFogNear(long a, long h) asm("func_800686D4");
/* Background / far colours; each component is shifted left by 4 on the way in. */
void SetBackColor(long rbk, long gbk, long bbk) asm("func_80069A18");
void SetFarColor(long rfc, long gfc, long bfc) asm("func_80069A38");
void SetGeomOffset(long ofx, long ofy) asm("func_80069A58");
void SetGeomScreen(long h) asm("func_80069A78");
/* Single COP2 commands. */
void LightColor(void *v0, void *v1) asm("func_80069A88");
void DpqColor3(
    void *v0,
    void *v1,
    void *v2,
    long p,
    void *o0,
    void *o1,
    void *o2) asm("func_80069AD8");
void Intpl(void *v0, long p, void *v1) asm("func_80069B14");
void *Square12(void *v0, void *v1) asm("func_80069B38");
void *Square0(void *v0, void *v1) asm("func_80069B60");
long AverageZ3(long sz0, long sz1, long sz2) asm("func_80069B88");
long AverageZ4(long sz0, long sz1, long sz2, long sz3) asm("func_80069BA8");
void OuterProduct12(void *v0, void *v1, void *v2) asm("func_80069BCC");
void OuterProduct0(void *v0, void *v1, void *v2) asm("func_80069C24");
long NormalClip(long sxy0, long sxy1, long sxy2) asm("func_80069C98");
long Lzc(long data) asm("func_80069C7C");
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
short *ApplyMatrixSV(void *m, void *v0, short *v1) asm("func_800696C8");

/* Matrix scaling. ScaleMatrix does m[i][j] *= v[j] (column j scaled by
 * v[j]); ScaleMatrixL does m[i][j] *= v[i]. LibRef47 8-150 / 8-151. */
void *ScaleMatrix(void *m, void *v) asm("func_80069728");
void *ScaleMatrixL(void *m, void *v) asm("func_80069110");
/* Rotation matrix from an SVECTOR of Z/Y/X Euler angles (raw asm sibling). */
void *RotMatrix(void *r, void *m) asm("func_80069D18");
/* Square root in 12-bit fixed point: returns sqrt(a << 12), i.e. 64*sqrt(a).
 * Normalises with Lzc, then runs the hyperbolic CORDIC in func_80068738. */
long SquareRoot12(long a) asm("func_8006888C");

#endif
