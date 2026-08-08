#ifndef RAGE_PC_STDIO_H
#define RAGE_PC_STDIO_H

#include "common.h"

s32 printf(u8 *format, ...);
s32 sprintf(u8 *dest, u8 *format, ...);
void putchar(s32 ch);
void puts(u8 *str);

#endif
