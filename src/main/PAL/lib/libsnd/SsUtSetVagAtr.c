#include "common.h"
#include "psyq/snd.h"

extern u8 D_801E4CFC[];
extern u8 D_801E4BD7;
extern volatile u8 *D_801E416C;

s32 SpuVmVSetUp(s32 arg0, s32 arg1) asm("func_80073314");

s32 SsUtSetVagAtr(s32 arg0, s32 arg1, s32 arg2, VagAtr *arg3) asm("func_80073820");

s32 SsUtSetVagAtr(s32 arg0, s32 arg1, s32 arg2, VagAtr *arg3) {
    s32 index;
    s32 offset;

    index = (s16)arg0;
    if (D_801E4CFC[index] == 1) {
        SpuVmVSetUp(index, (s16)arg1);
        offset = (s16)(arg2 + (D_801E4BD7 << 4)) << 5;

        /* Preserves the original `offset + base` addu operand order. */
        *(volatile u8 *)(offset + (s32)D_801E416C) = arg3->prior;
        D_801E416C[offset + 0x1] = arg3->mode;
        D_801E416C[offset + 0x2] = arg3->vol;
        D_801E416C[offset + 0x3] = arg3->pan;
        D_801E416C[offset + 0x4] = arg3->center;
        D_801E416C[offset + 0x5] = arg3->shift;
        D_801E416C[offset + 0x7] = arg3->max;
        D_801E416C[offset + 0x6] = arg3->min;
        D_801E416C[offset + 0x8] = arg3->vibW;
        D_801E416C[offset + 0x9] = arg3->vibT;
        D_801E416C[offset + 0xA] = arg3->porW;
        D_801E416C[offset + 0xB] = arg3->porT;
        D_801E416C[offset + 0xC] = arg3->pbmin;
        D_801E416C[offset + 0xD] = arg3->pbmax;
        *(u16 *)&D_801E416C[offset + 0x10] = arg3->adsr1;
        *(u16 *)&D_801E416C[offset + 0x12] = arg3->adsr2;
        *(u16 *)&D_801E416C[offset + 0x14] = arg3->prog;
        *(u16 *)&D_801E416C[offset + 0x16] = arg3->vag;

        return 0;
    }
    return -1;
}
