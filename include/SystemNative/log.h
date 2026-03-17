#ifndef SYSTEM_NATIVE_LOG_H
#define SYSTEM_NATIVE_LOG_H

#include <stdint.h>
#include <stddef.h>

void SystemNative_Log(uint8_t* buffer, int32_t length);
void SystemNative_LogError(uint8_t* buffer, int32_t length);

#endif
