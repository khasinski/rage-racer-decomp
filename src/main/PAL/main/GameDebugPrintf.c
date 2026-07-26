#include "common.h"

/*
 * Stubbed-out debug printf: returns 1 and does nothing. Left un-prototyped
 * because callers pass anything from one to seven arguments. The format
 * strings they pass survive in the binary and were the key to naming much of
 * the game (see docs/names.md).
 */
s32 GameDebugPrintf() {
    return 1;
}
