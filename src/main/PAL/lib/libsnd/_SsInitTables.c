#include "common.h"

extern u16 D_8009A528[];
extern u16 D_8009A538[];
extern s32 D_801E6C6C;
extern s32 D_801E4B80;
extern s32 D_801E40AC;
extern s32 D_801E7A58[][0x10];

void func_80075710(s32 arg0);

void _SsInitTables(void) asm("func_80071B0C");

void _SsInitTables(void) {
    {
        register s32 i asm("$4");
        register s32 j asm("$5");
        register u16 *dst asm("$6");
        register u16 *table asm("$7");
        register u16 *src asm("$3");

        dst = (u16 *)0x1F801C00;
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
        register s32 i asm("$4");
        register u16 *dst asm("$6");
        register u16 *src asm("$3");

        dst = (u16 *)0x1F801D80;
        i = 0;
        src = D_8009A538;
        for (i = 0; i < 0x10; i++) {
            *dst++ = *src++;
        }
    }

    func_80075710(0x18);

    {
        register s32 i asm("$5");
        register s32 j asm("$4");
        register s32 *row asm("$3");
        register s32 *clear asm("$2");

        i = 0;
        row = (s32 *)D_801E7A58;
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
