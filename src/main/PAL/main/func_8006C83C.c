#include "common.h"
#include "psyq/cd.h"

extern volatile s32 D_8009C118[];
extern CdSearchDirEntry D_8009C120[];

s32 LibcStrcmp(u8 *arg0, u8 *arg1) asm("func_8006CC28");

s32 func_8006C83C(s32 type, u8 *name) {
    register s32 typeReg asm("$19") = type;
    register u8 *nameReg asm("$20") = name;
    register s32 i asm("$16") = 0;
    register u8 *entryName asm("$18") = (u8 *)D_8009C120;
    register s32 offset asm("$17") = 0;
    s32 entryType;

    while (i < 0x80) {
        entryType = *(s32 *)((u8 *)D_8009C118 + offset);
        if (entryType != 0) {
            if (entryType == typeReg && LibcStrcmp(nameReg, entryName) == 0) {
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
