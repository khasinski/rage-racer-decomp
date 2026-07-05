#include "common.h"

#define RAGE_ASM_WRAP(FOLDER, NAME) INCLUDE_ASM(FOLDER, NAME)

RAGE_ASM_WRAP("asm/nonmatchings/PAL/main", _spu_t);

#if 0
void _spu_t(void) {
}
#endif
