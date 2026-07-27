#include "common.h"
#include "game/cd.h"

extern s32 g_CdMixPreset asm("D_8007F5FC");
extern u8 g_CdMixPresets[] asm("D_8007F5A8");
extern u8 g_CdVolume asm("D_8009B194");
extern s32 g_CdMixFullLL asm("D_8009B184");
extern s32 g_CdMixFullLR asm("D_8009B188");
extern s32 g_CdMixFullRR asm("D_8009B18C");
extern s32 g_CdMixFullRL asm("D_8009B190");
extern s32 g_CdMixLL asm("D_8009B174");
extern s32 g_CdMixLR asm("D_8009B178");
extern s32 g_CdMixRR asm("D_8009B17C");
extern s32 g_CdMixRL asm("D_8009B180");


void GameSetCdVolume(s32 arg0) asm("func_80042FA0");
void GameSetCdVolume(s32 arg0) {
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

    GameStepCdVolumeFade();
}
