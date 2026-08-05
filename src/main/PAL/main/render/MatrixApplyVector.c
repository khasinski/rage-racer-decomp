#include "common.h"
/* HANDWRITTEN_ASM - PSY-Q libgte hand-asm (matrix/GTE), excluded from progress (docs/ASM_AND_GTE_POLICY.md). */

#define FAST_LOAD(offset)                              \
    do {                                               \
        s32 value;                                     \
        __asm__ volatile("lw %0, %1(%2)"               \
                         : "=r"(value)                 \
                         : "i"(offset), "r"(v));       \
        out[(offset) / 4] = value;                     \
    } while (0)

#define DOT_ROW(row, dst)                              \
    do {                                               \
        register s32 value asm("$2");                  \
        __asm__ volatile(                              \
            "lh    $3, %1(%5)\n\t"                     \
            "lw    %0, 0(%4)\n\t"                      \
            "nop\n\t"                                  \
            "mult  $3, %0\n\t"                         \
            "lh    $4, %2(%5)\n\t"                     \
            "mflo  %0\n\t"                             \
            "lw    $3, 4(%4)\n\t"                      \
            "nop\n\t"                                  \
            "mult  $4, $3\n\t"                         \
            "lh    $4, %3(%5)\n\t"                     \
            "mflo  $5\n\t"                             \
            "lw    $3, 8(%4)\n\t"                      \
            "nop\n\t"                                  \
            "mult  $4, $3\n\t"                         \
            "addu  %0, %0, $5\n\t"                     \
            "mflo  $3\n\t"                             \
            "addu  %0, %0, $3\n\t"                     \
            "sra   %0, %0, 12"                         \
            : "=r"(value)                              \
            : "i"((row) + 0), "i"((row) + 2), "i"((row) + 4), "r"(v), "r"(m) \
            : "$3", "$4", "$5", "hi", "lo");         \
        out[(dst)] = value;                            \
    } while (0)

void MatrixApplyVector(s16 *mtx, s32 *vec, s32 *out);
void MatrixApplyVector(s16 *mtx, s32 *vec, s32 *out) {
    s16 *m = mtx;
    s32 *v = vec;

    if (*(s32 *)&m[0] == 0x1000 && m[2] == 0) {
        FAST_LOAD(0);
    } else {
        DOT_ROW(0, 0);
    }

    if (m[3] == 0 && *(s32 *)&m[4] == 0x1000) {
        FAST_LOAD(4);
    } else {
        DOT_ROW(6, 1);
    }

    if (*(s32 *)&m[6] == 0 && m[8] == 0x1000) {
        FAST_LOAD(8);
    } else {
        DOT_ROW(12, 2);
    }
}
