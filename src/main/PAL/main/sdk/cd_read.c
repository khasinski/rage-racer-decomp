#include "common.h"
#include "psyq/cd.h"

/*
 * Reads one 0x80-word directory/data sector at logical sector `sectorInt` into
 * `buf`: converts the sector index to a CdlLOC, issues CD command 2 (seek/read),
 * DMAs 0x80 words via CdRead, and returns whether the read completed OK.
 */
/* libcd's static cd_read, named from CD_newmedia's own error text
 * "Read error in cd_read(PVD)"; CD_newmedia and CD_cachefile are its
 * only callers. */
long cd_read(long sectors, long sectorInt, void *buf) {
    long scratch[2];

    CdIntToPos(sectorInt, (CdlLOC *)scratch);
    CdControl(2, scratch, 0);
    CdRead(sectors, buf, 0x80);
    return CdReadSync(0, 0) < 1U;
}

void LibcMemcpy(u_char *dst, u_char *src, long count) {
    long i;

    i = count;
    while (i--) {
        *dst = *src;
        src++;
        dst++;
    }
}

long LibcStrcmp(u_char *lhs, u_char *rhs) {
    long left;

    if ((lhs == 0) || (rhs == 0)) {
        if (lhs == rhs) {
            return 0;
        }
        if (lhs == 0) {
            return -1;
        }
        return 1;
    }

    while ((left = *lhs) == *rhs++) {
        if (left == 0) {
            return 0;
        }
        lhs++;
    }

    return *lhs - rhs[-1];
}

long LibcStrncmp(u_char *lhs, u_char *rhs, long len) {
    long left;
    long right;

    if ((lhs == 0) || (rhs == 0)) {
        if (lhs == rhs) {
            return 0;
        }
        if (lhs == 0) {
            return -1;
        }
        return 1;
    }

    len--;
    if (len < 0) {
        return 0;
    }

for (;;) {
    left = *lhs;
    right = *rhs++;
    if (left != right) {
    } else {
    lhs++;
    if (left == 0) {
        return 0;
    }
    len--;
    if (len >= 0) {
        continue;
    }

    }
break;
}
    if (len < 0) {
        return 0;
    }
    return *lhs - rhs[-1];
}
