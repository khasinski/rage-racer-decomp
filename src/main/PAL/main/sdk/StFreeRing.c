#include "common.h"
#include "psyq/cd.h"

extern long g_StRingSize asm("D_801F1850");
extern long g_StRingSlot asm("D_801E6C98");
extern StStrHeader *g_StRingBase asm("D_801E8AAC");
extern long D_801E8278;
extern long D_801E8A94;
extern long D_801E3E10;

u_long StFreeRing(u_long *base) {
    long temp_a1;
    long i;
    short nSectors;
    StStrHeader *temp_v0;
    StStrHeader *temp_v0_2;

    temp_a1 = (base - (u_long *)&g_StRingBase[g_StRingSize]) / 504;
    temp_v0 = &g_StRingBase[temp_a1];
    nSectors = g_StRingBase[temp_a1].nSectors;
    if ((short)temp_v0->state != 4) {
        return 1;
    }

    for (i = 0; i < nSectors;) {
        temp_v0_2 = &g_StRingBase[i + temp_a1];
        i++;
        *(short *)temp_v0_2 = 0;
    }

    g_StRingSlot = i + temp_a1;
    return 0;
}

void func_8006D0AC(long arg0, u_long arg1) {
    u_long i;

    for (i = 0; i < arg1; i++) {
        asm("" ::: "memory");
        ((StRingClearRecord *)g_StRingBase)[i + arg0].value = 0;
    }
}

long StGetNext(StRingEventRecord **arg0, StRingEventRecord **arg1) asm("func_8006D0EC");
long StGetNext(StRingEventRecord **arg0, StRingEventRecord **arg1) {
    StRingEventRecord **out0 = arg0;
    register StRingEventRecord **out1 asm("$8") = arg1;
    StRingEventRecord *entry;
    long old_flag;

    entry = (StRingEventRecord *)((g_StRingSlot * 32) + (long)g_StRingBase);

    if ((entry->state & 0xFFFF) == 1) {
        old_flag = D_801E8278;
        g_StRingSlot = 0;
        if (old_flag != 0) {
            entry->state = 0;
        }

        entry = (StRingEventRecord *)((g_StRingSlot * 32) + (long)g_StRingBase);
    }

    if ((entry->state & 0xFFFF) == 2) {
        long track;
        StRingEventRecord *raw_base;
        long index;
        StRingEventRecord *base;
        long offset;

        /* RAW() keeps this store ahead of the ring globals below -- see
         * common.h. */
        RAW(entry->state) = 4;
        track = g_StRingSize;
        raw_base = (StRingEventRecord *)g_StRingBase;
        index = g_StRingSlot;
        base = (StRingEventRecord *)((track * 32) + (long)raw_base);
        offset = index * 63;
        offset <<= 5;
        base = (StRingEventRecord *)((long)base + offset);
        *out0 = base;
        *out1 = entry;
        return 0;
    }

    return 1;
}

void func_8006D1B0(long arg0, long arg1, long arg2) {
    D_801E8A94 = arg0;
    D_801E3E10 = arg1;
    D_801E8278 = arg2;
}
