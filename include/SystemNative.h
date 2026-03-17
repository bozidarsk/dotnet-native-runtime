#ifndef SYSTEM_NATIVE_H
#define SYSTEM_NATIVE_H

#include <stdint.h>
#include <stddef.h>

#include "SystemNative/memory.h"
#include "SystemNative/io.h"
#include "SystemNative/env.h"
#include "SystemNative/console.h"
#include "SystemNative/monitor.h"
#include "SystemNative/dl.h"

// dynamic
void SystemNative_GetLoadLibraryError(void);
void SystemNative_LoadLibrary();

// ???
void SystemNative_LogError();

#endif
