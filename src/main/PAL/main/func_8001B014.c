#include "common.h"
extern s32 D_8019C760, D_801E6F28, D_8019CB70, D_8009E880, D_8019C768;
extern s32 D_801E40B8, D_801E42E4, D_801E8260, D_801E6F1C, D_8009F098;
void func_8001BE9C(s32 arg0, s32 arg1, s32 arg2);
void func_80065860(s32 arg0);
void func_8001A498(void);
void func_80021540(void);
void func_8005DBB4(void);
void func_8001B170(void);
void func_8001B014(void) {
    func_8001BE9C(0, 0, 0);
    if (D_8019C760 != 0) {
        D_801E6F28 = 0xFF;
        D_8019CB70 = 0x190;
        D_8009E880 = 0;
    } else {
        func_80065860(0);
        func_8001A498();
        D_801E6F28 = 0;
        D_8019CB70 = 0;
        D_8009E880 = 0x1E;
    }
    D_8019C768 = 0x80;
    D_801E40B8 = 0;
    D_801E42E4 = 4;
    D_801E8260 = 0;
    D_801E6F1C = 0;
    D_8009F098 = 0;
    func_80021540();
    func_8005DBB4();
    func_8001B170();
}
