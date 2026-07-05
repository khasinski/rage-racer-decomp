#include "common.h"

#define FAST_LOAD(offset)                              \
    do {                                               \
        register s32 value asm("$2");                  \
        __asm__ volatile("lw $2, %1($8)"               \
                         : "=r"(value)                 \
                         : "i"(offset), "r"(v));       \
        out[(offset) / 4] = value;                     \
    } while (0)

#define DOT_ROW(row, dst)                              \
    do {                                               \
        register s32 value asm("$2");                  \
        __asm__ volatile(                              \
            "lh    $3, %1($7)\n\t"                     \
            "lw    $2, 0($8)\n\t"                      \
            "nop\n\t"                                  \
            "mult  $3, $2\n\t"                         \
            "lh    $4, %2($7)\n\t"                     \
            "mflo  $2\n\t"                             \
            "lw    $3, 4($8)\n\t"                      \
            "nop\n\t"                                  \
            "mult  $4, $3\n\t"                         \
            "lh    $4, %3($7)\n\t"                     \
            "mflo  $5\n\t"                             \
            "lw    $3, 8($8)\n\t"                      \
            "nop\n\t"                                  \
            "mult  $4, $3\n\t"                         \
            "addu  $2, $2, $5\n\t"                     \
            "mflo  $3\n\t"                             \
            "addu  $2, $2, $3\n\t"                     \
            "sra   $2, $2, 12"                         \
            : "=r"(value)                              \
            : "i"((row) + 0), "i"((row) + 2), "i"((row) + 4), "r"(v) \
            : "$3", "$4", "$5", "hi", "lo");         \
        out[(dst)] = value;                            \
    } while (0)

void MatrixApplyVector(s16 *mtx, s32 *vec, s32 *out) asm("func_80068340");
void MatrixApplyVector(s16 *mtx, s32 *vec, s32 *out) {
    register s16 *m asm("$7") = mtx;
    register s32 *v asm("$8") = vec;

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
