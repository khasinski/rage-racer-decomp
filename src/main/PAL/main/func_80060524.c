#include "common.h"
#include "game/memcard.h"
#include "psyq/kernel.h"

extern s32 D_8009B740;

void func_8005F88C(void *arg0);

s32 GameWriteMemoryCardSaveFile(
    char *path,
    char *title,
    void *iconBlock,
    GameSaveHeaderRow *header,
    void *saveBlock) {
    register s32 fd asm("$16");
    register s32 saved asm("$18");
    s32 attempt;
    s32 ok;

    D_8009B740 = 0x1100;
    GameBuildSaveIconBlock(iconBlock, title, 0x222, 0x3C0, 0x1F0);
    D_8009B740 = 0x1200;
    GameWriteSaveHeaderRow(header);
    attempt = 0;
    D_8009B740 = 0x1300;
    func_8005F88C(saveBlock);
    D_8009B740 = 0x1500;

    do {
        fd = BiosFileOpen(path, 2);
        saved = fd;
        if (fd == -1) {
            fd = BiosFileOpen(path, 0x10200);
            if (fd == saved) {
                D_8009B740 = attempt | 0x1520;
            } else {
                BiosFileClose(fd);
                fd = BiosFileOpen(path, 2);
                if (fd == saved) {
                    D_8009B740 = attempt | 0x1510;
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
    D_8009B740 = attempt | 0x1530;
    if (BiosFileWrite(fd, iconBlock, 0x200) != 0x200) {
        return 0;
    }
    D_8009B740 = attempt | 0x1540;
    saved = BiosFileWrite(fd, header, 0x80);
    if (saved != 0x80) {
        return 0;
    }
    asm volatile("" : "=r"(saved) : "0"(saved));
    D_8009B740 = attempt | 0x1550;
    if (BiosFileWrite(fd, saveBlock, 0x1000) != 0x1000) {
        return 0;
    }
    D_8009B740 = attempt | 0x1560;
    if (BiosFileWrite(fd, header, 0x80) != saved) {
        return 0;
    }
    D_8009B740 = attempt | 0x1570;
    BiosFileClose(fd);
    return 1;
}
