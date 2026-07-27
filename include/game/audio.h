#ifndef GAME_AUDIO_H
#define GAME_AUDIO_H

#include "common.h"

/*
 * Sound voice work buffer, two regions keyed by hardware voice (0..23).
 * g_SndVoiceRegs at 0x8009DF20, stride 0x10, is a straight shadow of the SPU
 * voice registers: SsUtFlush copies +0/+2/+4/+6/+8/+0xA into volL, volR, pitch,
 * start address and the two ADSR words, each gated by a bit of
 * g_SndVoiceFlags (1/4/8/0x10). g_SndVoiceState at 0x8009E0B8, stride 0x34, is
 * libsnd's own record: +0xC note, +0x10 actual program, +0x14 tone.
 * (This block previously described +0 as a note and +2 as fine detune, and gave
 * the 0x34 record's fields as pitch/level/program; both were wrong, taken from
 * the mislabelled SpuVmKeyOnCore prototype whose 2nd/3rd arguments are really
 * the left and right volumes.)
 */
extern volatile u8 g_SndVoiceFlags[] asm("D_8009E0A0");

/*
 * Master scale applied to every sound-effect voice volume, 0..0x80.
 * GameSetEffectVolumeScale clamps into that range; the voice code multiplies a
 * cue's nominal volume by it before writing the SPU. This is the `.scale` word
 * of the SoundScale record at 0x801E6CA4 (see game/sound.h), which is the only
 * field addressed as a scalar.
 */
extern s32 g_EffectVolumeScale asm("D_801E6CA4");

/*
 * The libsnd VAB ids of the loaded banks, one per bank slot.
 * GameInitSoundWithVab fills it from SsVabOpenHead; every key-on passes an
 * element as the vabId argument of func_80077C7C / func_80078130
 * (`g_VabIds[slot]`), and the callers that only ever use the first bank read
 * `g_VabIds[0]`.
 */
extern s16 g_VabIds[] asm("D_801E6CA8");

void GameSetSequenceVolume(s32 volume) asm("func_8005E7A0");
void GameRefreshSequenceVolumeScale(void) asm("func_8005E7DC");
void GameSetSequenceVolumeScale(s32 scale) asm("func_8005E834");
void GamePlaySequence(void) asm("func_8005E88C");
void GameStopSequence(void) asm("func_8005E8B8");
void GameStartSequenceFadeOut(void) asm("func_8005E8E0");
void GameUpdateSequenceFadeOut(void) asm("func_8005E900");
void GameApplyDuckedSequenceAudio(void) asm("func_8005EA14");
void GameApplyCurrentSequenceAudio(void) asm("func_8005EA6C");
void GameSetMasterVolumeMono(s16 volume) asm("func_8005EAA8");
void GameStartMemoryCardEvents(void) asm("func_8005EAD0");
void GameStopMemoryCardEvents(void) asm("func_8005EAF8");
void GameSetReverbDepth(s32 left, s32 right) asm("func_8005B190");
void GameSetReverbPreset(s32 type, s32 left, s32 right) asm("func_8005B204");
void GamePlaySoundSlotVoice(s32 slot, s32 tone, s32 vabSlot) asm("func_8005B2F0");
void GameStopSoundSlotVoice(s32 slot) asm("func_8005B360");
void GameSetSoundSlotVoiceEnabled(s32 slot, s32 enabled) asm("func_8005B388");
void GameSetSoundSlotVoicesEnabled(s32 enabled) asm("func_8005B40C");
void GameSetEffectVoicesEnabled(s32 enabled) asm("func_8005B468");
void GameResetSoundState(void) asm("func_8005B488");
int GameInitSoundWithVab(s32 header, s32 body) asm("func_8005B5C4");
int GameInitSoundRuntime(void) asm("func_8005B6E4");
int GameCloseVabOnlyAudioSlot(s32 slot) asm("func_8005B948");
int GameCloseLoadedAudioSlots(void) asm("func_8005B9CC");
void GameCloseExtraVabSlot(void) asm("func_8005BC14");
void GameShutdownSoundSystem(void) asm("func_8005BC80");
void GameSetEffectVolumeScale(s32 scale) asm("func_8005BD2C");
void GameSetLoadedTableVolumeScale(s32 scale) asm("func_8005BD58");
void GameSetSequenceVolumeSetting(s32 setting) asm("func_8005BD84");
/* The effect-side twin of GameSetSequenceVolumeSetting: clamps the 0..15
 * option-screen level and scales it onto g_EffectVolumeScale's 0..0x80 range. */
void GameSetEffectVolumeSetting(s32 setting) asm("func_8005BDD4");
void GameSetStereoOutput(void) asm("func_8005BE24");
void GameSetMonoOutput(void) asm("func_8005BE58");
/* Push all three saved audio settings (BGM level, SFX level, mono/stereo) into
 * the sound runtime; run at boot and again after a memory-card load. */
void GameApplyAudioSettings(void) asm("func_80021224");
u32 GameGetLoadedAudioStep(void) asm("func_8005BE88");
s32 GameGetActiveAudioSlots(void) asm("func_8005BE98");
s32 GameSetSoundToneTableEntry(s32 row, s32 bank, s32 value) asm("func_8005B040");
void GameLoadAudioParameterTable(u16 *table) asm("func_8005B070");
s32 GameStartVabTransferWithTable(s32 header, s32 body, u16 *table) asm("func_8005BA20");
s32 GameLoadExtraVabSlotWithTable(s32 header, s32 body, u16 *table) asm("func_8005BB1C");
void GameSetPanVoiceTargetVolume(s32 left, s32 right) asm("func_8005BEA8");
void GameApplyPanVoiceVolume(void) asm("func_8005BF30");
void GameStartIndexedEffectVoice(s32 baseTone) asm("func_8005C09C");
void GameStopIndexedEffectVoice(void) asm("func_8005C0E4");
void GameSetIndexedEffectVoice(s32 index, s32 phase, s32 volume) asm("func_8005C104");
void GameUpdateIndexedEffectVoice(void) asm("func_8005C168");
/*
 * The two positional-cue setters. `cue` is an index into the shared 7-record
 * table at D_800126D0 (stride 0x18); each cue owns a fixed pair of voices, and a
 * call keys that pair on, updates it in place, or keys it off at volume 0.
 * Pitched drives EffectVoice D_801E6D30[4] with a 7.7 note; Stereo drives
 * D_801E6D00[2] with independent volumes. See docs/names.md 1.
 */
void GameSetPitchedSoundCue(s32 cue, s32 pitch, s32 volume) asm("func_8005C914");
void GameSetStereoSoundCue(s32 cue, s32 volLeft, s32 volRight) asm("func_8005C31C");
s32 GameStartSingleSpecialCue(s32 cue, s32 volume) asm("func_8005D414");
void GamePlaySoundCue(s32 cue) asm("func_8005D6EC");
void GameForcePanVoiceEnabled(s32 enabled) asm("func_8005DDB8");
void GameForceIndexedEffectVoiceEnabled(s32 enabled) asm("func_8005E058");
s32 GameInterpolateAudioParameter(s32 param, s32 position, s32 bank) asm("func_8005D8EC");
void GameUpdateLoadedAudioVoices(s32 position, s32 bank) asm("func_8005D9F8");
void GameInitEffectVoiceRuntime(void) asm("func_8005DC1C");
void GameForceBasicEffectVoicesEnabled(s32 enabled) asm("func_8005DEF0");
void GameForcePitchEffectVoicesEnabled(s32 enabled) asm("func_8005E1D0");
void GameForceSoundSlotVoicePlayback(s32 enabled) asm("func_8005E364");
void GameForceAllEffectVoicesEnabled(s32 enabled) asm("func_8005E4A4");
void GameOpenVabSequenceSlot(s32 slot, s32 vabHeader, s32 vabBody, s32 seqData) asm("func_8005E4EC");
void GameStartVabSlotVoice(s32 voice, s32 unused, s16 vabSlot) asm("func_8005E694");
void GameStopDirectVoice(s32 voice) asm("func_8005E6F4");
void GameSetVabSlotVoiceEnabled(s32 voice, s32 enabled, s32 vabSlot) asm("func_8005E718");
void GameSetDefaultReverbDepth(void) asm("func_8005DBB4");
void GameInitSequenceAudio(void) asm("func_8005DBD8");
void GameRestoreReverbDepth(s32 enabled) asm("func_8005DD74");
int GameCloseAudioSlot(s32 slot) asm("func_8005E600");

#endif
