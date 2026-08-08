#include "common.h"
#include <stdio.h>
#include <string.h>
#include "game/state.h"
#include "psyq/libc_internal.h"

/*
 * PSY-Q libc sprintf: the whole formatter, with no vsprintf split. Identified by
 * its %X / %x digit tables at g_LibcUpperDigits / g_LibcLowerDigits and by its memchr /
 * memmove / strlen callees.
 */
typedef struct LibcFormatWork {
    u8 number[0x200];
    LibcFormatSpec spec;
} LibcFormatWork;

#define LIBC_LEFT       0x01
#define LIBC_PLUS       0x02
#define LIBC_ALT        0x04
#define LIBC_ZERO       0x08
#define LIBC_PRECISION  0x10
#define LIBC_SHORT      0x20
#define LIBC_LONG       0x40
#define LIBC_LDOUBLE    0x80

#define LIBC_NEXT_ARG(type) (args += 4, *(type *)(args - 4))
#define LIBC_NEXT_NUM_ARG(type) ({ \
    oldArgs = args; \
    args = oldArgs + 4; \
    *(type *)oldArgs; \
})
#define LIBC_LOCAL_ZERO ({ s32 zero = '0'; zero; })

s32 sprintf(u8 *dest, u8 *format, ...) {
    LibcFormatWork work;
    u8 *argState[2];
#define args argState[0]
    u8 *src;
    u8 *end;
    u8 *digits;
    u32 value;
    u8 *oldArgs;
    u32 nflags;
    s32 length;
    s32 count;
    s32 width;
    s32 c;
    s32 hash;
    s32 zeroFlag;
    register s32 plus;
    register s32 minus;

    args = (u8 *)&format + 4;
    c = *format;
    length = 0;
    if (c == 0) {
        goto finished;
    }
    minus = '-';
    plus = '+';

    do {
        if (c != '%') {
            dest[length++] = c;
            continue;
        }

        work.spec = g_LibcDefaultFormat;
        hash = '#';
        zeroFlag = '0';

        for (;;) {
        c = *++format;
        if (c == minus) {
            work.spec.header.flags |= LIBC_LEFT;
            continue;
        }
        if (c == plus) {
            work.spec.header.flags |= LIBC_PLUS;
            continue;
        }
        if (c == ' ') {
            work.spec.header.bytes.prefix = c;
            continue;
        }
        if (c == hash) {
            work.spec.header.flags |= LIBC_ALT;
            continue;
        }
        if (c == zeroFlag) {
            work.spec.header.flags |= LIBC_ZERO;
            continue;
        }
        break;
        }

        if (c == '*') {
            work.spec.width = LIBC_NEXT_ARG(s32);
            if (work.spec.width < 0) {
                work.spec.width = -work.spec.width;
                work.spec.header.flags |= LIBC_LEFT;
            }
            c = *++format;
        } else {
            while ((u32)(c - '0') < 10) {
                work.spec.width =
                    work.spec.width * 10 - ('0' - c);
                c = *++format;
            }
        }

        if (c == '.') {
            c = *++format;
            if (c == '*') {
                work.spec.precision = LIBC_NEXT_ARG(s32);
                c = *++format;
            } else {
                while ((u32)(c - '0') < 10) {
                    work.spec.precision =
                        work.spec.precision * 10 - ('0' - c);
                    c = *++format;
                }
            }
            if (work.spec.precision >= 0) {
                work.spec.header.flags |= LIBC_PRECISION;
            }
        }

        src = (u8 *)&work.spec;
        if (work.spec.header.flags & LIBC_LEFT) {
            work.spec.header.flags &= ~LIBC_ZERO;
        }

conversion:
        switch (c) {
        case 'h':
            work.spec.header.flags |= LIBC_SHORT;
            c = *++format;
            goto conversion;

        case 'l':
            work.spec.header.flags |= LIBC_LONG;
            c = *++format;
            goto conversion;

        case 'L':
            work.spec.header.flags |= LIBC_LDOUBLE;
            c = *++format;
            goto conversion;

        case 'd':
        case 'i':
            value = LIBC_NEXT_NUM_ARG(s32);
            asm("" : "=r"(value) : "0"(value));
            if (work.spec.header.flags & LIBC_SHORT) {
                value = (s16)value;
            }
            if ((s32)value < 0) {
                value = -value;
                work.spec.header.bytes.prefix = minus;
            } else if (work.spec.header.flags & LIBC_PLUS) {
                work.spec.header.bytes.prefix = plus;
            }
            goto decimal;

        case 'u':
            value = LIBC_NEXT_NUM_ARG(u32);
            asm("" : "=r"(value) : "0"(value));
            if (work.spec.header.flags & LIBC_SHORT) {
                value &= 0xFFFF;
            }
            work.spec.header.bytes.prefix = 0;

decimal:
            if (!(work.spec.header.flags & LIBC_PRECISION)) {
                if (work.spec.header.flags & LIBC_ZERO) {
                    work.spec.precision = work.spec.width;
                    if (work.spec.header.bytes.prefix != 0) {
                        work.spec.precision--;
                    }
                }
                if (work.spec.precision <= 0) {
                    work.spec.precision = 1;
                }
            }
            count = 0;
            while (value != 0) {
                *--src = value % 10 + '0';
                value /= 10;
                count++;
            }
            while (count < work.spec.precision) {
                *--src = LIBC_LOCAL_ZERO;
                count++;
            }
            if (work.spec.header.bytes.prefix != 0) {
                *--src = work.spec.header.bytes.prefix;
                count++;
            }
            break;

        case 'o':
            value = LIBC_NEXT_NUM_ARG(u32);
            asm("" : "=r"(value) : "0"(value));
            if (work.spec.header.flags & LIBC_SHORT) {
                value &= 0xFFFF;
            }
            if (!(work.spec.header.flags & LIBC_PRECISION)) {
                if (work.spec.header.flags & LIBC_ZERO) {
                    work.spec.precision = work.spec.width;
                }
                if (work.spec.precision <= 0) {
                    work.spec.precision = 1;
                }
            }
            count = 0;
            while (value != 0) {
                *--src = (value % 8) + '0';
                value >>= 3;
                count++;
            }
            if ((work.spec.header.flags & LIBC_ALT) &&
                count != 0 && *src != '0') {
                *--src = '0';
                count++;
            }
            while (count < work.spec.precision) {
                *--src = LIBC_LOCAL_ZERO;
                count++;
            }
            break;

        case 'p':
            work.spec.precision = 8;
            work.spec.header.flags |= LIBC_PRECISION | LIBC_LONG;

        case 'X':
            digits = g_LibcUpperDigits;
            goto hexadecimal;

        case 'x':
            digits = g_LibcLowerDigits;

hexadecimal:
            value = LIBC_NEXT_NUM_ARG(u32);
            asm("" : "=r"(value) : "0"(value));
            if (work.spec.header.flags & LIBC_SHORT) {
                value &= 0xFFFF;
            }
            if (!(work.spec.header.flags & LIBC_PRECISION)) {
                if (work.spec.header.flags & LIBC_ZERO) {
                    work.spec.precision = work.spec.width;
                    if (work.spec.header.flags & LIBC_ALT) {
                        work.spec.precision -= 2;
                    }
                }
                if (work.spec.precision <= 0) {
                    work.spec.precision = 1;
                }
            }
            count = 0;
            while (value != 0) {
                *--src = digits[value % 16];
                value >>= 4;
                count++;
            }
            while (count < work.spec.precision) {
                *--src = LIBC_LOCAL_ZERO;
                count++;
            }
            if (work.spec.header.flags & LIBC_ALT) {
                *--src = c;
                *--src = LIBC_LOCAL_ZERO;
                count += 2;
            }
            break;

        case 'c':
            *--src = LIBC_NEXT_ARG(s32);
            count = 1;
            break;

        case 's':
            src = LIBC_NEXT_ARG(u8 *);
            if (work.spec.header.flags & LIBC_ALT) {
                count = *src++;
                if ((work.spec.header.flags & LIBC_PRECISION) &&
                    work.spec.precision < count) {
                    count = work.spec.precision;
                }
            } else if (!(work.spec.header.flags & LIBC_PRECISION)) {
                count = strlen(src);
            } else {
                end = memchr(src, 0, work.spec.precision);
                if (end != 0) {
                    count = end - src;
                } else {
                    count = work.spec.precision;
                }
            }
            break;

        case 'n':
            src = LIBC_NEXT_ARG(u8 *);
            nflags = work.spec.header.flags;
            if (nflags & LIBC_SHORT) {
                *(s16 *)src = length;
            } else {
                asm("" : : "r"(nflags));
                *(s32 *)src = length;
            }
            continue;

        default:
            if (c != '%') {
                goto finished;
            }
            dest[length++] = c;
            continue;
        }

        if (count < work.spec.width &&
            !(work.spec.header.flags & LIBC_LEFT)) {
            while (count < work.spec.width) {
                dest[length++] = ' ';
                work.spec.width--;
            }
        }

        memmove(dest + length, src, count);
        length += count;

        if (count < work.spec.width) {
            while (count < work.spec.width) {
                dest[length++] = ' ';
                count++;
            }
        }

    } while (format++, (c = *format) != 0);

finished:
    dest[length] = 0;
    return length;
#undef args
}
