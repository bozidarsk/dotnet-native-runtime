#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <errno.h>
#include <sys/mman.h>

#include "SystemNative.h"

static int32_t ConvertMMapProtection(int32_t protection) 
{
	if (protection == PAL_PROT_NONE)
		return PROT_NONE;

	if (protection & ~(PAL_PROT_READ | PAL_PROT_WRITE | PAL_PROT_EXEC)) 
	{
		assert(!"Unknown protection.");
		return -1;
	}

	int32_t ret = 0;

	if (protection & PAL_PROT_READ)
		ret |= PROT_READ;
	if (protection & PAL_PROT_WRITE)
		ret |= PROT_WRITE;
	if (protection & PAL_PROT_EXEC)
		ret |= PROT_EXEC;

	assert(ret != -1);
	return ret;
}

static int32_t ConvertMMapFlags(int32_t flags) 
{
	if (flags & ~(PAL_MAP_SHARED | PAL_MAP_PRIVATE | PAL_MAP_ANONYMOUS)) 
	{
		assert(!"Unknown MMap flag.");
		return -1;
	}

	int32_t ret = 0;

	if (flags & PAL_MAP_PRIVATE)
		ret |= MAP_PRIVATE;
	if (flags & PAL_MAP_SHARED)
		ret |= MAP_SHARED;
	if (flags & PAL_MAP_ANONYMOUS)
		ret |= MAP_ANON;

	assert(ret != -1);
	return ret;
}

void SystemNative_Free(void* ptr) 
{
	free(ptr);
}

void* SystemNative_Malloc(size_t size) 
{
	return malloc(size);
}

void* SystemNative_MMap(
	void* address,
	uint64_t length,
	int32_t protection, // bitwise OR of PAL_PROT_*
	int32_t flags,	  // bitwise OR of PAL_MAP_*, but PRIVATE and SHARED are mutually exclusive.
	intptr_t fd,
	int64_t offset
)
{
	if (length > SIZE_MAX) 
	{
		errno = ERANGE;
		return NULL;
	}

	protection = ConvertMMapProtection(protection);
	flags = ConvertMMapFlags(flags);

	if (flags == -1 || protection == -1) 
	{
		errno = EINVAL;
		return NULL;
	}

	// Use ToFileDescriptorUnchecked to allow -1 to be passed for the file descriptor, since managed code explicitly uses -1
	void* ret =mmap64(
		address,
		(size_t)length,
		protection,
		flags,
		(int)fd,
		(off_t)offset
	);

	if (ret == MAP_FAILED)
		return NULL;

	assert(ret != NULL);
	return ret;
}

int32_t SystemNative_MUnmap(void* address, uint64_t length) 
{
	if (length > SIZE_MAX) 
	{
		errno = ERANGE;
		return -1;
	}

	return munmap(address, (size_t)length);
}

void SystemNative_MProtect(void) { assert(!"Not implemented."); }
