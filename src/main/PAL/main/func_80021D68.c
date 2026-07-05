#include "common.h"

extern char D_80010EC4[];
extern char D_80010ED0[];

void func_80016EA0(s32 arg0, s32 arg1, char *arg2, s32 arg3);
void func_80016A18(s32 arg0, s32 arg1, char *arg2, s32 arg3);
void func_800200D0(void);

void func_80021D68(void) {
    func_80016EA0(0x10, 0x1C, D_80010EC4, 0x7812);
    func_80016A18(0x10, 0x39, D_80010ED0, 0x78CC);
    func_800200D0();
}
