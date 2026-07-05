#include "common.h"
extern s32 D_8019C704;
extern s32 D_801E8A50;
extern s32 D_801E6C70;
void func_8005BD84(s32 arg0);
void func_8005BDD4(s32 arg0);
void func_8005BE24(void);
void func_8005BE58(void);
void func_80021224(void) {
    func_8005BD84(D_8019C704);
    func_8005BDD4(D_801E8A50);
    if (D_801E6C70 == 0) {
        func_8005BE24();
    } else {
        func_8005BE58();
    }
}
