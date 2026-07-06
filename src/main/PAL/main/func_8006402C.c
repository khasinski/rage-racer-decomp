#include "common.h"

void func_80064264(void);
void func_800642F4(void);
void func_8006438C(void);
void func_8006DF94(s32 arg0, s32 arg1);

void func_8006402C(void) {
    func_80064264();
}

void func_8006404C(void) {
    func_800642F4();
}

void func_8006406C(void) {
    func_8006438C();
}

void func_8006408C(s32 arg0) {
    func_8006DF94(0, arg0);
}

void func_800640B0(s32 arg0) {
    func_8006DF94(1, arg0);
}
