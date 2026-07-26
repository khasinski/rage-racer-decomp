#include "common.h"
#include "psyq/cd.h"

extern s32 D_80099048;
extern s32 D_8009903C;
extern s32 D_80099040;
extern char *D_80099060[];
extern char *D_800990E0[];
extern u8 D_800136D0[];

void func_8006BAF0(void);
s32 func_8006B0D4(s32 arg0, s32 arg1);
s32 func_8006B354(s32 arg0, s32 arg1);

/* CdFlush. */
void CdFlush(void) asm("func_8006A494");
void CdFlush(void) {
    func_8006BAF0();
}

/* CdSetDebug: sets the CD debug-verbosity `level`, returns the previous one. */
s32 CdSetDebug(s32 level) asm("func_8006A4B4");
s32 CdSetDebug(s32 level) {
    s32 old = D_80099048;
    D_80099048 = level;
    return old;
}

/* CdComstr: returns the human-readable name for CD command `cmd` (or a default
 * string if out of range). */
char * CdComstr(s32 cmd) asm("func_8006A4CC");
char *CdComstr(s32 cmd) {
    cmd &= 0xFF;
    if ((u32)cmd >= 0x1C) {
        return (char *)D_800136D0;
    }
    return D_80099060[cmd];
}

/* CdIntstr: returns the human-readable name for CD interrupt code `intr`. */
char * CdIntstr(s32 intr) asm("func_8006A500");
char *CdIntstr(s32 intr) {
    intr &= 0xFF;
    if ((u32)intr >= 7) {
        return (char *)D_800136D0;
    }
    return D_800990E0[intr];
}

s32 CdSync(s32 arg0, s32 arg1) asm("func_8006A534");
s32 CdSync(s32 arg0, s32 arg1) {
    return func_8006B0D4(arg0, arg1);
}

s32 CdReady(s32 arg0, s32 arg1) asm("func_8006A554");
s32 CdReady(s32 arg0, s32 arg1) {
    return func_8006B354(arg0, arg1);
}

s32 CdSyncCallback(s32 arg0) asm("func_8006A574");
s32 CdSyncCallback(s32 arg0) {
    s32 old = D_8009903C;

    D_8009903C = arg0;
    return old;
}

s32 CdReadyCallback(s32 arg0) asm("func_8006A58C");
s32 CdReadyCallback(s32 arg0) {
    s32 old = D_80099040;

    D_80099040 = arg0;
    return old;
}
