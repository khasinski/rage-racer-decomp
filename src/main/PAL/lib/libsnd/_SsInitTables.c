#include <sys/types.h>

#include "common.h"
#include "psyq/snd.h"

extern u_short g_SndVoiceRegDefaults[];
extern u_short g_SndSpuCtrlDefaults[];
extern long g_SndTickResolution;
extern long g_SndSeqOpenMask;
extern long g_SndUpdateLock;
extern long g_SndMarkCallbacks[][0x10];

void SpuVmInit(long arg0);

void _SsInitTables(void) {
    {
        long i;
        register long j asm("$5");
        register u_short *dst asm("$6");
        u_short *table;
        u_short *src;

        dst = (u_short *)0x1F801C00;
        i = 0;
        table = g_SndVoiceRegDefaults;
        for (i = 0; i < 0x18; i++) {
            j = 0;
            src = table;
            for (j = 0; j < 8; j++) {
                *dst++ = *src++;
            }
        }
    }

    {
        long i;
        register u_short *dst asm("$6");
        u_short *src;

        dst = (u_short *)0x1F801D80;
        i = 0;
        src = g_SndSpuCtrlDefaults;
        for (i = 0; i < 0x10; i++) {
            *dst++ = *src++;
        }
    }

    SpuVmInit(0x18);

    {
        register long i asm("$5");
        long j;
        register long *row asm("$3");
        long *clear;

        i = 0;
        row = (long *)g_SndMarkCallbacks;
        for (i = 0; i < 0x20; i++) {
            j = 0xF;
            clear = row + 0xF;
            for (j = 0xF; j >= 0; j--) {
                *clear-- = 0;
            }
            row += 0x10;
        }
    }

    g_SndTickResolution = 0x3C;
    g_SndSeqOpenMask = 0;
    g_SndUpdateLock = 0;
}
