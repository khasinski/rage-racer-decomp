#include "common.h"
#include "game/asset.h"

extern s32 D_8019C768;
extern s32 D_801E4B30;
extern s32 D_8009E6A4;
extern s32 D_801E40B8;
extern s32 D_801E42E4;
extern s32 D_801E42E0;
extern u8 *D_8009E67C;
extern s32 D_8009EC90;
extern s32 D_801E428C;

void func_80065860(s32 arg0);
void func_8005B9CC(void);
void func_8001A3C0(s32 arg0);
void func_80018F08(void);

void func_8001C7BC(void) {
    s32 count;
    u8 *ptr;
    u8 *end;

    func_80065860(0);
    D_8019C768 = 0x80;

    if (g_AssetLoadState != 1) {
        func_8005B9CC();
        func_8001A3C0(D_801E4B30);
        func_80018F08();

        D_8019C768 = 0x180;
        D_801E40B8 = 0;
        D_801E42E4 = 10;
        D_801E42E0 = 0;
        count = (D_8009E6A4 < 2) ? 3 : 4;
        D_8009EC90 = 0;

        if (count != 0) {
            ptr = D_8009E67C;
            asm volatile("addu %0,%1,%2" : "=r"(end) : "0"(count), "r"(ptr));
            do {
                if (*ptr != 0) {
                    D_8009EC90++;
                }
                ptr++;
            } while ((s32)ptr < (s32)end);
        }

        if (D_8009E67C[D_801E428C] == 0) {
            D_8009EC90++;
        }
    }
}
