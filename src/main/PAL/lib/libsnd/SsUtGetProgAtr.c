#include "common.h"
#include "psyq/snd.h"

extern u8 D_801E4CFC[];
extern u8 *D_801E4110;

s32 SpuVmVSetUp(s32 arg0, s32 arg1) asm("func_80073314");

s32 SsUtGetProgAtr(s32 arg0, s32 arg1, ProgAtr *out) asm("func_8007320C");

s32 SsUtGetProgAtr(s32 arg0, s32 arg1, ProgAtr *out) {
    s32 chan = (s16)arg0;
    s32 index;
    s32 offset;

    if (D_801E4CFC[chan] == 1) {
        index = (s16)arg1;
        SpuVmVSetUp(chan, index);
        offset = index * 0x10;

        out->tones = *(u8 *)(offset + (s32)D_801E4110);
        out->mvol = D_801E4110[offset + 1];
        out->prior = D_801E4110[offset + 2];
        out->mode = D_801E4110[offset + 3];
        out->mpan = D_801E4110[offset + 4];
        offset = (s32)(D_801E4110 + offset);
        out->attr = *(u16 *)(offset + 6);
    } else {
        return -1;
    }

    return 0;
}
