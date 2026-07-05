#include "common.h"
#include "psyq/cd.h"

extern s32 D_8019C7A0;
extern u8 D_8009DF14[];
extern s32 D_8009DF18;

s32 StGetBackloc(CdlLOC *arg0) {
    if (D_8019C7A0 != 0) {
        return -1;
    }
    CdIntToPos(CdPosToInt_Local((CdlLOC *)D_8009DF14) + 1, arg0);
    return D_8009DF18;
}
