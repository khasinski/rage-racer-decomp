#include "common.h"
#include "psyq/cd.h"

extern volatile s32 D_8009C118[];
extern CdSearchDirEntry D_8009C120[];

s32 func_8006CC28(u8 *arg0, u8 *arg1);

s32 DS_searchdir(s32 type, u8 *name) asm("func_8006C83C");
s32 DS_searchdir(s32 type, u8 *name) {
    s32 i = 0;
    u8 *entryName = (u8 *)D_8009C120;
    s32 offset = 0;
    s32 entryType;

    while (i < 0x80) {
        entryType = *(s32 *)((u8 *)D_8009C118 + offset);
        if (entryType != 0) {
            if (entryType == type && func_8006CC28(name, entryName) == 0) {
                return i + 1;
            }

            entryName += sizeof(CdSearchDirEntry);
            i++;
            offset += sizeof(CdSearchDirEntry);
        } else {
            return -1;
        }
    }

    return -1;
}
