#include "common.h"

typedef struct {
    u8 raw[8];
} UnkCopyChunk;

extern UnkCopyChunk D_8007BE68[];

void func_80021288(void *arg0, s32 *arg1) {
    UnkCopyChunk *dst;
    UnkCopyChunk *src;
    s32 i;

    i = 0;
    dst = arg0;
    src = D_8007BE68;
    do {
        *dst = *src;
        dst++;
        i++;
    } while (src++, i < 13);

    arg1[1] = 3;
    arg1[0] = 0;
    arg1[2] = 0;
    arg1[3] = -1;
    arg1[4] = 0;
}
