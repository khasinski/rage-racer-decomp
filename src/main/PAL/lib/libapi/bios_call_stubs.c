#include "common.h"

/* Five BIOS call stubs; the whole unit is assembly.
 * See src/main/PAL/lib/libapi/bios_call_stubs.s. */
INCLUDE_ASM_TU("src/main/PAL/lib/libapi", bios_call_stubs);
