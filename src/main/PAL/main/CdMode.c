#include "common.h"
#include "psyq/cd.h"

extern CdlLOC D_80099058;
extern u8 D_8009905C;
extern u8 D_8009905D;

u8 CdMode(void) asm("func_8006A3F8");
u8 CdMode(void) {
    return D_8009905C;
}

u8 CdLastCom(void) asm("func_8006A408");
u8 CdLastCom(void) {
    return D_8009905D;
}

CdlLOC *CdLastPos(void) asm("func_8006A418");
CdlLOC *CdLastPos(void) {
    return &D_80099058;
}
