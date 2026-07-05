#include "common.h"

extern s32 D_8009E6D4;
extern s32 D_801F1854;
extern s16 D_801E4DAC;
extern s16 D_8009E74C;

void func_8002C168(void *arg0);
void func_80032280(void *arg0);
void func_80019EFC(s32 arg0);

void func_80035040(void) {
    void *ptr = &D_8009E6D4;

    func_8002C168(ptr);
    func_80032280(ptr);

    if (D_801E4DAC == 1) {
        ptr = &D_801F1854;
        func_8002C168(ptr);
        func_80032280(ptr);
    }

    func_80019EFC(D_8009E74C);
}
