#ifndef RAGE_PC_STRING_H
#define RAGE_PC_STRING_H

#include "common.h"

u8 *memchr(u8 *buf, s32 ch, s32 len);
void memcpy(u8 *dest, u8 *src, long count);
void *memmove(u8 *dest, u8 *src, s32 count);
long strcmp(u8 *lhs, u8 *rhs);
long strncmp(u8 *lhs, u8 *rhs, long len);
s32 strlen(u8 *str);

#endif
