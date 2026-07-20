#include "common.h"

typedef struct {
    void (*cb)(s32, s32);
    s32 arg;
    s32 tag;
    s32 params[21];
} QEntry;

typedef struct {
    void (*cb)(s32, s32);
    s32 arg;
    s32 tag;
} LastCb;

extern volatile QEntry D_801E5024[];
extern volatile s32 D_800942EC;
extern volatile s32 D_800942F0;
extern s32 D_800942F8;
extern volatile LastCb D_800942DC;
extern volatile u32 *D_800942C8;
extern volatile u32 *D_800942BC;
extern u8 D_800941F0[];
extern u8 D_800941F4v[] asm("D_800941F4");
extern volatile s32 D_800941F4s asm("D_800941F4");

extern s32 func_8006E0B0(s32);
extern void func_8006DF94(s32, void *);

s32 func_80067984(void) {
    if (*D_800942C8 & 0x01000000) {
        return 1;
    }

    D_800942F8 = func_8006E0B0(0);

    if (D_800942EC != D_800942F0) {
        while ((*D_800942C8 & 0x01000000) == 0) {
            if ((((D_800942F0 + 1) & 0x3f) == D_800942EC) && (*(volatile s32 *)D_800941F4v == 0)) {
                func_8006DF94(2, 0);
            }

            while ((*D_800942BC & 0x04000000) == 0) {
                ;
            }

            D_801E5024[D_800942F0].cb(D_801E5024[D_800942F0].arg,
                                      D_801E5024[D_800942F0].tag);
            D_800942DC.cb = D_801E5024[D_800942F0].cb;
            D_800942DC.arg = D_801E5024[D_800942F0].arg;
            D_800942DC.tag = D_801E5024[D_800942F0].tag;

            D_800942F0 = (D_800942F0 + 1) & 0x3f;
            if (D_800942EC == D_800942F0) {
                break;
            }
        }
    }

    func_8006E0B0(D_800942F8);

    if ((D_800942EC == D_800942F0) && ((*D_800942C8 & 0x01000000) == 0) &&
        (*(volatile s32 *)D_800941F0 != 0) && (D_800941F4s != 0)) {
        *(volatile s32 *)D_800941F0 = 0;
        ((void (*)(void))D_800941F4s)();
    }

    return (D_800942EC - D_800942F0) & 0x3f;
}
