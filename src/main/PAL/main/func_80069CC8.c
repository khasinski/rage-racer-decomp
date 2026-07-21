#include "common.h"
#include "psyq/gte.h"

Matrix *func_80069CC8(Matrix *src, Matrix *dst) {
    s16 *srcp = (s16 *)src;
    register s16 *dstp asm("$5") = (s16 *)dst;
    register Matrix *ret asm("$2");
    register s32 value0 asm("$9");
    register s32 value1 asm("$10");
    register s32 value2 asm("$11");

    value0 = srcp[0];
    asm("addu $2,$5,$0");
    dstp[0] = value0;
    value1 = srcp[3];
    value0 = srcp[6];
    dstp[1] = value1;
    value2 = srcp[1];
    dstp[2] = value0;
    value1 = srcp[4];
    dstp[3] = value2;
    value0 = srcp[7];
    dstp[4] = value1;
    value2 = srcp[2];
    dstp[5] = value0;
    value1 = srcp[5];
    dstp[6] = value2;
    value0 = srcp[8];
    dstp[7] = value1;
    dstp[8] = value0;
    return ret;
}

/*
 * func_80069D18 = RotMatrix (psyq libgte): builds a 3x3 fixed-point rotation MATRIX from 3 Euler angles via the packed sin/cos table at 0x80094FAC. Hand-written asm: uses 14x 32-bit multu+mflo (GCC 2.6.3 never emits 32-bit multu, only mult) - unmatchable as compiled C, kept as documented asm.
 */
asm(
    ".set noreorder
"
    ".set noat
"
    ".globl func_80069D18
"
    "func_80069D18:
"
    ".word 0x848F0000
"
    ".word 0x00A01021
"
    ".word 0x05E1000E
"
    ".word 0x31F90FFF
"
    ".word 0x000F7823
"
    ".word 0x05E10001
"
    ".word 0x31EF0FFF
"
    ".word 0x000FC080
"
    ".word 0x3C198009
"
    ".word 0x0338C821
"
    ".word 0x8F394FAC
"
    ".word 0x00000000
"
    ".word 0x0019C400
"
    ".word 0x0018C403
"
    ".word 0x00185823
"
    ".word 0x0801A75F
"
    ".word 0x00194403
"
    ".word 0x0019C080
"
    ".word 0x3C198009
"
    ".word 0x0338C821
"
    ".word 0x8F394FAC
"
    ".word 0x00000000
"
    ".word 0x0019C400
"
    ".word 0x00185C03
"
    ".word 0x00194403
"
    ".word 0x848F0002
"
    ".word 0x00000000
"
    ".word 0x05E1000E
"
    ".word 0x31F90FFF
"
    ".word 0x000F7823
"
    ".word 0x05E10001
"
    ".word 0x31EF0FFF
"
    ".word 0x000FC080
"
    ".word 0x3C198009
"
    ".word 0x0338C821
"
    ".word 0x8F394FAC
"
    ".word 0x00000000
"
    ".word 0x00196400
"
    ".word 0x000C6403
"
    ".word 0x000C7023
"
    ".word 0x0801A779
"
    ".word 0x00194C03
"
    ".word 0x0019C080
"
    ".word 0x3C198009
"
    ".word 0x0338C821
"
    ".word 0x8F394FAC
"
    ".word 0x00000000
"
    ".word 0x00197400
"
    ".word 0x000E7403
"
    ".word 0x000E6023
"
    ".word 0x00194C03
"
    ".word 0x012B0019
"
    ".word 0x848F0004
"
    ".word 0xA4AE0004
"
    ".word 0x0000C012
"
    ".word 0x0018C823
"
    ".word 0x00197303
"
    ".word 0x01280019
"
    ".word 0xA4AE000A
"
    ".word 0x05E10011
"
    ".word 0x31F90FFF
"
    ".word 0x0000C012
"
    ".word 0x00187303
"
    ".word 0xA4AE0010
"
    ".word 0x000F7823
"
    ".word 0x05E10001
"
    ".word 0x31EF0FFF
"
    ".word 0x000FC080
"
    ".word 0x3C198009
"
    ".word 0x0338C821
"
    ".word 0x8F394FAC
"
    ".word 0x00000000
"
    ".word 0x0019C400
"
    ".word 0x0018C403
"
    ".word 0x00186823
"
    ".word 0x0801A79E
"
    ".word 0x00195403
"
    ".word 0x00007812
"
    ".word 0x000F7303
"
    ".word 0xA4AE0010
"
    ".word 0x0019C080
"
    ".word 0x3C198009
"
    ".word 0x0338C821
"
    ".word 0x8F394FAC
"
    ".word 0x00000000
"
    ".word 0x0019C400
"
    ".word 0x00186C03
"
    ".word 0x00195403
"
    ".word 0x01490019
"
    ".word 0x00000000
"
    ".word 0x00000000
"
    ".word 0x00007812
"
    ".word 0x000F7303
"
    ".word 0xA4AE0000
"
    ".word 0x01A90019
"
    ".word 0x00000000
"
    ".word 0x00000000
"
    ".word 0x00007812
"
    ".word 0x000F7023
"
    ".word 0x000E7B03
"
    ".word 0x014C0019
"
    ".word 0xA4AF0002
"
    ".word 0x00000000
"
    ".word 0x00007812
"
    ".word 0x000FC303
"
    ".word 0x00000000
"
    ".word 0x030B0019
"
    ".word 0x00000000
"
    ".word 0x00000000
"
    ".word 0x00007812
"
    ".word 0x000F7303
"
    ".word 0x00000000
"
    ".word 0x01A80019
"
    ".word 0x00000000
"
    ".word 0x00000000
"
    ".word 0x00007812
"
    ".word 0x000FCB03
"
    ".word 0x032E7823
"
    ".word 0x03080019
"
    ".word 0xA4AF0006
"
    ".word 0x00000000
"
    ".word 0x00007012
"
    ".word 0x000E7B03
"
    ".word 0x00000000
"
    ".word 0x01AB0019
"
    ".word 0x00000000
"
    ".word 0x00000000
"
    ".word 0x00007012
"
    ".word 0x000ECB03
"
    ".word 0x032F7021
"
    ".word 0x01AC0019
"
    ".word 0xA4AE000C
"
    ".word 0x00000000
"
    ".word 0x00007812
"
    ".word 0x000FC303
"
    ".word 0x00000000
"
    ".word 0x030B0019
"
    ".word 0x00000000
"
    ".word 0x00000000
"
    ".word 0x00007812
"
    ".word 0x000F7303
"
    ".word 0x00000000
"
    ".word 0x01480019
"
    ".word 0x00000000
"
    ".word 0x00000000
"
    ".word 0x00007812
"
    ".word 0x000FCB03
"
    ".word 0x032E7821
"
    ".word 0x03080019
"
    ".word 0xA4AF0008
"
    ".word 0x00000000
"
    ".word 0x00007012
"
    ".word 0x000E7B03
"
    ".word 0x00000000
"
    ".word 0x014B0019
"
    ".word 0x00000000
"
    ".word 0x00000000
"
    ".word 0x00007012
"
    ".word 0x000ECB03
"
    ".word 0x032F7023
"
    ".word 0xA4AE000E
"
    ".word 0x03E00008
"
    ".word 0x00000000
"
    ".word 0x00000000
"
);
