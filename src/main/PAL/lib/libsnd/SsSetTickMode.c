#include "common.h"

#define RAGE_ASM_WRAP(FOLDER, NAME) INCLUDE_ASM(FOLDER, NAME)
#define RAGE_RODATA_WRAP(FOLDER, NAME) INCLUDE_RODATA(FOLDER, NAME)

RAGE_RODATA_WRAP("asm/nonmatchings/PAL/main", SsSetTickMode_rodata);
RAGE_ASM_WRAP("asm/nonmatchings/PAL/main", SsSetTickMode);

#if 0
void SsSetTickMode(void) {
}
#endif
