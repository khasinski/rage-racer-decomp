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
extern volatile u8 g_SndVoiceFlags[];

/*
 * The libsnd VAB ids of the loaded banks, one per bank slot.
 * InitSoundWithVab fills it from SsVabOpenHead; every key-on passes an
 * element as the vabId argument of SsUtKeyOnV / func_80078130
 * (`g_VabIds[slot]`), and the callers that only ever use the first bank read
 * `g_VabIds[0]`.
 */
extern s16 g_VabIds[];

void SetSequenceVolume(s32 volume);
void RefreshSequenceVolumeScale(void);
void SetSequenceVolumeScale(s32 scale);
void PlaySequence(void);
void StopSequence(void);
void StartSequenceFadeOut(void);
void UpdateSequenceFadeOut(void);
void ApplyDuckedSequenceAudio(void);
void ApplyCurrentSequenceAudio(void);
void SetMasterVolumeMono(s16 volume);
void SetReverbDepth(s32 left, s32 right);
void SetReverbPreset(s32 type, s32 left, s32 right);
void PlaySoundSlotVoice(s32 slot, s32 tone, s32 vabSlot);
void StopSoundSlotVoice(s32 slot);
void SetSoundSlotVoiceEnabled(s32 slot, s32 enabled);
void SetSoundSlotVoicesEnabled(s32 enabled);
void SetEffectVoicesEnabled(s32 enabled);
void ResetSoundState(void);
int InitSoundWithVab(s32 header, s32 body);
int InitSoundRuntime(void);
s32 CloseVabOnlyAudioSlot(s32 slot);
s32 CloseLoadedAudioSlots(void);
void CloseExtraVabSlot(void);
void ShutdownSoundSystem(void);
void SetEffectVolumeScale(s32 scale);
void SetLoadedTableVolumeScale(s32 scale);
void SetSequenceVolumeSetting(s32 setting);
/* The effect-side twin of SetSequenceVolumeSetting: clamps the 0..15
 * option-screen level and scales it onto g_SoundScale.scale's 0..0x80 range. */
void SetEffectVolumeSetting(s32 setting);
void SetStereoOutput(void);
void SetMonoOutput(void);
/* Push all three saved audio settings (BGM level, SFX level, mono/stereo) into
 * the sound runtime; run at boot and again after a memory-card load. */
void ApplyAudioSettings(void);
u32 GetLoadedAudioStep(void);
s32 GetActiveAudioSlots(void);
s32 SetSoundToneTableEntry(s32 row, s32 bank, s32 value);
void LoadAudioParameterTable(u16 *table);
s32 StartVabTransferWithTable(s32 header, s32 body, u16 *table);
s32 LoadExtraVabSlotWithTable(s32 header, s32 body, s32 table);
void SetPanVoiceTargetVolume(s32 left, s32 right);
void ApplyPanVoiceVolume(void);
void StartIndexedEffectVoice(s32 baseTone);
void StopIndexedEffectVoice(void);
void SetIndexedEffectVoice(s32 index, s32 phase, s32 volume);
void UpdateIndexedEffectVoice(void);
/*
 * The two positional-cue setters. `cue` is an index into the shared 7-record
 * table at D_800126D0 (stride 0x18); each cue owns a fixed pair of voices, and a
 * call keys that pair on, updates it in place, or keys it off at volume 0.
 * Pitched drives EffectVoice D_801E6D30[4] with a 7.7 note; Stereo drives
 * D_801E6D00[2] with independent volumes. See docs/names.md 1.
 */
void SetPitchedSoundCue(s32 cue, s32 pitch, s32 volume);
void SetStereoSoundCue(s32 cue, s32 volLeft, s32 volRight);
s32 StartSingleSpecialCue(s32 cue, s32 volume);
void PlaySoundCue(s32 cue);
void ForcePanVoiceEnabled(s32 enabled);
void ForceIndexedEffectVoiceEnabled(s32 enabled);
s32 InterpolateAudioParameter(s32 param, s32 position, s32 bank);
void UpdateLoadedAudioVoices(s32 position, s32 bank);
void InitEffectVoiceRuntime(void);
void ForceBasicEffectVoicesEnabled(s32 enabled);
void ForcePitchEffectVoicesEnabled(s32 enabled);
void ForceSoundSlotVoicePlayback(s32 enabled);
void ForceAllEffectVoicesEnabled(s32 enabled);
s32 OpenVabSequenceSlot(s32 slot, s32 vabHeader, s32 vabBody, s32 seqData);
void StartVabSlotVoice(s32 voice, s32 unused, s16 vabSlot);
void StopDirectVoice(s32 voice);
void SetVabSlotVoiceEnabled(s32 voice, s32 enabled, s32 vabSlot);
void SetDefaultReverbDepth(void);
void InitSequenceAudio(void);
void RestoreReverbDepth(s32 enabled);
int CloseAudioSlot(s32 slot);

/* Declared identically by 8 translation units before this
 * header carried them. */

extern s32 g_EngineSoundCurves[];
extern s32 g_EngineSoundMaxRpm;
extern s32 g_SoundSlotActive[];

s32 GetOwnedCarAssetIndex(s32 arg0);

#endif
