#include "common.h"
#include "psyq/cd.h"

long CdControl(long com, void *param, long result);
void CdRead(long arg0, void *arg1, long arg2);
long CdReadSync(long arg0, long arg1);

/*
 * Reads one 0x80-word directory/data sector at logical sector `sectorInt` into
 * `buf`: converts the sector index to a CdlLOC, issues CD command 2 (seek/read),
 * DMAs 0x80 words via CdRead, and returns whether the read completed OK.
 */
/* libcd's static cd_read, named from CD_newmedia's own error text
 * "Read error in cd_read(PVD)"; CD_newmedia and CD_cachefile are its
 * only callers. */
long cd_read(long count, long sectorInt, void *buf);
long cd_read(long arg0, long sectorInt, void *buf) {
    long scratch[2];

    CdIntToPos(sectorInt, (CdlLOC *)scratch);
    CdControl(2, scratch, 0);
    CdRead(arg0, buf, 0x80);
    return CdReadSync(0, 0) < 1U;
}

void LibcMemcpy(u_char *dst, u_char *src, long count) {
    u_char scratch[8];
    register long i asm("$3");
    long end;

    /* Forces the target's otherwise unused 8-byte leaf frame. */
    asm volatile("" : "=m"(scratch));

    if (count == 0) {
        return;
    }

    i = count - 1;
    end = -1;
    do {
        *dst = *src;
        src++;
        i--;
        dst++;
    } while (i != end);
}

long LibcStrcmp(u_char *arg0, u_char *arg1) {
    long left;

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

long LibcStrncmp(u_char *arg0, u_char *arg1, long arg2) {
    long left;
    long right;

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

for (;;) {
    left = *arg0;
    right = *arg1++;
    if (left != right) {
    } else {
    arg0++;
    if (left == 0) {
        return 0;
    }
    arg2--;
    if (arg2 >= 0) {
        continue;
    }

    }
break;
}
    if (arg2 < 0) {
        return 0;
    }
    return *arg0 - arg1[-1];
}
