#ifndef PSYQ_LIBC_INTERNAL_H
#define PSYQ_LIBC_INTERNAL_H

#include "common.h"

typedef union LibcFormatHeader {
    u32 flags;
    struct {
        u8 flags;
        u8 prefix;
        u16 unused;
    } bytes;
} LibcFormatHeader;

typedef struct LibcFormatSpec {
    LibcFormatHeader header;
    s32 width;
    s32 precision;
} LibcFormatSpec;

extern u8 g_LibcCtype[];
extern LibcFormatSpec g_LibcDefaultFormat;

#endif
