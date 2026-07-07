#include "common.h"
#include "game/asset.h"
#include "psyq/cd.h"

extern char D_80010ADC[];
extern char D_80010B38[];
extern char D_80010B44[];
extern char D_80010B58[];
extern char D_80010B68[];
extern char D_80010B80[];
extern char D_80010B8C[];
extern GameCdLoadEntry D_801E6834[];
extern GameCdLoadEntry D_8007C6A8[];
extern s32 D_8009F0B8[];

void func_8001674C(char *fmt, ...);
s32 func_8006A5A4(s32 arg0, void *arg1, s32 arg2);
s32 func_80027688(s32 arg0, void *arg1, s32 arg2);
s32 func_80027790(s32 arg0, s32 arg1);

void func_80017E8C(void) {
    struct {
        CdlLOC file;
        u8 pad[20];
    } stack;
    register s32 one asm("$16");
    s32 base;
    s32 i;
    s32 status;
    s32 *src;
    GameCdLoadEntry *dst;
    GameCdLoadEntry *smallSrc;

    func_8001674C(D_80010ADC, D_80010B38, D_8009F0B8);
    if (DsSearchFile(&stack.file, D_80010B38) == 0) {
        func_8001674C(D_80010B44, D_80010B38);
    }

    one = 1;
    do {
        func_8006A5A4(2, &stack.file, 0);
        func_80027688(one, D_8009F0B8, 0x80);
        do {
            status = func_80027790(1, 0);
        } while (status > 0);
    } while (status != 0);

    func_8001674C(D_80010B58, one);
    base = CdPosToInt_Local(&stack.file);
    src = D_8009F0B8;
    dst = D_801E6834;
    for (i = 0; i < 135; i++) {
        CdIntToPos(base + *src, (CdlLOC *)dst);
        dst->size = src[1];
        src += 2;
        dst++;
    }

    func_8001674C(D_80010B68, D_80010B80);
    if (DsSearchFile(&stack.file, D_80010B80) == 0) {
        func_8001674C(D_80010B44, D_80010B80);
        goto loadSmallTable;
    }

    func_8001674C(D_80010B8C);
    asm(""); /* Match note: target keeps the debug-print call delay slot as nop. */
loadSmallTable:
    base = CdPosToInt_Local(&stack.file);
    smallSrc = D_8007C6A8;
    for (i = 0; i < 11; i++) {
        CdIntToPos(base + smallSrc->position, (CdlLOC *)smallSrc);
        smallSrc++;
    }
}
