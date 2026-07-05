#include "common.h"

extern u32 D_801E42E4;

void func_80042C0C(void);
void func_80018410(void);

void func_80026F68(void) {
    D_801E42E4 = 6;
    func_80042C0C();
    func_80018410();
}
