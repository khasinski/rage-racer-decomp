#ifndef RAGE_PC_PSYQ_KERNEL_H
#define RAGE_PC_PSYQ_KERNEL_H

#include <sys/types.h>

#include "common.h"

typedef void (*KernelCallback)(void);

void copyKernelWords(u_long *dst, u_long *src, u_long count, long unused);
long VSync(long mode);
void waitVSync(long target, long timeoutFrames);
void ChangeClearRCnt(long clear) asm("func_8006DF14");
void ChangeClearInterruptMask(long index, long clear) asm("func_8006DF24");
void KernelCallbackSlot3(void) asm("func_8006DF34");
void KernelCallbackSlot2(void) asm("func_8006DF64");
/* BIOS DMA callback installer: spec 0=MDECin 1=MDECout 2=GPU 3=CD-ROM 4=SPU.
 * Was declared ResetCallback here; that was wrong (docs/names.md 17). */
void DMACallback(long spec, long callback);
void VSyncCallback(long callback) asm("func_8006DFC4");
void KernelCallbackSlot5(void);
void KernelCallbackSlot4(void);
void KernelCallbackSlot6(void);
long GetKernelStatus(void);
long GetIntrMask(void);
long SetIntrMask(long mask);

void *InitKernelInterrupts(void) asm("func_8006E0CC");
void intrDispatch(void);
KernelCallback SetKernelInterruptCallback(long index, KernelCallback callback) asm("func_8006E390");
void *StartKernelInterrupts(void) asm("func_8006E590");
void clearKernelInterruptState(u_long *dst, long count);
void SysEnqIntRP(void *rp) asm("func_8006E644");
void ReturnFromException(void) asm("func_8006E654");
void ResetEntryInt(void) asm("func_8006E664");
void HookEntryInt(void *entry) asm("func_8006E674");
long SaveKernelRegisters(void *state) asm("func_8006E684");
void RestoreKernelRegisters(void *state, long ret) asm("func_8006E6C0");

void *startIntrVSync(void);
void intrVSyncDispatcher(void);
void setIntrVSync(long index, void *callback) asm("func_8006E7D4");
void clearIntrVSyncCallbacks(u_long *dst, long count);
void *startIntrDMA(void);
void intrDMADispatcher(void);
u_long setIntrDMA(long index, u_long callback) asm("func_8006EA00");
void clearIntrDMACallbacks(u_long *dst, long count);
long SetDMAInterruptState(long value);
long GetDMAInterruptState(void);

/* LibRef47 narrows `target` to unsigned short; kept long here because narrowing
 * it changes the truncation gcc emits at the call sites. */
long SetRCnt(long spec, long target, long mode);
long GetRCnt(long spec);
long StartRCnt(long spec);
long StopRCnt(long spec);
long ResetRCnt(long spec);
void EnterCriticalSection(void) asm("func_80063210");
void ExitCriticalSection(void) asm("func_80063220");
long OpenEvent(long desc, long spec, long mode, long func) asm("func_800631A0");
void CloseEvent(long event) asm("func_800631B0");
long TestEvent(long event) asm("func_800631C0");
void EnableEvent(long event) asm("func_800631D0");
void DisableEvent(long event) asm("func_800631E0");
void WaitEvent(long event);

long BiosFileOpen(void *path, long mode) asm("func_80063230");
long BiosFileSeek(long fd, long offset, long whence);
long BiosFileRead(long fd, void *buf, long len);
long BiosFileWrite(long fd, void *buf, long len);
long BiosFileClose(long fd);
long BiosFormatDevice(void *device);
void *BiosFirstFile(char *path, void *entry);
void *BiosNextFile(void *entry) asm("func_800632A0");

#endif
