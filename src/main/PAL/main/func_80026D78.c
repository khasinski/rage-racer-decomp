#include "common.h"

extern u32 D_8007BED8;
extern u32 D_801E42A0;
extern u32 D_801E4178;

void func_80042BC0(u32 arg0);
void func_80026C0C(void);

void func_80026D78(void) {
    if (D_8007BED8 == 0) {
        D_801E42A0 = 4;
        func_80042BC0(2);
        D_801E4178 = 2;
    }

    func_80026C0C();
}
