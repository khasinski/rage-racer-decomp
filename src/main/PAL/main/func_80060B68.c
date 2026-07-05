#include "common.h"
#include "game/memcard.h"
#include "psyq/kernel.h"

extern char D_80012FAC[];
extern char D_8009B748[];

void Square_Vsprintf(char *dst, char *fmt, s32 arg0, s32 arg1) asm("func_800632F0");

s32 GameCountMemoryCardFiles(s32 arg0, s32 arg1) {
    char path[0x20];
    void *entry;
    void *ret;
    s32 count;

    count = 0;
    Square_Vsprintf(path, D_80012FAC, arg0, arg1);
    entry = D_8009B748;

    if (BiosFirstFile(path, entry) == entry) {
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
