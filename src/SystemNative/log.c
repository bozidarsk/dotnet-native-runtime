#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

#include "SystemNative.h"

void SystemNative_Log(uint8_t* buffer, int32_t length) 
{
	fwrite(buffer, 1, (size_t)length, stdout);
	fflush(stdout);
}

void SystemNative_LogError(uint8_t* buffer, int32_t length) 
{
	fwrite(buffer, 1, (size_t)length, stderr);
	fflush(stderr);
}
