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
 * The libsnd VAB ids of the loaded banks, one per bank slot.
 * InitSoundWithVab fills it from SsVabOpenHead; every key-on passes an
 * element as the vabId argument of func_80077C7C / func_80078130
 * (`g_VabIds[slot]`), and the callers that only ever use the first bank read
 * `g_VabIds[0]`.
 */
extern s16 g_VabIds[] asm("D_801E6CA8");

void SetSequenceVolume(s32 volume);
void RefreshSequenceVolumeScale(void) asm("func_8005E7DC");
void SetSequenceVolumeScale(s32 scale);
void PlaySequence(void) asm("func_8005E88C");
void StopSequence(void) asm("func_8005E8B8");
void StartSequenceFadeOut(void) asm("func_8005E8E0");
void UpdateSequenceFadeOut(void) asm("func_8005E900");
void ApplyDuckedSequenceAudio(void) asm("func_8005EA14");
void ApplyCurrentSequenceAudio(void) asm("func_8005EA6C");
void SetMasterVolumeMono(s16 volume);
void SetReverbDepth(s32 left, s32 right) asm("func_8005B190");
void SetReverbPreset(s32 type, s32 left, s32 right) asm("func_8005B204");
void PlaySoundSlotVoice(s32 slot, s32 tone, s32 vabSlot) asm("func_8005B2F0");
void StopSoundSlotVoice(s32 slot);
void SetSoundSlotVoiceEnabled(s32 slot, s32 enabled);
void SetSoundSlotVoicesEnabled(s32 enabled) asm("func_8005B40C");
void SetEffectVoicesEnabled(s32 enabled);
void ResetSoundState(void);
int InitSoundWithVab(s32 header, s32 body) asm("func_8005B5C4");
int InitSoundRuntime(void) asm("func_8005B6E4");
s32 CloseVabOnlyAudioSlot(s32 slot) asm("func_8005B948");
s32 CloseLoadedAudioSlots(void) asm("func_8005B9CC");
void CloseExtraVabSlot(void) asm("func_8005BC14");
void ShutdownSoundSystem(void) asm("func_8005BC80");
void SetEffectVolumeScale(s32 scale);
void SetLoadedTableVolumeScale(s32 scale);
void SetSequenceVolumeSetting(s32 setting) asm("func_8005BD84");
/* The effect-side twin of SetSequenceVolumeSetting: clamps the 0..15
 * option-screen level and scales it onto g_SoundScale.scale's 0..0x80 range. */
void SetEffectVolumeSetting(s32 setting);
void SetStereoOutput(void) asm("func_8005BE24");
void SetMonoOutput(void) asm("func_8005BE58");
/* Push all three saved audio settings (BGM level, SFX level, mono/stereo) into
 * the sound runtime; run at boot and again after a memory-card load. */
void ApplyAudioSettings(void);
u32 GetLoadedAudioStep(void);
s32 GetActiveAudioSlots(void);
s32 SetSoundToneTableEntry(s32 row, s32 bank, s32 value);
void LoadAudioParameterTable(u16 *table);
s32 StartVabTransferWithTable(s32 header, s32 body, u16 *table) asm("func_8005BA20");
s32 LoadExtraVabSlotWithTable(s32 header, s32 body, s32 table) asm("func_8005BB1C");
void SetPanVoiceTargetVolume(s32 left, s32 right);
void ApplyPanVoiceVolume(void) asm("func_8005BF30");
void StartIndexedEffectVoice(s32 baseTone);
void StopIndexedEffectVoice(void) asm("func_8005C0E4");
void SetIndexedEffectVoice(s32 index, s32 phase, s32 volume) asm("func_8005C104");
void UpdateIndexedEffectVoice(void) asm("func_8005C168");
/*
 * The two positional-cue setters. `cue` is an index into the shared 7-record
 * table at D_800126D0 (stride 0x18); each cue owns a fixed pair of voices, and a
 * call keys that pair on, updates it in place, or keys it off at volume 0.
 * Pitched drives EffectVoice D_801E6D30[4] with a 7.7 note; Stereo drives
 * D_801E6D00[2] with independent volumes. See docs/names.md 1.
 */
void SetPitchedSoundCue(s32 cue, s32 pitch, s32 volume) asm("func_8005C914");
void SetStereoSoundCue(s32 cue, s32 volLeft, s32 volRight) asm("func_8005C31C");
s32 StartSingleSpecialCue(s32 cue, s32 volume) asm("func_8005D414");
void PlaySoundCue(s32 cue) asm("func_8005D6EC");
void ForcePanVoiceEnabled(s32 enabled) asm("func_8005DDB8");
void ForceIndexedEffectVoiceEnabled(s32 enabled) asm("func_8005E058");
s32 InterpolateAudioParameter(s32 param, s32 position, s32 bank) asm("func_8005D8EC");
void UpdateLoadedAudioVoices(s32 position, s32 bank) asm("func_8005D9F8");
void InitEffectVoiceRuntime(void) asm("func_8005DC1C");
void ForceBasicEffectVoicesEnabled(s32 enabled) asm("func_8005DEF0");
void ForcePitchEffectVoicesEnabled(s32 enabled) asm("func_8005E1D0");
void ForceSoundSlotVoicePlayback(s32 enabled);
void ForceAllEffectVoicesEnabled(s32 enabled);
s32 OpenVabSequenceSlot(s32 slot, s32 vabHeader, s32 vabBody, s32 seqData) asm("func_8005E4EC");
void StartVabSlotVoice(s32 voice, s32 unused, s16 vabSlot) asm("func_8005E694");
void StopDirectVoice(s32 voice);
void SetVabSlotVoiceEnabled(s32 voice, s32 enabled, s32 vabSlot);
void SetDefaultReverbDepth(void) asm("func_8005DBB4");
void InitSequenceAudio(void) asm("func_8005DBD8");
void RestoreReverbDepth(s32 enabled);
int CloseAudioSlot(s32 slot) asm("func_8005E600");

#endif
