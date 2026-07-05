#include "common.h"
#include "game/memcard.h"
#include "psyq/kernel.h"

extern s32 D_8009B740;

s32 GameReadVerifiedSaveHeader(s32 arg0, GameSaveHeaderRow *arg1) {
    register s32 fd asm("$17");
    register void *buffer asm("$18");
    register s32 sum asm("$16");
    register s32 i asm("$3");
    register u16 *ptr asm("$4");

    fd = arg0;
    buffer = arg1;
    asm("" : "=r"(sum) : "r"(fd), "r"(buffer), "0"(0));

    D_8009B740 = 0x120;
    if (BiosFileSeek(fd, 0x1280, 0) < 0) {
        return 0;
    }

    D_8009B740 = 0x130;
    if (BiosFileRead(fd, buffer, 0x80) != 0x80) {
        return 0;
    }

    D_8009B740 = 0x140;
    i = 0;
    ptr = buffer;
    do {
        sum += *ptr++;
        i++;
    } while ((u32)i < 0x3E);

    if (*(s32 *)((u8 *)buffer + 0x7C) == ~sum) {
        return 1;
    }

    D_8009B740 = 0x150;
    if (BiosFileSeek(fd, 0x200, 0) < 0) {
        return 0;
    }

    D_8009B740 = 0x160;
    if (BiosFileRead(fd, buffer, 0x80) != 0x80) {
        return 0;
    }

    D_8009B740 = 0x170;
    sum = 0;
    i = 0;
    ptr = buffer;
    do {
        sum += *ptr++;
        i++;
    } while ((u32)i < 0x3E);

    if (*(s32 *)((u8 *)buffer + 0x7C) == ~sum) {
        return 1;
    }

    D_8009B740 = 0x180;
    return 0;
}
