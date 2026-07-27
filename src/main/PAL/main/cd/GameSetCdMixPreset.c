#include "common.h"
#include "game/cd.h"
#include "psyq/cd.h"

extern s32 g_CdMixPreset asm("D_8007F5FC");
extern u8 g_CdVolume asm("D_8009B194");


void GameSetCdMixPreset(s32 arg0) asm("func_8004318C");
void GameSetCdMixPreset(s32 arg0) {
    g_CdMixPreset = arg0;
    GameSetCdVolume(g_CdVolume);
}

extern CdlLOC g_CdTrackLocs[] asm("D_8009AFD4");
extern CdlLOC g_CdBgmTrackLocs[] asm("D_8009AFDC");
extern void *g_CdAudioFileNames[] asm("D_8007F568");
extern s32 g_CdTocEntryCount asm("D_801E6C80");
extern u8 g_CdSearchFile[] asm("D_8009B198");

void GameBuildCdTrackTable(void) asm("func_800431BC");
void GameBuildCdTrackTable(void) {
    register CdlLOC *toc asm("$16");
    register s32 i asm("$17");
    register CdlLOC *tocDst asm("$18");
    register u8 *file asm("$19");
    register s32 count asm("$2");

    toc = g_CdTrackLocs;
    g_CdTocEntryCount = CdGetToc(toc);
    if (g_CdTocEntryCount > 0) {
        i = 1;
        toc++;
        do {
            CdIntToPos(CdPosToInt_Local(toc) + 0x3C, toc);
            count = g_CdTocEntryCount;
            i++;
        } while ((count >= i) ? (toc++, 1) : (toc++, 0));
    }

    i = 2;
    file = g_CdSearchFile;
    tocDst = g_CdBgmTrackLocs;
    toc = (CdlLOC *)g_CdAudioFileNames;
    do {
        if (DsSearchFile(file, *(void **)toc) == 0) {
            break;
        }
        *tocDst = *(CdlLOC *)file;
        tocDst++;

        i++;
        toc++;
    } while (i < 0x12);

    g_CdTocEntryCount = 0x10;
}
