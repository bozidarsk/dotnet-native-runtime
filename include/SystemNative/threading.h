#ifndef SYSTEM_NATIVE_THREADING_H
#define SYSTEM_NATIVE_THREADING_H

#include <stdint.h>
#include <stddef.h>

typedef struct LowLevelMonitor LowLevelMonitor;

LowLevelMonitor* SystemNative_LowLevelMonitor_Create(void);
void SystemNative_LowLevelMonitor_Destroy(LowLevelMonitor* monitor);
void SystemNative_LowLevelMonitor_Acquire(LowLevelMonitor* monitor);
void SystemNative_LowLevelMonitor_Release(LowLevelMonitor* monitor);
void SystemNative_LowLevelMonitor_Wait(LowLevelMonitor* monitor);
int32_t SystemNative_LowLevelMonitor_TimedWait(LowLevelMonitor *monitor, int32_t timeoutMilliseconds);
void SystemNative_LowLevelMonitor_Signal_Release(LowLevelMonitor* monitor);
int32_t SystemNative_CreateThread(uintptr_t stackSize, void *(*startAddress)(void*), void *parameter);

#endif
