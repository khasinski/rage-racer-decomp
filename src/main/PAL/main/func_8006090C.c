#include "common.h"
#include "game/memcard.h"
#include "psyq/kernel.h"

extern s32 D_8009B740;
extern char D_800128AC[];

s32 GameScanMemoryCardSaveHeaders(GameSaveHeaderRow *arg0) {
    register s32 fd asm("$16");
    register s32 i asm("$17");
    register s32 mask asm("$18");
    register s32 nameOffset asm("$19");
    register void *buffer asm("$20");
    s32 bit;

    mask = 0;
    D_8009B740 = 0x110;
    i = 0;
    buffer = arg0;
    nameOffset = 0;

    do {
        fd = BiosFileOpen(D_800128AC + nameOffset, 1);
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

    D_8009B740 = 0x190;
    return mask;
}
