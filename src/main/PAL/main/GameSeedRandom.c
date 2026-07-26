#include "common.h"
#include "game/random.h"
#include "psyq/kernel.h"

u8 *LibcMemchr(u8 *arg0, s32 arg1, s32 arg2);
void *LibcMemmove(u8 *dest, u8 *src, s32 count);
s32 LibcStrlen(u8 *arg0);

extern s32 D_8009B9A8;

void GameSeedRandom(s32 seed) {
    D_8009B9A8 = seed;
}

/*
 * PSY-Q libc sprintf: the whole formatter, with no vsprintf split. Identified by
 * its %X / %x digit tables at D_800131E4 / D_800131F8 and by its LibcMemchr /
 * LibcMemmove / LibcStrlen callees.
 */
INCLUDE_ASM("asm/PAL/main/nonmatchings/main/GameSeedRandom", func_800632F0);

u8 * LibcMemchr(u8 *arg0, s32 arg1, s32 arg2) asm("func_80063B4C");
u8 *LibcMemchr(u8 *arg0, s32 arg1, s32 arg2) {
    if (arg0 == 0) {
        return 0;
    }
    if (arg2 <= 0) {
        return 0;
    }

    goto start;

found:
    return arg0 - 1;

start:

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

extern u8 D_800132C0[];

void func_80063C88(s32 arg0);

void LibcPutString(u8 *arg0) asm("func_80063C38");
void LibcPutString(u8 *arg0) {
    u8 *ptr;
    s32 value;

    ptr = arg0;
    if (ptr == 0) {
        ptr = D_800132C0;
    }

    while (value = *ptr++, value != 0) {
        func_80063C88(value);
    }
}

extern u8 D_80082FD9[];
extern s32 D_8009B9AC;

void LibcPutChar(s32 arg0) asm("func_80063C88");
void LibcPutChar(s32 arg0) {
    u8 c;
    s32 value = arg0;

    c = value;
    value = c;
    if (value == 9) {
        goto tab;
    }
    if (value != 10) {
        goto other;
    }

    LibcPutChar(13);
    D_8009B9AC = 0;
    goto output;

tab:
    do {
        LibcPutChar(0x20);
    } while ((D_8009B9AC & 7) != 0);
    return;

other:
    if (D_80082FD9[value] & 0x97) {
        D_8009B9AC++;
    }

output:
    BiosFileWrite(1, &c, 1);
}

s32 LibcToUpper(s32 arg0) asm("func_80063D3C");
s32 LibcToUpper(s32 arg0) {
    u8 value = arg0;

    if (D_80082FD9[value & 0xFF] & 2) {
        value = arg0 - 0x20;
    }

    return value & 0xFF;
}
