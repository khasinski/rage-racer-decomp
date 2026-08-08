#ifndef RAGE_PC_STDIO_H
#define RAGE_PC_STDIO_H

#include "common.h"

s32 printf(const char *format, ...);
s32 sprintf(char *dest, const char *format, ...);
void putchar(s32 ch);
void puts(u8 *str);

#endif
