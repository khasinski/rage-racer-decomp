#include "common.h"

#define RAGE_ASM_WRAP(FOLDER, NAME) INCLUDE_ASM(FOLDER, NAME)
#define RAGE_RODATA_WRAP(FOLDER, NAME) INCLUDE_RODATA(FOLDER, NAME)

RAGE_RODATA_WRAP("asm/nonmatchings/PAL/main", func_8001901C_rodata);
RAGE_ASM_WRAP("asm/nonmatchings/PAL/main", func_8001901C);

#if 0
void func_8001901C(void) {
}
#endif
