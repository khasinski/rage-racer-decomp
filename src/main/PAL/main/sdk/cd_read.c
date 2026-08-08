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
