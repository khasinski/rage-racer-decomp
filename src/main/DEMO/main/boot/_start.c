#include "common.h"

/* This pre-release build's entry point: unlike PAL's trivial BSS-clear-then-
 * call-__main() _start, this one computes a heap/stack boundary pair for the
 * test harness and jumps directly into main_loop, never returning. Hand-
 * written per the project's established entry-stub convention (see
 * src/main/PAL/main/boot/_start.c); the raw jump-through-a-register/syscall
 * class of content a C compiler would never emit this way. */
HANDWRITTEN_ASM("src/main/DEMO/main/boot", _start);
