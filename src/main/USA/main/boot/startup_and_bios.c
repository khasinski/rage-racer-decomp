#include "common.h"

/* Pre-C runtime entry, BIOS trampolines and syscall wrappers. */
HANDWRITTEN_ASM("src/main/USA/main/boot", startup_and_bios);
