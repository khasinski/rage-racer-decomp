#include "common.h"

#define RAGE_ASM_WRAP(FOLDER, NAME) INCLUDE_ASM(FOLDER, NAME)

RAGE_ASM_WRAP("asm/nonmatchings/PAL/main", Snd_SetPlayMode);

#if 0
void Snd_SetPlayMode(void) {
}
#endif
