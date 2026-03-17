#ifndef SYSTEM_NATIVE_MONITOR_H
#define SYSTEM_NATIVE_MONITOR_H

#include <stdint.h>
#include <stddef.h>

void SystemNative_LowLevelMonitor_Acquire(void);
void SystemNative_LowLevelMonitor_Create(void);
void SystemNative_LowLevelMonitor_Destroy(void);
void SystemNative_LowLevelMonitor_Release(void);
void SystemNative_LowLevelMonitor_Signal_Release(void);
void SystemNative_LowLevelMonitor_TimedWait(void);
void SystemNative_LowLevelMonitor_Wait(void);

#endif
