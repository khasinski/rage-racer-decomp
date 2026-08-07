#include "common.h"
#include "psyq/cd.h"
#include "psyq/kernel.h"

void CD_datasync(void);

/* Both of these forward their own incoming arguments untouched, which is why
 * m2c saw them as (void): the values are already in a0/a1 when the tail call
 * is made. The signatures below are the ones the callers use -- CdMix from
 * cd/cd_audio_control.c and CdGetSector2 from sdk/CdReadDataReadyCallback.c. */
long CdMix(CdlATV *vol) {
    CD_vol(vol);
    return 1;
}

long CdGetSector2(long madr, u_long size) {
    return CD_getsector2(madr, size) == 0;
}

void CdDataCallback(long arg0) {
    DMACallback(3, arg0);
}

void CdMixDataSync(void) {
    CD_datasync();
}

CdlLOC *CdIntToPos(long i, CdlLOC *p) {
    inline long ENCODE_BCD(n) { return (((n / 10) << 4) + (n % 10)); };

    i += 150;
    p->sector = ENCODE_BCD(i % 75);
    p->second = ENCODE_BCD(i / 75 % 60);
    p->minute = ENCODE_BCD(i / 75 / 60);
    return p;
}

long CdPosToInt_Local(CdlLOC *arg0) {
    u_char sector = arg0->sector;
    u_char second = arg0->second;
    u_char minute = arg0->minute;

#define DECODE_BCD(x) (((x) >> 4) * 10 + ((x) & 0xF))
    return (DECODE_BCD(minute) * 60 + DECODE_BCD(second)) * 75 +
           DECODE_BCD(sector) - 150;
#undef DECODE_BCD
}
