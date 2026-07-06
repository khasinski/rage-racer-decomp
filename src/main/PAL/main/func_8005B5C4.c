#include "common.h"
#include "game/audio.h"
#include "psyq/snd.h"

extern u8 D_8009B3B8[];
extern s16 D_801E6CA8;
extern char D_8001267C[];
extern char D_80012694[];

s32 func_8007317C(s32 arg0);
s32 func_800730BC(s32 arg0, s32 arg1);
s32 func_80072C4C(s32 arg0, s32 arg1, s32 arg2);
void func_8001674C(char *arg0);
void func_80063D9C(s32 arg0);

s32 func_8005B5C4(s32 header, s32 body) {
    register s32 headerReg asm("$16") = header;
    register s32 bodyReg asm("$18") = body;
    register s16 *vabIdPtr asm("$17");
    register s32 currentVabId asm("$5");
    register s32 fail asm("$16");
    register s32 ret asm("$2");

    asm("" : "=r"(bodyReg) : "0"(bodyReg));
    SsSetTableSize(D_8009B3B8, 2, 1);
    SsSetTickMode(1);
    SsStartSoundTickMode1();
    SsSetVoiceCount(0xA);
    SsUtReverbOff();
    GameSetReverbPreset(2, 0, 0);
    GameResetSoundState();

    ret = func_80072C4C(headerReg, -1, 0x1000);
    vabIdPtr = &D_801E6CA8;
    *vabIdPtr = ret;
    asm volatile("" : "=r"(ret) : "0"(ret));
    currentVabId = (s16)ret;
    fail = -1;
    if (currentVabId == fail) {
        func_8001674C(D_8001267C);
        func_80063D9C(1);
    }

    ret = func_800730BC(bodyReg, currentVabId);
    *vabIdPtr = ret;
    if ((s16)ret == fail) {
        func_8001674C(D_80012694);
        func_80063D9C(1);
    }

    func_8007317C(1);
    SsSetMVol(0x3FFF, 0x3FFF);
    SsSetReservedVoice(0);
    return 0;
}
