#include "common.h"

/*
 * LEFT GENERIC ON PURPOSE: libsnd internal, so there is no public Ss* name to
 * claim. It is the MIDI Control Change #6 (Data Entry MSB) handler - it applies
 * the pending RPN/NRPN to the channel's VAB program by rewriting the VagAtr of
 * every tone. See docs/names.md 1.
 */
INCLUDE_ASM("asm/PAL/main/nonmatchings/main/sdk/func_8007010C", func_8007010C);

