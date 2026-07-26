#include "common.h"
#include "psyq/cd.h"

s32 CdControl(s32 com, void *param, s32 result) asm("func_8006A5A4");
void func_80027688(s32 arg0, void *arg1, s32 arg2);
s32 func_80027790(s32 arg0, s32 arg1);

/*
 * Reads one 0x80-word directory/data sector at logical sector `sectorInt` into
 * `buf`: converts the sector index to a CdlLOC, issues CD command 2 (seek/read),
 * DMAs 0x80 words via func_80027688, and returns whether the read completed OK.
 */
s32 func_8006CB88(s32 arg0, s32 sectorInt, void *buf) {
    s32 scratch[2];

    CdIntToPos(sectorInt, (CdlLOC *)scratch);
    CdControl(2, scratch, 0);
    func_80027688(arg0, buf, 0x80);
    return func_80027790(0, 0) < 1U;
}
