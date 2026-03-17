#ifndef SYSTEM_NATIVE_DL_H
#define SYSTEM_NATIVE_DL_H

#include <stdint.h>
#include <stddef.h>

void* SystemNative_LoadLibrary(const char* filename);
void* SystemNative_GetLoadLibraryError(void);
void* SystemNative_GetProcAddress(void* handle, const char* symbol);
void SystemNative_FreeLibrary(void* handle);

#endif
