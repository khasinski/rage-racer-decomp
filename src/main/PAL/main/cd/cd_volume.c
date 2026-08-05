#include "common.h"
#include "game/cd.h"

extern u8 g_CdVolume;
extern s32 g_CdMixFullLL;
extern s32 g_CdMixFullLR;
extern s32 g_CdMixFullRR;
extern s32 g_CdMixFullRL;
extern s32 g_CdMixLL;
extern s32 g_CdMixLR;
extern s32 g_CdMixRR;
extern s32 g_CdMixRL;

void SetCdVolume(s32 arg0) {
    s32 offset;
    s32 scale;
    s32 magic;
    s32 product;
    s32 value;

    offset = g_CdMixPreset;
    g_CdVolume = arg0;
    scale = g_CdVolume;
    offset <<= 2;

    product = g_CdMixPresets[offset] * scale;
    magic = 0x81020409;
    value = (product / 127) << 12;
    g_CdMixFullLL = value;
    g_CdMixLL = value;

    product = g_CdMixPresets[offset + 1] * scale;
    value = (product / 127) << 12;
    g_CdMixFullLR = value;
    g_CdMixLR = value;

    product = g_CdMixPresets[offset + 2] * scale;
    value = (product / 127) << 12;
    g_CdMixFullRR = value;
    g_CdMixRR = value;

    product = g_CdMixPresets[offset + 3] * scale;
    value = (product / 127) << 12;
    g_CdMixFullRL = value;
    g_CdMixRL = value;

    StepCdVolumeFade();
}

void ApplyCdVolume(void) {
    SetCdVolume(g_CdVolume);
}

void SetCdVolumeSetting(s32 arg0) {
    s32 product = (arg0 << 7) - arg0;

    g_CdVolume = product / 15;
    SetCdVolume(g_CdVolume);
}
