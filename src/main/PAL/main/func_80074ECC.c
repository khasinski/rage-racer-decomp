#include "common.h"

typedef struct SpuVoice {
    u8 pad0[0x1C];
    s16 autoVolume;
    s16 volumeStep;
    s16 volumeCounter;
    s16 volumeCounterReload;
    s16 currentVolume;
    s16 targetVolume;
    u8 pad28[0xC];
} SpuVoice;

typedef struct SvmCurrent {
    u8 field0[4];
    u8 currentVolume;
    u8 pan;
    u8 field6[4];
    u8 masterVolume;
    u8 masterPan;
    u8 fieldC;
    u8 volume;
    u8 currentPan;
} SvmCurrent;

typedef struct VabHeader {
    u8 pad0[0x18];
    u8 masterVolume;
} VabHeader;

extern u16 D_8009DF20[];
extern u8 D_8009E0A0[];
extern SpuVoice D_8009E0B8[];
extern VabHeader *D_801E413C;
extern SvmCurrent D_801E4BD0;
extern s16 D_801E3FB0;

void func_80074ECC(s16 voice) {
    s16 currentVolume;
    u32 leftVolumeTemp;
    u32 rightVolumeTemp;
    u16 leftVolume;
    u16 rightVolume;
    s16 registerOffset;
    s32 scaledMasterVolume;

    registerOffset = voice * 8;
    if (D_8009E0B8[voice].volumeCounter != 0) {
        if (D_8009E0B8[voice].volumeCounterReload-- > 0) {
            return;
        }
        D_8009E0B8[voice].volumeCounterReload =
            D_8009E0B8[voice].volumeCounter;
    }

    D_8009E0B8[voice].currentVolume += D_8009E0B8[voice].volumeStep;
    if (D_8009E0B8[voice].volumeStep > 0) {
        if (D_8009E0B8[voice].currentVolume >=
            D_8009E0B8[voice].targetVolume) {
            D_8009E0B8[voice].currentVolume =
                D_8009E0B8[voice].targetVolume;
            D_8009E0B8[voice].autoVolume = 0;
        }
    } else if (D_8009E0B8[voice].volumeStep < 0) {
        if (D_8009E0B8[voice].currentVolume <=
            D_8009E0B8[voice].targetVolume) {
            D_8009E0B8[voice].currentVolume =
                D_8009E0B8[voice].targetVolume;
            D_8009E0B8[voice].autoVolume = 0;
        }
    }

    currentVolume = D_8009E0B8[voice].currentVolume;
    D_801E4BD0.currentVolume = currentVolume;
    scaledMasterVolume = D_801E413C->masterVolume * 0x3FFF;

    leftVolumeTemp =
        ((currentVolume * scaledMasterVolume) / 0x7F) / 0x7F;
    rightVolumeTemp =
        ((currentVolume * scaledMasterVolume) / 0x7F) / 0x7F;

    leftVolumeTemp =
        ((leftVolumeTemp * D_801E4BD0.masterVolume *
          D_801E4BD0.volume) /
         0x7F) /
        0x7F;
    rightVolumeTemp =
        ((rightVolumeTemp * D_801E4BD0.masterVolume *
          D_801E4BD0.volume) /
         0x7F) /
        0x7F;

    if (D_801E4BD0.currentPan < 0x40) {
        leftVolume = leftVolumeTemp;
        rightVolume =
            (rightVolumeTemp * D_801E4BD0.currentPan) / 0x40;
    } else {
        leftVolume =
            (leftVolumeTemp * (0x7F - D_801E4BD0.currentPan)) / 0x40;
        rightVolume = rightVolumeTemp;
    }

    if (D_801E4BD0.masterPan < 0x40) {
        rightVolume = (rightVolume * D_801E4BD0.masterPan) / 0x40;
    } else {
        leftVolume =
            (leftVolume * (0x7F - D_801E4BD0.masterPan)) / 0x40;
    }

    if (D_801E4BD0.pan < 0x40) {
        rightVolume = (rightVolume * D_801E4BD0.pan) / 0x40;
    } else {
        leftVolume = (leftVolume * (0x7F - D_801E4BD0.pan)) / 0x40;
    }

    if (D_801E3FB0 == 1) {
        if (rightVolume > leftVolume) {
            leftVolume = rightVolume;
        } else {
            rightVolume = leftVolume;
        }
    }

    D_8009DF20[registerOffset] = leftVolume;
    D_8009DF20[registerOffset + 1] = rightVolume;
    D_8009E0A0[voice] |= 3;
}
