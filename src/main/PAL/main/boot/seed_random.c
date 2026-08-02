#include "common.h"
#include "game/random.h"
#include "psyq/kernel.h"

u8 *LibcMemchr(u8 *arg0, s32 arg1, s32 arg2) asm("func_80063B4C");
void *LibcMemmove(u8 *dest, u8 *src, s32 count) asm("func_80063B9C");
s32 LibcStrlen(u8 *arg0) asm("func_80063C08");

extern s32 g_RandomSeed asm("D_8009B9A8");

void SeedRandom(s32 seed) {
    g_RandomSeed = seed;
}

/*
 * PSY-Q libc sprintf: the whole formatter, with no vsprintf split. Identified by
 * its %X / %x digit tables at D_800131E4 / D_800131F8 and by its LibcMemchr /
 * LibcMemmove / LibcStrlen callees.
 */
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

typedef struct LibcFormatWork {
    u8 number[0x200];
    LibcFormatSpec spec;
} LibcFormatWork;

extern LibcFormatSpec g_LibcDefaultFormat asm("D_80082FCC");
extern u8 g_LibcUpperDigits[] asm("D_800131E4");
extern u8 g_LibcLowerDigits[] asm("D_800131F8");

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
    register u8 *oldArgs asm("$3") = args; \
    args = oldArgs + 4; \
    *(type *)oldArgs; \
})
#define LIBC_LOCAL_ZERO ({ s32 zero = '0'; zero; })

s32 func_800632F0(u8 *dest, u8 *format, ...) {
    LibcFormatWork work;
    u8 *argState[2];
#define args argState[0]
    u8 *src;
    u8 *end;
    u8 *digits;
    u32 value;
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
            goto copy_character;
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
                *--src = (value & 7) + '0';
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
                *--src = digits[value & 0xF];
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
                count = LibcStrlen(src);
            } else {
                end = LibcMemchr(src, 0, work.spec.precision);
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
copy_character:
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

        LibcMemmove(dest + length, src, count);
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

u8 * LibcMemchr(u8 *arg0, s32 arg1, s32 arg2) asm("func_80063B4C");
u8 *LibcMemchr(u8 *arg0, s32 arg1, s32 arg2) {
    switch (0) { default:
    if (arg0 == 0) {
        return 0;
    }
    if (arg2 <= 0) {
        return 0;
    }

    break;

found:
    return arg0 - 1;

    }

    arg2--;
    if (arg2 < 0) {
        return 0;
    }

    arg1 &= 0xFF;
    do {
        if (*arg0++ == arg1) {
            goto found;
        }
        arg2--;
    } while (arg2 >= 0);

    return 0;
}

void * LibcMemmove(u8 *dest, u8 *src, s32 count) asm("func_80063B9C");
void *LibcMemmove(u8 *dest, u8 *src, s32 count) {
    if (dest >= src) {
        while (count-- > 0) {
            dest[count] = src[count];
        }
    } else {
        while (count-- > 0) {
            *dest++ = *src++;
        }
    }

    return dest;
}

s32 LibcStrlen(u8 *arg0) asm("func_80063C08");
s32 LibcStrlen(u8 *arg0) {
    s32 count = 0;

    if (arg0 == 0) {
        return 0;
    }

    while (*arg0++ != 0) {
        count++;
    }

    return count;
}

extern u8 g_LibcNullText[] asm("D_800132C0");

void func_80063C88(s32 arg0);

void LibcPutString(u8 *arg0) asm("func_80063C38");
void LibcPutString(u8 *arg0) {
    u8 *ptr;
    s32 value;

    ptr = arg0;
    if (ptr == 0) {
        ptr = g_LibcNullText;
    }

    while (value = *ptr++, value != 0) {
        func_80063C88(value);
    }
}

extern u8 g_LibcCtype[] asm("D_80082FD9");
extern s32 g_LibcOutColumn asm("D_8009B9AC");

void LibcPutChar(s32 arg0) asm("func_80063C88");
void LibcPutChar(s32 arg0) {
    u8 c;
    s32 value = arg0;

    c = value;
    switch (0) { default:
    value = c;
    if (!(value == 9)) {
    if (value != 10) {
    } else {

    LibcPutChar(13);
    g_LibcOutColumn = 0;
    break;

    }
    } else {
    do {
        LibcPutChar(0x20);
    } while ((g_LibcOutColumn & 7) != 0);
    return;
    }

    if (g_LibcCtype[value] & 0x97) {
        g_LibcOutColumn++;
    }

    }
    BiosFileWrite(1, &c, 1);
}

s32 LibcToUpper(s32 arg0) asm("func_80063D3C");
s32 LibcToUpper(s32 arg0) {
    u8 value = arg0;

    if (g_LibcCtype[value & 0xFF] & 2) {
        value = arg0 - 0x20;
    }

    return value & 0xFF;
}
