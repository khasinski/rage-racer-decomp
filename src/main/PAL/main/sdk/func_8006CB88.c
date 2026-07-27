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

void LibcMemcpy(u8 *dst, u8 *src, s32 count) asm("func_8006CBF4");
void LibcMemcpy(u8 *dst, u8 *src, s32 count) {
    u8 scratch[8];
    register s32 i asm("$3");
    register s32 end asm("$6");

    /* Forces the target's otherwise unused 8-byte leaf frame. */
    asm volatile("" : "=m"(scratch));

    if (count == 0) {
        return;
    }

    i = count - 1;
    end = -1;
loop:
    *dst = *src;
    src++;
    i--;
    dst++;
    if (i != end) {
        goto loop;
    }
}

s32 LibcStrcmp(u8 *arg0, u8 *arg1) asm("func_8006CC28");
s32 LibcStrcmp(u8 *arg0, u8 *arg1) {
    s32 left;

    if ((arg0 == 0) || (arg1 == 0)) {
        if (arg0 == arg1) {
            return 0;
        }
        if (arg0 == 0) {
            return -1;
        }
        return 1;
    }

    while ((left = *arg0) == *arg1++) {
        if (left == 0) {
            return 0;
        }
        arg0++;
    }

    return *arg0 - arg1[-1];
}

s32 LibcStrncmp(u8 *arg0, u8 *arg1, s32 arg2) asm("func_8006CC8C");
s32 LibcStrncmp(u8 *arg0, u8 *arg1, s32 arg2) {
    s32 left;
    s32 right;

    if ((arg0 == 0) || (arg1 == 0)) {
        if (arg0 == arg1) {
            return 0;
        }
        if (arg0 == 0) {
            return -1;
        }
        return 1;
    }

    arg2--;
    if (arg2 < 0) {
        return 0;
    }

loop:
    left = *arg0;
    right = *arg1++;
    if (left != right) {
        goto mismatch;
    }
    arg0++;
    if (left == 0) {
        return 0;
    }
    arg2--;
    if (arg2 >= 0) {
        goto loop;
    }

mismatch:
    if (arg2 < 0) {
        return 0;
    }
    return *arg0 - arg1[-1];
}
