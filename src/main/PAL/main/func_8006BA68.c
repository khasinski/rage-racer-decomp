#include "common.h"
#include "psyq/cd.h"

extern u8 *D_80099300;
extern u8 *D_80099304;
extern u8 *D_80099308;
extern u8 *D_8009930C;

s32 CD_vol(CdlATV *arg0) {
    *D_80099300 = 2;
    *D_80099308 = arg0->val0;
    *D_8009930C = arg0->val1;
    *D_80099300 = 3;
    *D_80099304 = arg0->val2;
    *D_80099308 = arg0->val3;
    *D_8009930C = 0x20;
    return 0;
}
