#include "common.h"
#include "game/state.h"
#include "game/asset.h"

extern s32 D_8009AEFC;

void func_800180CC(void);
void func_80018344(void);
void func_80018484(void);
void func_80018588(void);
void func_800188B8(s32);
void func_80018A70(s32);
void func_80018C0C(void);
void func_80018DF8(void);
void func_8001901C(void);
void func_800195F4(void);
void func_800196B4(void);
void func_8001989C(void);

void func_80019C04(void) {
    if (g_AssetLoadState != 0) {
        switch (g_MainState) {
        case 1:
            func_800180CC();
            break;
        case 2:
            func_80018344();
            break;
        case 3:
            func_80018484();
            break;
        case 4:
            func_80018588();
            break;
        case 5:
            func_800188B8(D_8009AEFC);
            break;
        case 6:
            func_80018A70(D_8009AEFC);
            break;
        case 7:
            func_80018C0C();
            break;
        case 8:
            func_80018DF8();
            break;
        case 9:
            func_8001901C();
            break;
        case 10:
            func_800195F4();
            break;
        case 11:
            func_800196B4();
            break;
        case 12:
            func_8001989C();
            break;
        }
    }
}
