#ifndef SYSTEM_NATIVE_MEMORY_H
#define SYSTEM_NATIVE_MEMORY_H

#include <stdint.h>
#include <stddef.h>

void SystemNative_Free(void* ptr);
void* SystemNative_Malloc(size_t size);
void* SystemNative_MMap(void* address, uint64_t length, int32_t /*PAL*/ protection, int32_t /*PAL*/ flags, intptr_t fd, int64_t offset);
// void SystemNative_MProtect(void); // cannot find implementation
int32_t SystemNative_MUnmap(void* address, uint64_t length);

#endif
