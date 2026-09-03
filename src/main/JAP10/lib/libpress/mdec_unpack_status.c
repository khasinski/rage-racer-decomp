#include "common.h"

/* Original resumable MDEC VLC decoder with its state word embedded in .text. */
HANDWRITTEN_ASM("src/main/JAP10/lib/libpress", mdec_unpack_status);
