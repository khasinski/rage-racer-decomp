# Function name restoration map

Evidence-based, match-safe renames of `func_XXXXXXXX` placeholder identifiers to
their canonical PSY-Q SDK names. Every rename keeps the emitted linker symbol
pinned to the original `func_XXXXXXXX` address via an `asm("func_XXXXXXXX")` label
(call-site aliases) or via the already-pinned header declaration in the including
TU (definitions), so codegen stays byte-identical.

Build verified: `make check VERSION=PAL` -> `build/PAL/main.exe` hashes
`2913e15648eddef40821c5f666460abc04155ee6` (unchanged from baseline).

Evidence column: the `include/psyq/*.h` line whose `NAME(...) asm("func_XXXX")`
declaration is the blessed address->name mapping.

Total symbols renamed: 77

Dropped (not byte-safe, see note below): func_80063230 -> BiosFileOpen in
src/main/PAL/lib/libapi/exit_critical_section.c.

| func_ address | SDK name | kind | evidence (header:line) | files |
|---|---|---|---|---|
| func_80063240 | BiosFileSeek | BIOS trampoline array (asm-pinned) | include/psyq/kernel.h:68 + B(0x33) FileSeek | src/main/PAL/lib/libapi/bios_file_stubs.c |
| func_80063250 | BiosFileRead | BIOS trampoline array (asm-pinned) | include/psyq/kernel.h:69 + B(0x34) FileRead | src/main/PAL/lib/libapi/bios_file_stubs.c |
| func_80063260 | BiosFileWrite | BIOS trampoline array (asm-pinned) | include/psyq/kernel.h:70 + B(0x35) FileWrite | src/main/PAL/lib/libapi/bios_file_stubs.c |
| func_80063270 | BiosFileClose | BIOS trampoline array (asm-pinned) | include/psyq/kernel.h:71 + B(0x36) FileClose | src/main/PAL/lib/libapi/bios_file_stubs.c |
| func_80063280 | BiosFormatDevice | BIOS trampoline array (asm-pinned) | include/psyq/kernel.h:72 + B(0x41) selector | src/main/PAL/lib/libapi/bios_file_stubs.c |
| func_80063290 | BiosFirstFile | BIOS trampoline array (asm-pinned) | include/psyq/kernel.h:73 + B(0x42) selector | src/main/PAL/lib/libapi/bios_file_stubs.c |
| func_800632A0 | BiosNextFile | BIOS trampoline array (asm-pinned) | include/psyq/kernel.h:74 + B(0x43) selector | src/main/PAL/lib/libapi/bios_file_stubs.c |
| func_80064C7C | GetClut | call-site alias (local proto pinned) | include/psyq/gpu.h:254 | src/main/PAL/main/menu/DrawTeamLogoCanvas.c |
| func_80064F30 | SetPolyF4 | call-site alias (local proto pinned) | include/psyq/gpu.h:207 | src/main/PAL/main/race/DrawWrongWayWarning.c |
| func_80064F80 | SetSprt8 | call-site alias (local proto pinned) | include/psyq/gpu.h:211 | src/main/PAL/main/race/DrawWrongWayWarning.c |
| func_80064FA8 | SetSprt | call-site alias (local proto pinned) | include/psyq/gpu.h:213 | src/main/PAL/lib/libcd/read.c |
| func_80065A90 | ClearImage | call-site alias (local proto pinned) | include/psyq/gpu.h:242 | src/main/PAL/main/fmv/PresentFmvFrame.c |
| func_80065B24 | LoadImage | call-site alias (local proto pinned) | include/psyq/gpu.h:243 | src/main/PAL/main/menu/DrawTeamLogoCanvas.c |
| func_80068180 | MemFill | call-site alias (local proto pinned) | include/psyq/gpu.h:288 | src/main/PAL/lib/libgpu/gpu_timeout.c |
| func_80069A00 | SetDQA | call-site alias (local proto pinned) | include/psyq/gte.h:37 | src/main/PAL/main/render/SetFogNear.c |
| func_80069A0C | SetDQB | call-site alias (local proto pinned) | include/psyq/gte.h:38 | src/main/PAL/main/render/SetFogNear.c |
| func_80069A38 | SetFarColor | call-site alias (local proto pinned) | include/psyq/gte.h:43 | src/main/PAL/main/track/SetEnvPaletteTable.c |
| func_80069B14 | Intpl | call-site alias (local proto pinned) | include/psyq/gte.h:57 | src/main/PAL/main/track/SetEnvPaletteTable.c |
| func_8006A058 | StSetRing | call-site alias (local proto pinned) | include/psyq/cd.h:106 | src/main/PAL/main/fmv/OpenFmvStream.c |
| func_8006A0AC | CdGetToc2 | call-site alias (local proto pinned) | include/psyq/cd.h:94 | src/main/PAL/lib/libpress/stream_setup.c |
| func_8006A3E8 | CdStatus | call-site alias (local proto pinned) | include/psyq/cd.h:87 | 2 files |
| func_8006A3F8 | CdMode | call-site alias (local proto pinned) | include/psyq/cd.h:88 | src/main/PAL/lib/libcd/read_callbacks.c |
| func_8006A418 | CdLastPos | call-site alias (local proto pinned) | include/psyq/cd.h:90 | src/main/PAL/lib/libcd/read_callbacks.c |
| func_8006A428 | CD_init | call-site alias (local proto pinned) | include/psyq/cd.h:174 | src/main/PAL/lib/libcd/initialization_and_toc.c |
| func_8006A494 | CdFlush | call-site alias (local proto pinned) | include/psyq/cd.h:85 | src/main/PAL/lib/libcd/read_callbacks.c |
| func_8006A534 | CdSync | call-site alias (local proto pinned) | include/psyq/cd.h:123 | 2 files |
| func_8006A554 | CdReady | call-site alias (local proto pinned) | include/psyq/cd.h:124 | src/main/PAL/lib/libcd/read.c |
| func_8006A574 | CdSyncCallback | call-site alias (local proto pinned) | include/psyq/cd.h:126 | 3 files |
| func_8006A58C | CdReadyCallback | call-site alias (local proto pinned) | include/psyq/cd.h:127 | 3 files |
| func_8006A6DC | CdControlF | call-site alias (local proto pinned) | include/psyq/cd.h:121 | src/main/PAL/lib/libcd/read_callbacks.c |
| func_8006A808 | CdControlB | call-site alias (local proto pinned) | include/psyq/cd.h:122 | 2 files |
| func_8006A9D8 | CdIntToPos | call-site alias (local proto pinned) | include/psyq/cd.h:91 | src/main/PAL/lib/libcd/iso_cache_file.c |
| func_8006AADC | CdPosToInt_Local | call-site alias (local proto pinned) | include/psyq/cd.h:92 | src/main/PAL/lib/libcd/read_callbacks.c |
| func_8006B0D4 | CD_sync | call-site alias (local proto pinned) | include/psyq/cd.h:175 | 2 files |
| func_8006B620 | CD_cw | call-site alias (local proto pinned) | include/psyq/cd.h:177 | src/main/PAL/lib/libcd/command_control.c |
| func_8006BAF0 | CD_flush | call-site alias (local proto pinned) | include/psyq/cd.h:97 | 4 files |
| func_8006C83C | DS_searchdir | call-site alias (local proto pinned) | include/psyq/cd.h:101 | src/main/PAL/lib/libds/search_file.c |
| func_8006CDC0 | StClearRing | call-site alias (local proto pinned) | include/psyq/cd.h:102 | src/main/PAL/lib/libpress/stream_setup.c |
| func_8006CF08 | StGetBackloc | call-site alias (local proto pinned) | include/psyq/cd.h:103 | src/main/PAL/main/fmv/DecodeFmvFrame.c |
| func_8006CF68 | StSetStream | call-site alias (local proto pinned) | include/psyq/cd.h:107 | src/main/PAL/main/fmv/OpenFmvStream.c |
| func_8006D1D0 | StCdInterrupt | call-site alias (local proto pinned) | include/psyq/cd.h:113 | src/main/PAL/main/fmv/OpenFmvStream.c |
| func_8006DF34 | KernelCallbackSlot3 | call-site alias (local proto pinned) | include/psyq/kernel.h:15 | 2 files |
| func_8006DF64 | KernelCallbackSlot2 | call-site alias (local proto pinned) | include/psyq/kernel.h:16 | src/main/PAL/lib/libcd/drive_initialization.c |
| func_8006E088 | GetKernelStatus | call-site alias (local proto pinned) | include/psyq/kernel.h:24 | 3 files |
| func_8006E0B0 | SetIntrMask | call-site alias (local proto pinned) | include/psyq/kernel.h:26 | 2 files |
| func_8006EAEC | GetDMAInterruptState | call-site alias (local proto pinned) | include/psyq/kernel.h:49 | 2 files |
| func_8006EAFC | SsSetMVol | call-site alias (local proto pinned) | include/psyq/snd.h:155 | src/main/PAL/main/audio/ApplyCurrentSequenceAudio.c |
| func_80070D70 | SsSeqReadDeltaTime | call-site alias (local proto pinned) | include/psyq/snd.h:165 | src/main/PAL/lib/libapi/controller_data.c |
| func_80070E28 | SsUnpackAdsr | call-site alias (local proto pinned) | include/psyq/snd.h:166 | src/main/PAL/lib/libsnd/SsSeqApplyNrpn.c |
| func_80070E84 | SsPackAdsr | call-site alias (local proto pinned) | include/psyq/snd.h:167 | src/main/PAL/lib/libsnd/SsSeqApplyNrpn.c |
| func_80071B0C | _SsInitTables | call-site alias (local proto pinned) | include/psyq/snd.h:177 | src/main/PAL/lib/libsnd/ssinit.c |
| func_80072734 | _SsSndStop | definition (renamed, header pin) | include/psyq/snd.h:124 | src/main/PAL/lib/libsnd/Snd_SetPlayMode.c |
| func_80072AE0 | SsSetMono | call-site alias (local proto pinned) | include/psyq/snd.h:199 | src/main/PAL/main/audio/SetEffectVolumeScale.c |
| func_80072AF4 | SsSetStereo | call-site alias (local proto pinned) | include/psyq/snd.h:200 | src/main/PAL/main/audio/SetEffectVolumeScale.c |
| func_80072C4C | SsVabOpenHeadSticky | definition (renamed, header pin) | include/psyq/snd.h:205 | src/main/PAL/lib/libsnd/SsVabOpen.c |
| func_8007320C | SsUtGetProgAtr | call-site alias (local proto pinned) | include/psyq/snd.h:216 | src/main/PAL/lib/libapi/controller_data.c |
| func_80073314 | SpuVmVSetUp | call-site alias (local proto pinned) | include/psyq/snd.h:217 | 3 files |
| func_800733D8 | SsUtGetVagAtr | call-site alias (local proto pinned) | include/psyq/snd.h:218 | 2 files |
| func_80073614 | SsUtSetReverbType | call-site alias (local proto pinned) | include/psyq/snd.h:129 | src/main/PAL/lib/libsnd/SsSeqApplyNrpn.c |
| func_800736C8 | SsUtReverbOn | call-site alias (local proto pinned) | include/psyq/snd.h:131 | src/main/PAL/lib/libsnd/SsSeqApplyNrpn.c |
| func_800736E8 | SsUtReverbOff | call-site alias (local proto pinned) | include/psyq/snd.h:132 | src/main/PAL/lib/libsnd/SsSeqApplyNrpn.c |
| func_80073708 | SsUtSetReverbFeedback | call-site alias (local proto pinned) | include/psyq/snd.h:133 | src/main/PAL/lib/libsnd/SsSeqApplyNrpn.c |
| func_80073748 | SsUtSetReverbDepth | call-site alias (local proto pinned) | include/psyq/snd.h:128 | src/main/PAL/lib/libsnd/SsSeqApplyNrpn.c |
| func_800737E0 | SsUtSetReverbDelay | call-site alias (local proto pinned) | include/psyq/snd.h:134 | src/main/PAL/lib/libsnd/SsSeqApplyNrpn.c |
| func_80073820 | SsUtSetVagAtr | call-site alias (local proto pinned) | include/psyq/snd.h:219 | 2 files |
| func_800739E8 | SpuVmAlloc | call-site alias (local proto pinned) | include/psyq/snd.h:222 | 2 files |
| func_800749B4 | SpuVmCalculateCurrentPitch | call-site alias (local proto pinned) | include/psyq/snd.h:220 | src/main/PAL/lib/libsnd/SpuVmSeKeyOn.c |
| func_80074A6C | SpuVmCalculateTonePitch | call-site alias (local proto pinned) | include/psyq/snd.h:221 | 4 files |
| func_80075FA4 | SsUtFlush | call-site alias (local proto pinned) | include/psyq/snd.h:229 | src/main/PAL/lib/libsnd/SpuVmInit.c |
| func_80076940 | SpuVmSeKeyOff | call-site alias (local proto pinned) | include/psyq/snd.h:231 | src/main/PAL/lib/libsnd/SpuVmSeKeyOn.c |
| func_80078838 | SpuInit | call-site alias (local proto pinned) | include/psyq/spu.h:97 | src/main/PAL/lib/libsnd/ssinit.c |
| func_80078F4C | _spu_writeByIO | call-site alias (local proto pinned) | include/psyq/spu.h:102 | src/main/PAL/lib/libspu/_spu_init.c |
| func_80079B60 | SpuInitMalloc | call-site alias (local proto pinned) | include/psyq/spu.h:116 | src/main/PAL/lib/libsnd/SpuVmInit.c |
| func_8007A1F8 | SpuSetNoiseVoice | call-site alias (local proto pinned) | include/psyq/spu.h:119 | src/main/PAL/lib/libsnd/voice_allocation.c |
| func_8007B014 | SpuTransferStatus | call-site alias (local proto pinned) | include/psyq/spu.h:125 | src/main/PAL/main/menu/UpdateMenuMode.c |
| func_8007B088 | SpuGetKeyStatus | call-site alias (local proto pinned) | include/psyq/spu.h:126 | src/main/PAL/main/audio/SetPitchedSoundCue.c |
| func_8007B294 | _spu_setTransferCompletionFlag | call-site alias (local proto pinned) | include/psyq/spu.h:131 | src/main/PAL/lib/libsnd/SpuVmInit.c |

## Dropped rename: func_80063230 -> BiosFileOpen (ExitCriticalSection.c)

This rename is NOT byte-safe and was dropped from the salvaged set. Unlike the
other seven BIOS trampolines (each the sole object in its own `.c` file), the
`func_80063230` trampoline lives in `ExitCriticalSection.c`, right after an
`INCLUDE_ASM(... func_80063220)` handwritten-asm stub.

`tools/scripts/gen_nonmatching_asm.py` (TEXT_OBJECT_RE, ~line 43) discovers the
end of the `func_80063220` disassembly by matching a following C data object
written literally as `func_80063230[4] __attribute__((section(".text")))`. That
match sets the boundary so the regenerated `func_80063220.s` stops at
0x80063230 (4 instructions).

Renaming the object to `BiosFileOpen[4] asm("func_80063230") __attribute__(...)`
changes the C identifier away from `func_80063230`, so the boundary regex no
longer matches. On the next `make split` (which does `rm -rf asm/...` and
regenerates), `func_80063220.s` is emitted with 8 instructions, swallowing the
16-byte trampoline at 0x80063230. The C array then emits that same trampoline a
second time, duplicating it, growing `.text` by 0x10 (PS-EXE t_size 0x0800b000
-> 0x0800b010) and shifting all following code. Result: build hashes
`cb5243c90db7e4474bd168f851ba6752e1ca4327` instead of the retail
`2913e15648eddef40821c5f666460abc04155ee6`. Reverting this one file to its
`func_80063230[4]` form restores byte-exactness.
