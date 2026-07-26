#include "common.h"
#include "psyq/cd.h"

s32 CdControl(s32 com, void *param, s32 result) asm("func_8006A5A4");
void CdRead(s32 arg0, void *arg1, s32 arg2) asm("func_80027688");
s32 CdReadSync(s32 arg0, s32 arg1) asm("func_80027790");

/*
 * Reads one 0x80-word directory/data sector at logical sector `sectorInt` into
 * `buf`: converts the sector index to a CdlLOC, issues CD command 2 (seek/read),
 * DMAs 0x80 words via CdRead, and returns whether the read completed OK.
 */
s32 func_8006CB88(s32 arg0, s32 sectorInt, void *buf) {
    s32 scratch[2];

    CdIntToPos(sectorInt, (CdlLOC *)scratch);
    CdControl(2, scratch, 0);
    CdRead(arg0, buf, 0x80);
    return CdReadSync(0, 0) < 1U;
}
