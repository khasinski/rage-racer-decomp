#include "common.h"

#define RAGE_ASM_WRAP(FOLDER, NAME) INCLUDE_ASM(FOLDER, NAME)

RAGE_ASM_WRAP("asm/nonmatchings/PAL/main", SsSetSerialVol);

#if 0
void SsSetSerialVol(void) {
}
#endif
