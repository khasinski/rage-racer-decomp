#include "common.h"
#include "game/memcard.h"
#include "psyq/kernel.h"
#include "game/menu.h"

void func_8005F88C(void *arg0);

s32 GameWriteMemoryCardSaveFile(
    char *path,
    char *title,
    void *iconBlock,
    GameSaveHeaderRow *header,
    void *saveBlock) {
    s32 fd;
    /* This pin is load-bearing: removing it changes .text. */
    register s32 saved asm("$18");
    s32 attempt;
    s32 ok;

    GameMenuLoadPhase = 0x1100;
    GameBuildSaveIconBlock(iconBlock, title, 0x222, 0x3C0, 0x1F0);
    GameMenuLoadPhase = 0x1200;
    GameWriteSaveHeaderRow(header);
    attempt = 0;
    GameMenuLoadPhase = 0x1300;
    func_8005F88C(saveBlock);
    GameMenuLoadPhase = 0x1500;

    do {
        fd = BiosFileOpen(path, 2);
        saved = fd;
        if (fd == -1) {
            fd = BiosFileOpen(path, 0x10200);
            if (fd == saved) {
                GameMenuLoadPhase = attempt | 0x1520;
            } else {
                BiosFileClose(fd);
                fd = BiosFileOpen(path, 2);
                if (fd == saved) {
                    GameMenuLoadPhase = attempt | 0x1510;
                }
            }
            ok = 0;
        } else {
            ok = 1;
        }

        if (ok) {
            goto body;
        }
        attempt++;
    } while (attempt < 2);

    return 0;

body:
    GameMenuLoadPhase = attempt | 0x1530;
    if (BiosFileWrite(fd, iconBlock, 0x200) != 0x200) {
        return 0;
    }
    GameMenuLoadPhase = attempt | 0x1540;
    saved = BiosFileWrite(fd, header, 0x80);
    if (saved != 0x80) {
        return 0;
    }
    asm volatile("" : "=r"(saved) : "0"(saved));
    GameMenuLoadPhase = attempt | 0x1550;
    if (BiosFileWrite(fd, saveBlock, 0x1000) != 0x1000) {
        return 0;
    }
    GameMenuLoadPhase = attempt | 0x1560;
    if (BiosFileWrite(fd, header, 0x80) != saved) {
        return 0;
    }
    GameMenuLoadPhase = attempt | 0x1570;
    BiosFileClose(fd);
    return 1;
}

extern char g_SaveFilePath[] asm("D_800128AC");
extern char g_SaveTitleSjis[] asm("D_800127D8");

s32 GameWriteMemoryCardSaveSlot(s32 arg0, GameSaveHeaderRow *arg1) {
    u8 block0[0x200];
    u8 block1[0x1000];
    s32 i;

    for (i = 0x1FF; i >= 0; i--) {
        block0[i] = 0;
    }

    GameMenuLoadPhase = 0x1000;
    return GameWriteMemoryCardSaveFile(
        g_SaveFilePath + arg0 * 0x1A,
        g_SaveTitleSjis + arg0 * 0x46,
        block0,
        arg1,
        block1);
}

s32 GameReadVerifiedSaveHeader(s32 arg0, GameSaveHeaderRow *arg1) {
    s32 fd;
    /* These pins are load-bearing: removing any one changes .text. */
    register void *buffer asm("$18");
    register s32 sum asm("$16");
    s32 i;
    register u16 *ptr asm("$4");

    fd = arg0;
    buffer = arg1;
    /* This barrier is load-bearing: removing it changes .text. */
    asm("" : "=r"(sum) : "r"(fd), "r"(buffer), "0"(0));

    GameMenuLoadPhase = 0x120;
    if (BiosFileSeek(fd, 0x1280, 0) < 0) {
        return 0;
    }

    GameMenuLoadPhase = 0x130;
    if (BiosFileRead(fd, buffer, 0x80) != 0x80) {
        return 0;
    }

    GameMenuLoadPhase = 0x140;
    i = 0;
    ptr = buffer;
    do {
        sum += *ptr++;
        i++;
    } while ((u32)i < 0x3E);

    if (*(s32 *)((u8 *)buffer + 0x7C) == ~sum) {
        return 1;
    }

    GameMenuLoadPhase = 0x150;
    if (BiosFileSeek(fd, 0x200, 0) < 0) {
        return 0;
    }

    GameMenuLoadPhase = 0x160;
    if (BiosFileRead(fd, buffer, 0x80) != 0x80) {
        return 0;
    }

    GameMenuLoadPhase = 0x170;
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

    GameMenuLoadPhase = 0x180;
    return 0;
}

s32 GameScanMemoryCardSaveHeaders(GameSaveHeaderRow *arg0) {
    s32 fd;
    s32 i;
    /* This pin is load-bearing: removing it changes .text. */
    register s32 mask asm("$18");
    s32 nameOffset;
    void *buffer;
    s32 bit;

    mask = 0;
    GameMenuLoadPhase = 0x110;
    i = 0;
    buffer = arg0;
    nameOffset = 0;

    do {
        fd = BiosFileOpen(g_SaveFilePath + nameOffset, 1);
        if (fd >= 0) {
            if (GameReadVerifiedSaveHeader(fd, buffer) == 0) {
                BiosFileClose(fd);
                bit = 0x10000;
            } else {
                BiosFileClose(fd);
                bit = 1;
            }
            mask |= bit << i;
        }

        buffer = (void *)((u8 *)buffer + 0x80);
        i++;
        nameOffset += 0x1A;
    } while (i < 3);

    GameMenuLoadPhase = 0x190;
    return mask;
}

extern volatile s32 GameMenuLoadPhase asm("D_8009B740");
extern s32 g_SaveElapsedTicks asm("D_801E7A54");

s32 GameLoadMemoryCardSaveSlot(s32 arg0, GameSaveHeaderRow *arg1) {
    u8 block[0x1000];
    void *header;
    s32 tries;
    s32 fd;
    s32 temp;
    s32 i;

    header = arg1;
    GameMenuLoadPhase = 0x3000;
    tries = 0;
    temp = arg0 << 1;
    temp += arg0;
    temp <<= 2;
    temp += arg0;

    {
        s32 nameOffset = temp << 1;
        /* This pin is load-bearing: removing it changes .text. */
        register char *name asm("$4");

        do {
            name = g_SaveFilePath;
            asm volatile("" : "=r"(name) : "0"(name));
            name = (char *)(nameOffset + (s32)name);
            fd = BiosFileOpen(name, 1);
            if (fd >= 0) {
                break;
            }
            tries++;
        } while (tries < 2);
    }

    GameMenuLoadPhase = tries | 0x3100;

    if (fd < 0) {
        return 0;
    }

    GameMenuLoadPhase = 0x3300;
    if (GameReadVerifiedSaveHeader(fd, header) == 0) {
        return 0;
    }

    GameMenuLoadPhase = 0x3500;
    if (BiosFileSeek(fd, 0x280, 0) < 0) {
        return 0;
    }

    GameMenuLoadPhase = 0x3600;
    if (BiosFileRead(fd, block, 0x1000) != 0x1000) {
        return 0;
    }

    BiosFileClose(fd);
    GameMenuLoadPhase = 0x3700;
    if (GameLoadSaveStateBlock(block) == 0) {
        return 0;
    }

    GameMenuLoadPhase = 0x3800;
    g_TeamNameLength = *(u8 *)header;
    i = 0;
    do {
        u8 *copy_src = (u8 *)header + i;
        g_TeamNameChars[i] = copy_src[1];
        i++;
    } while (i < 7);

    {
        s32 one = 1;
        s32 word;
        s32 status;

        word = *(s32 *)((u8 *)header + 8);
        status = tries | 0x3900;
        GameMenuLoadPhase = status;
        g_SaveElapsedTicks = word;
        return one;
    }
}

extern char g_FmtCardWildcard[] asm("D_80012FAC");
extern char g_McDirEntries[] asm("D_8009B748");

void LibcSprintf(char *dst, char *fmt, s32 arg0, s32 arg1) asm("func_800632F0");

s32 GameCountMemoryCardFiles(s32 arg0, s32 arg1) {
    char path[0x20];
    void *entry;
    void *ret;
    s32 count;

    count = 0;
    LibcSprintf(path, g_FmtCardWildcard, arg0, arg1);
    entry = g_McDirEntries;

    if (BiosFirstFile(path, entry) == entry) {
        /* This barrier is load-bearing: removing it changes .text. */
        asm("" : "=r"(count) : "0"(count));
        count++;
        do {
            entry = (char *)entry + 0x28;
            ret = BiosNextFile(entry);
            count++;
        } while (ret == entry);
        count--;
    }

    return count;
}
