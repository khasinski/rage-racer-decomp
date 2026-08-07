#include "common.h"
#include "psyq/cd.h"

extern long g_StRingSize;
extern StStrHeader *g_StRingBase;
extern long g_StEndFrame;
extern long g_StStreamFlag;
extern long g_StStartFrame;

u_long StFreeRing(u_long *base) {
    long slot;
    long i;
    short nSectors;
    StStrHeader *frame;
    StStrHeader *sector;

    slot = (base - (u_long *)&g_StRingBase[g_StRingSize]) / 504;
    frame = &g_StRingBase[slot];
    nSectors = g_StRingBase[slot].nSectors;
    if ((short)frame->state != 4) {
        return 1;
    }

    for (i = 0; i < nSectors;) {
        sector = &g_StRingBase[i + slot];
        i++;
        *(short *)sector = 0;
    }

    g_StRingSlot = i + slot;
    return 0;
}

/* Returns a value the callers ignore; declaring it void changes codegen.
 * RAW() on the store replaces the barrier that used to hold it in place. */
s32 StClearRingRange(long first, u_long count) {
    u_long i;

    for (i = 0; i < count; i++) {
        RAW(((StRingClearRecord *)g_StRingBase)[i + first].value) = 0;
    }
}

long StGetNext(StRingEventRecord **addr, StRingEventRecord **header) {
    StRingEventRecord **out0 = addr;
    register StRingEventRecord **out1 asm("$8") = header;
    StRingEventRecord *entry;
    long old_flag;

    entry = (StRingEventRecord *)((g_StRingSlot * 32) + (long)g_StRingBase);

    if ((entry->state & 0xFFFF) == 1) {
        old_flag = g_StEndFrame;
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

void StSetRingParams(long streamFlag, long startFrame, long endFrame) {
    g_StStreamFlag = streamFlag;
    g_StStartFrame = startFrame;
    g_StEndFrame = endFrame;
}
