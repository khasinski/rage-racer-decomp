#ifndef RAGE_PC_PSYQ_KERNEL_H
#define RAGE_PC_PSYQ_KERNEL_H

#include "common.h"

typedef void (*KernelCallback)(void);

void copyKernelWords(u32 *dst, u32 *src, u32 count, s32 unused) asm("func_8006DB40");
s32 VSync(s32 mode) asm("func_8006DD30");
void waitVSync(s32 target, s32 timeoutFrames) asm("func_8006DE78");
void ChangeClearRCnt(s32 clear) asm("func_8006DF14");
void ChangeClearInterruptMask(s32 index, s32 clear) asm("func_8006DF24");
void KernelCallbackSlot3(void) asm("func_8006DF34");
void KernelCallbackSlot2(void) asm("func_8006DF64");
void ResetCallback(s32 spec, s32 callback) asm("func_8006DF94");
void VSyncCallback(s32 callback) asm("func_8006DFC4");
void KernelCallbackSlot5(void) asm("func_8006DFF8");
void KernelCallbackSlot4(void) asm("func_8006E028");
void KernelCallbackSlot6(void) asm("func_8006E058");
s32 GetKernelStatus(void) asm("func_8006E088");
s32 GetIntrMask(void) asm("func_8006E098");
s32 SetIntrMask(s32 mask) asm("func_8006E0B0");

void *InitKernelInterrupts(void) asm("func_8006E0CC");
void intrDispatch(void) asm("func_8006E1A8");
KernelCallback SetKernelInterruptCallback(s32 index, KernelCallback callback) asm("func_8006E390");
void *StartKernelInterrupts(void) asm("func_8006E590");
void clearKernelInterruptState(u32 *dst, s32 count) asm("func_8006E618");
void SysEnqIntRP(void *rp) asm("func_8006E644");
void ReturnFromException(void) asm("func_8006E654");
void ResetEntryInt(void) asm("func_8006E664");
void HookEntryInt(void *entry) asm("func_8006E674");
s32 SaveKernelRegisters(void *state) asm("func_8006E684");
void RestoreKernelRegisters(void *state, s32 ret) asm("func_8006E6C0");

void *startIntrVSync(void) asm("func_8006E704");
void intrVSyncDispatcher(void) asm("func_8006E75C");
void setIntrVSync(s32 index, void *callback) asm("func_8006E7D4");
void clearIntrVSyncCallbacks(u32 *dst, s32 count) asm("func_8006E800");
void *startIntrDMA(void) asm("func_8006E82C");
void intrDMADispatcher(void) asm("func_8006E87C");
u32 setIntrDMA(s32 index, u32 callback) asm("func_8006EA00");
void clearIntrDMACallbacks(u32 *dst, s32 count) asm("func_8006EAA8");
s32 SetDMAInterruptState(s32 value) asm("func_8006EAD4");
s32 GetDMAInterruptState(void) asm("func_8006EAEC");

s32 SetRCnt(s32 spec, s32 target, s32 mode) asm("func_80071F7C");
s32 GetRCnt(s32 spec) asm("func_8007201C");
s32 StartRCnt(s32 spec) asm("func_80072054");
s32 StopRCnt(s32 spec) asm("func_80072088");
s32 ResetRCnt(s32 spec) asm("func_800720BC");
void EnterCriticalSection(void) asm("func_80063210");
void ExitCriticalSection(void) asm("func_80063220");
s32 OpenEvent(s32 desc, s32 spec, s32 mode, s32 func) asm("func_800631A0");
void CloseEvent(s32 event) asm("func_800631B0");
s32 TestEvent(s32 event) asm("func_800631C0");
void EnableEvent(s32 event) asm("func_800631D0");
void DisableEvent(s32 event) asm("func_800631E0");
void WaitEvent(s32 event) asm("func_8007B004");

s32 BiosFileOpen(void *path, s32 mode) asm("func_80063230");
s32 BiosFileSeek(s32 fd, s32 offset, s32 whence) asm("func_80063240");
s32 BiosFileRead(s32 fd, void *buf, s32 len) asm("func_80063250");
s32 BiosFileWrite(s32 fd, void *buf, s32 len) asm("func_80063260");
s32 BiosFileClose(s32 fd) asm("func_80063270");
s32 BiosFormatDevice(void *device) asm("func_80063280");
void *BiosFirstFile(char *path, void *entry) asm("func_80063290");
void *BiosNextFile(void *entry) asm("func_800632A0");

#endif
