#include "common.h"
#include "game/memcard.h"
#include "psyq/kernel.h"
#include "game/menu.h"

extern volatile s32 D_8009B740;
extern char D_800128AC[];
extern s32 D_801E7A54;

s32 GameLoadMemoryCardSaveSlot(s32 arg0, GameSaveHeaderRow *arg1) {
    u8 block[0x1000];
    register void *header asm("$19");
    register s32 tries asm("$17");
    register s32 fd asm("$16");
    register s32 temp asm("$2");
    s32 i;

    header = arg1;
    D_8009B740 = 0x3000;
    tries = 0;
    temp = arg0 << 1;
    temp += arg0;
    temp <<= 2;
    temp += arg0;

    {
        register s32 nameOffset asm("$18") = temp << 1;
        register char *name asm("$4");

        do {
            name = D_800128AC;
            asm volatile("" : "=r"(name) : "0"(name));
            name = (char *)(nameOffset + (s32)name);
            fd = BiosFileOpen(name, 1);
            if (fd >= 0) {
                break;
            }
            tries++;
        } while (tries < 2);
    }

    D_8009B740 = tries | 0x3100;

    if (fd < 0) {
        return 0;
    }

    D_8009B740 = 0x3300;
    if (GameReadVerifiedSaveHeader(fd, header) == 0) {
        return 0;
    }

    D_8009B740 = 0x3500;
    if (BiosFileSeek(fd, 0x280, 0) < 0) {
        return 0;
    }

    D_8009B740 = 0x3600;
    if (BiosFileRead(fd, block, 0x1000) != 0x1000) {
        return 0;
    }

    BiosFileClose(fd);
    D_8009B740 = 0x3700;
    if (GameLoadSaveStateBlock(block) == 0) {
        return 0;
    }

    D_8009B740 = 0x3800;
    g_TeamNameLength = *(u8 *)header;
    i = 0;
    do {
        register u8 *copy_src asm("$2") = (u8 *)header + i;
        g_TeamNameChars[i] = copy_src[1];
        i++;
    } while (i < 7);

    {
        register s32 one asm("$2") = 1;
        register s32 word asm("$4");
        register s32 status asm("$3");

        asm volatile("" : "=r"(one) : "0"(one));
        word = *(s32 *)((u8 *)header + 8);
        status = tries | 0x3900;
        D_8009B740 = status;
        D_801E7A54 = word;
        return one;
    }
}
