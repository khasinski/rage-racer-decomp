#include "common.h"


extern s16 D_8019C798;
extern s32 D_8019CAB4;
extern s32 D_8019CA04;
extern u8 *D_801E42D8;
extern s16 D_801E4B90;
extern s16 D_801E4BA0;
extern s16 D_801E8A4C;

void func_8002BE18(u8 *arg0) {
    s32 value;
    s16 index;

    value = ((D_8019CAB4 - D_8019C798) * 10000) / *(s16 *)(D_801E42D8 + 0x100);
    D_801E8A4C = 0;

    if (value < 0) {
        value = D_8019CAB4 - 1000;
        if (D_8019CAB4 >= 2000) {
            goto done;
        }
        value = 0;
        goto done;
    } else {
        index = *(s16 *)(arg0 + 0x132);
        value *= D_801E4B90 / ((index * 200) + 300);
        *(s32 *)(arg0 + 0x150) = *(s32 *)(arg0 + 0x150) / index;
        if (*(s16 *)(arg0 + 0x132) >= 2) {
            D_801E4BA0 = 200;
        }
    }

done:
    D_8019CA04 = value;
}
