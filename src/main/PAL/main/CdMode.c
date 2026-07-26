#include "common.h"
#include "psyq/cd.h"

extern CdlLOC D_80099058;
extern u8 D_8009905C;
extern u8 D_8009905D;

/* CdMode: returns the last CD mode byte. */
u8 func_8006A3F8(void) {
    return D_8009905C;
}

/* CdLastCom: returns the last CD command byte issued. */
u8 func_8006A408(void) {
    return D_8009905D;
}

/* CdLastPos: returns the last reported disc position. */
CdlLOC *func_8006A418(void) {
    return &D_80099058;
}
