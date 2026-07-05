#include "common.h"

extern s32 D_8019C768;
extern s32 D_801E42E4;
extern s32 D_801E40B8;
extern s32 D_801E8260;
extern s32 D_801E6F28;
extern s32 D_801E6F1C;
extern s32 D_801E429C;
extern s32 D_8009F098;
extern s32 D_8009E880;
extern s32 D_8019CB70;

void func_80065860(s32 arg0);
void func_8005B9CC(void);
void func_80019EBC(void);
void func_8001A498(void);
void func_80021540(void);
void func_8005DBB4(void);

void func_8001AF70(void) {
    func_80065860(0);
    func_8005B9CC();
    func_80019EBC();
    func_8001A498();

    D_8019C768 = 0x80;
    D_801E42E4 = 4;
    D_801E40B8 = 0;
    D_801E8260 = 0;
    D_801E6F28 = 0;
    D_801E6F1C = 0;
    D_801E429C = 0;
    D_8009F098 = 0;
    D_8009E880 = 0;
    D_8019CB70 = -1;

    func_80021540();
    func_8005DBB4();
}
