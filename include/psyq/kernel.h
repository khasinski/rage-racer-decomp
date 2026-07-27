#ifndef RAGE_PC_PSYQ_KERNEL_H
#define RAGE_PC_PSYQ_KERNEL_H

#include <sys/types.h>

#include "common.h"

typedef void (*KernelCallback)(void);

void copyKernelWords(u_long *dst, u_long *src, u_long count, long unused) asm("func_8006DB40");
long VSync(long mode) asm("func_8006DD30");
void waitVSync(long target, long timeoutFrames) asm("func_8006DE78");
void ChangeClearRCnt(long clear) asm("func_8006DF14");
void ChangeClearInterruptMask(long index, long clear) asm("func_8006DF24");
void KernelCallbackSlot3(void) asm("func_8006DF34");
void KernelCallbackSlot2(void) asm("func_8006DF64");
/* BIOS DMA callback installer: spec 0=MDECin 1=MDECout 2=GPU 3=CD-ROM 4=SPU.
 * Was declared ResetCallback here; that was wrong (docs/names.md 17). */
void DMACallback(long spec, long callback) asm("func_8006DF94");
void VSyncCallback(long callback) asm("func_8006DFC4");
void KernelCallbackSlot5(void) asm("func_8006DFF8");
void KernelCallbackSlot4(void) asm("func_8006E028");
void KernelCallbackSlot6(void) asm("func_8006E058");
long GetKernelStatus(void) asm("func_8006E088");
long GetIntrMask(void) asm("func_8006E098");
long SetIntrMask(long mask) asm("func_8006E0B0");

void *InitKernelInterrupts(void) asm("func_8006E0CC");
void intrDispatch(void) asm("func_8006E1A8");
KernelCallback SetKernelInterruptCallback(long index, KernelCallback callback) asm("func_8006E390");
void *StartKernelInterrupts(void) asm("func_8006E590");
void clearKernelInterruptState(u_long *dst, long count) asm("func_8006E618");
void SysEnqIntRP(void *rp) asm("func_8006E644");
void ReturnFromException(void) asm("func_8006E654");
void ResetEntryInt(void) asm("func_8006E664");
void HookEntryInt(void *entry) asm("func_8006E674");
long SaveKernelRegisters(void *state) asm("func_8006E684");
void RestoreKernelRegisters(void *state, long ret) asm("func_8006E6C0");

void *startIntrVSync(void) asm("func_8006E704");
void intrVSyncDispatcher(void) asm("func_8006E75C");
void setIntrVSync(long index, void *callback) asm("func_8006E7D4");
void clearIntrVSyncCallbacks(u_long *dst, long count) asm("func_8006E800");
void *startIntrDMA(void) asm("func_8006E82C");
void intrDMADispatcher(void) asm("func_8006E87C");
u_long setIntrDMA(long index, u_long callback) asm("func_8006EA00");
void clearIntrDMACallbacks(u_long *dst, long count) asm("func_8006EAA8");
long SetDMAInterruptState(long value) asm("func_8006EAD4");
long GetDMAInterruptState(void) asm("func_8006EAEC");

/* LibRef47 narrows `target` to unsigned short; kept long here because narrowing
 * it changes the truncation gcc emits at the call sites. */
long SetRCnt(long spec, long target, long mode) asm("func_80071F7C");
long GetRCnt(long spec) asm("func_8007201C");
long StartRCnt(long spec) asm("func_80072054");
long StopRCnt(long spec) asm("func_80072088");
long ResetRCnt(long spec) asm("func_800720BC");
void EnterCriticalSection(void) asm("func_80063210");
void ExitCriticalSection(void) asm("func_80063220");
long OpenEvent(long desc, long spec, long mode, long func) asm("func_800631A0");
void CloseEvent(long event) asm("func_800631B0");
long TestEvent(long event) asm("func_800631C0");
void EnableEvent(long event) asm("func_800631D0");
void DisableEvent(long event) asm("func_800631E0");
void WaitEvent(long event) asm("func_8007B004");

long BiosFileOpen(void *path, long mode) asm("func_80063230");
long BiosFileSeek(long fd, long offset, long whence) asm("func_80063240");
long BiosFileRead(long fd, void *buf, long len) asm("func_80063250");
long BiosFileWrite(long fd, void *buf, long len) asm("func_80063260");
long BiosFileClose(long fd) asm("func_80063270");
long BiosFormatDevice(void *device) asm("func_80063280");
void *BiosFirstFile(char *path, void *entry) asm("func_80063290");
void *BiosNextFile(void *entry) asm("func_800632A0");

#endif
