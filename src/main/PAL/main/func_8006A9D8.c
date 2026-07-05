#include "common.h"
#include "psyq/cd.h"

CdlLOC *CdIntToPos(s32 i, CdlLOC *p) {
    inline int ENCODE_BCD(n) { return (((n / 10) << 4) + (n % 10)); };

    i += 150;
    p->sector = ENCODE_BCD(i % 75);
    p->second = ENCODE_BCD(i / 75 % 60);
    p->minute = ENCODE_BCD(i / 75 / 60);
    return p;
}
