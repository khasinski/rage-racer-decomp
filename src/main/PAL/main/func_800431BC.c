#include "common.h"
#include "psyq/cd.h"

extern CdlLOC D_8009AFD4[];
extern CdlLOC D_8009AFDC[];
extern void *D_8007F568[];
extern s32 D_801E6C80;
extern u8 D_8009B198[];

void func_800431BC(void) {
    register CdlLOC *toc asm("$16");
    register s32 i asm("$17");
    register CdlLOC *tocDst asm("$18");
    register u8 *file asm("$19");
    register s32 count asm("$2");
    register u32 word asm("$2");

    toc = D_8009AFD4;
    D_801E6C80 = CdGetToc(toc);
    if (D_801E6C80 > 0) {
        i = 1;
        toc++;
        do {
            CdIntToPos(CdPosToInt_Local(toc) + 0x3C, toc);
            count = D_801E6C80;
            i++;
        } while ((count >= i) ? (toc++, 1) : (toc++, 0));
    }

    i = 2;
    file = D_8009B198;
    tocDst = D_8009AFDC;
    toc = (CdlLOC *)D_8007F568;
    do {
        if (DsSearchFile(file, *(void **)toc) == 0) {
            break;
        }
        asm volatile(
            "lwl %0,3(%1)\n"
            "lwr %0,0(%1)\n"
            "nop\n"
            "swl %0,3(%2)\n"
            "swr %0,0(%2)"
            : "=&r"(word)
            : "r"(file), "r"(tocDst)
            : "memory");
        tocDst++;

        i++;
        toc++;
    } while (i < 0x12);

    D_801E6C80 = 0x10;
}
