#include "common.h"

extern u32 D_8007BED8;
extern u32 D_801E4178;

void func_80019730(void);
void func_80019844(void);
void func_80026C0C(void);

void func_80026D30(void) {
    if (D_8007BED8 == 0) {
        func_80019730();
        func_80019844();
        D_801E4178 = 1;
    }

    func_80026C0C();
}
