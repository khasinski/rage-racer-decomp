#include "common.h"
#include "game/audio.h"

extern s16 D_80082F28[];
extern s16 D_80082F2A[];
extern s32 D_801E6CC8[];

void GameSetSoundSlotVoicesEnabledWithRegisterArg(void) asm("func_8005B40C");
void GamePlaySoundSlotVoice(s32 slot, s32 tone, s32 vabSlot) asm("func_8005B2F0");
void func_8005D7D4(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);

void GameForceSoundSlotVoicePlayback(s32 arg0) {
    register s32 saved asm("$16") = arg0;
    register s32 i asm("$17");
    register s32 *base asm("$18");
    register s32 *active asm("$19");
    register s32 odd asm("$20");
    s32 first;
    s32 second;
    register s32 factor asm("$3");
    register s32 scaled asm("$6");
    register s32 call_arg0 asm("$4");
    register s32 call_arg1 asm("$5");
    register s32 call_arg3 asm("$7");

    GameSetSoundSlotVoicesEnabledWithRegisterArg();

    i = 0;
    if (saved != 0) {
        base = D_801E6CC8;
        active = base;
        saved = 0;
        do {
            if (*base++ != 0 && *(s16 *)((s32)D_80082F28 + saved) != *(s16 *)((s32)D_80082F2A + saved)) {
                GamePlaySoundSlotVoice(i, active[-3], 3);
            }
            i++;
            saved += 4;
        } while (i < 6);

        i = 0;
        odd = 1;
        base = D_801E6CC8;
        active = base;
        do {
            if (*active != 0) {
                first = GameInterpolateAudioParameter(i * 2, base[-4], base[-3]);
                second = GameInterpolateAudioParameter(odd, base[-4], base[-3]);
                factor = base[6];
                scaled = second * factor;
                if (scaled < 0) {
                    scaled += 0x7F;
                }
                call_arg0 = i;
                call_arg1 = first;
                scaled >>= 7;
                call_arg3 = base[-3];
                func_8005D7D4(call_arg0, call_arg1, scaled, call_arg3, 3);
            }
            odd += 2;
            i++;
            active++;
        } while (i < 6);
    }
}
