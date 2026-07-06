#include "common.h"
#include "game/audio.h"

extern u16 D_801E436E;
extern s32 D_8019C97C;
extern s32 D_8019CB14;

void func_80024420(void);
void func_80023FE8(void);

void func_80024718(void) {
    u16 *buttonPtr;
    s32 oldCursor;
    u16 buttons;

    func_80024420();

    buttonPtr = &D_801E436E;
    oldCursor = D_8019C97C;
    buttons = *buttonPtr;
    if (buttons & 0x1000) {
        D_8019C97C = oldCursor - 1;
    }
    if (buttons & 0x4000) {
        D_8019C97C = D_8019C97C + 1;
    }

    D_8019C97C = (D_8019C97C + 2) % 2;
    if (oldCursor != D_8019C97C) {
        GamePlaySoundCue(1);
    }

    buttons = *buttonPtr;
    if (buttons & 0x860) {
        GamePlaySoundCue(2);
        if (D_8019C97C != 0) {
            D_8019CB14 = 1;
        } else {
            D_8019CB14 = 3;
        }
    } else if (buttons & 0x90) {
        GamePlaySoundCue(3);
        D_8019CB14 = 1;
    }

    func_80023FE8();
}
