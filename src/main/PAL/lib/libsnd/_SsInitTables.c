#include <sys/types.h>

#include "common.h"

extern u_short D_8009A528[];
extern u_short D_8009A538[];
extern long D_801E6C6C;
extern long D_801E4B80;
extern long D_801E40AC;
extern long D_801E7A58[][0x10];

void SpuVmInit(long arg0) asm("func_80075710");

void _SsInitTables(void) asm("func_80071B0C");

void _SsInitTables(void) {
    {
        register long i asm("$4");
        register long j asm("$5");
        register u_short *dst asm("$6");
        register u_short *table asm("$7");
        register u_short *src asm("$3");

        dst = (u_short *)0x1F801C00;
        i = 0;
        table = D_8009A528;
        for (i = 0; i < 0x18; i++) {
            j = 0;
            src = table;
            for (j = 0; j < 8; j++) {
                *dst++ = *src++;
            }
        }
    }

    {
        register long i asm("$4");
        register u_short *dst asm("$6");
        register u_short *src asm("$3");

        dst = (u_short *)0x1F801D80;
        i = 0;
        src = D_8009A538;
        for (i = 0; i < 0x10; i++) {
            *dst++ = *src++;
        }
    }

    SpuVmInit(0x18);

    {
        register long i asm("$5");
        register long j asm("$4");
        register long *row asm("$3");
        register long *clear asm("$2");

        i = 0;
        row = (long *)D_801E7A58;
        for (i = 0; i < 0x20; i++) {
            j = 0xF;
            clear = row + 0xF;
            for (j = 0xF; j >= 0; j--) {
                *clear-- = 0;
            }
            row += 0x10;
        }
    }

    D_801E6C6C = 0x3C;
    D_801E4B80 = 0;
    D_801E40AC = 0;
}
