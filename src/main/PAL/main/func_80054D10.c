#include "common.h"

#define RAGE_ASM_WRAP(FOLDER, NAME) INCLUDE_ASM(FOLDER, NAME)
#define RAGE_RODATA_WRAP(FOLDER, NAME) INCLUDE_RODATA(FOLDER, NAME)

RAGE_RODATA_WRAP("asm/nonmatchings/PAL/main", func_80054D10_rodata);
RAGE_ASM_WRAP("asm/nonmatchings/PAL/main", func_80054D10);

#if 0
void func_80054D10(void) {
}
#endif
