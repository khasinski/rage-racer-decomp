#include "common.h"

/* A kernel entry reached by `syscall`; the whole unit is assembly.
 * See src/main/PAL/lib/libapi/exit_critical_section.s. */
INCLUDE_ASM_TU("src/main/PAL/lib/libapi", exit_critical_section);
