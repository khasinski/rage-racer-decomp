#include <sys/types.h>

/* The kernel is reached by a `syscall` instruction, which has no C spelling, so
   this entry is emitted as its instruction words like the other kernel stubs:
   addiu $a0, $zero, 0x2; syscall 0; jr $ra; nop. The exported name is bound to
   this object in linkers/PAL/undefined_syms_manual.txt so callers can keep the
   `void ExitCriticalSection(void)` declaration in psyq/kernel.h. */
u_long ExitCriticalSectionStub[4] __attribute__((section(".text"))) = {
    0x24040002,
    0x0000000C,
    0x03E00008,
    0,
};
