#include "common.h"

/* Pre-C runtime entry, BIOS trampolines and syscall wrappers. */
HANDWRITTEN_ASM("src/main/JAP11/main/boot", startup_and_bios);
