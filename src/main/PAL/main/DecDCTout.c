#include "common.h"

void func_80064264(void);
void func_800642F4(void);
void func_8006438C(void);
void func_8006DF94(s32 arg0, s32 arg1);

void DecDCTout(void) asm("func_8006402C");
void DecDCTout(void) {
    func_80064264();
}

void DecDCTinSync(void) asm("func_8006404C");
void DecDCTinSync(void) {
    func_800642F4();
}

void DecDCToutSync(void) asm("func_8006406C");
void DecDCToutSync(void) {
    func_8006438C();
}

void DecDCTinCallback(s32 arg0) asm("func_8006408C");
void DecDCTinCallback(s32 arg0) {
    func_8006DF94(0, arg0);
}

void DecDCToutCallback(s32 arg0) asm("func_800640B0");
void DecDCToutCallback(s32 arg0) {
    func_8006DF94(1, arg0);
}
