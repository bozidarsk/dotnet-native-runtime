#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <poll.h>
#include <stdcountof.h>

#include "SystemNative.h"

static int32_t ConvertOpenFlags(int32_t flags) 
{
	int32_t ret;

	switch (flags & PAL_O_ACCESS_MODE_MASK) 
	{
		case PAL_O_RDONLY:
			ret = O_RDONLY;
			break;
		case PAL_O_RDWR:
			ret = O_RDWR;
			break;
		case PAL_O_WRONLY:
			ret = O_WRONLY;
			break;
		default:
			assert(!"Unknown Open access mode.");
			return -1;
	}

	if (flags & ~(PAL_O_ACCESS_MODE_MASK | PAL_O_CLOEXEC | PAL_O_CREAT | PAL_O_EXCL | PAL_O_TRUNC | PAL_O_SYNC | PAL_O_NOFOLLOW)) 
	{
		assert(!"Unknown Open flag.");
		return -1;
	}

	if (flags & PAL_O_CLOEXEC)
		ret |= O_CLOEXEC;
	if (flags & PAL_O_CREAT)
		ret |= O_CREAT;
	if (flags & PAL_O_EXCL)
		ret |= O_EXCL;
	if (flags & PAL_O_TRUNC)
		ret |= O_TRUNC;
	if (flags & PAL_O_SYNC)
		ret |= O_SYNC;
	if (flags & PAL_O_NOFOLLOW)
		ret |= O_NOFOLLOW;

	assert(ret != -1);
	return ret;
}

static void ConvertFileStatus(const struct stat64* src, FileStatus* dst) 
{
	dst->Dev = (int64_t)src->st_dev;
	dst->RDev = (int64_t)src->st_rdev;
	dst->Ino = (int64_t)src->st_ino;
	dst->Flags = FILESTATUS_FLAGS_NONE;
	dst->Mode = (int32_t)src->st_mode;
	dst->Uid = src->st_uid;
	dst->Gid = src->st_gid;
	dst->Size = src->st_size;

	dst->ATime = src->st_atime;
	dst->MTime = src->st_mtime;
	dst->CTime = src->st_ctime;

	dst->ATimeNsec = src->st_atim.tv_nsec;
	dst->MTimeNsec = src->st_mtim.tv_nsec;
	dst->CTimeNsec = src->st_ctim.tv_nsec;

	// Linux path: until we use statx() instead
	dst->BirthTime = 0;
	dst->BirthTimeNsec = 0;
	dst->UserFlags = 0;
}

static int16_t ConvertPollEventsPalToPlatform(int16_t palEvents) 
{
    // we need to do this for platforms like AIX where PAL_POLL* doesn't
    // match up to their reality; this is PollEvent -> system polling
    int16_t platformEvents = 0;

    if ((palEvents & PAL_POLLIN) != 0)
    	platformEvents |= POLLIN;
    if ((palEvents & PAL_POLLPRI) != 0)
    	platformEvents |= POLLPRI;
    if ((palEvents & PAL_POLLOUT) != 0)
    	platformEvents |= POLLOUT;
    if ((palEvents & PAL_POLLERR) != 0)
    	platformEvents |= POLLERR;
    if ((palEvents & PAL_POLLHUP) != 0)
    	platformEvents |= POLLHUP;
    if ((palEvents & PAL_POLLNVAL) != 0)
    	platformEvents |= POLLNVAL;

    return platformEvents;
}

static int16_t ConvertPollEventsPlatformToPal(int16_t platformEvents) 
{
    // same as the other function, just system -> PollEvent
    int16_t palEvents = 0;

    if ((platformEvents & POLLIN) != 0)
    	palEvents |= PAL_POLLIN;
    if ((platformEvents & POLLPRI) != 0)
    	palEvents |= PAL_POLLPRI;
    if ((platformEvents & POLLOUT) != 0)
    	palEvents |= PAL_POLLOUT;
    if ((platformEvents & POLLERR) != 0)
    	palEvents |= PAL_POLLERR;
    if ((platformEvents & POLLHUP) != 0)
    	palEvents |= PAL_POLLHUP;
    if ((platformEvents & POLLNVAL) != 0)
    	palEvents |= PAL_POLLNVAL;

    return palEvents;
}

int32_t SystemNative_Close(intptr_t fd) 
{
	return close(fd);
}

int32_t SystemNative_Read(intptr_t fd, void* buffer, int32_t bufferSize) 
{
    assert(buffer != NULL || bufferSize == 0);
    assert(bufferSize >= 0);

    if (bufferSize < 0) 
    {
        errno = EINVAL;
        return -1;
    }

    ssize_t count;
    while ((count = read(fd, buffer, (uint32_t)bufferSize)) < 0 && errno == EINTR);

    assert(count >= -1 && count <= bufferSize);
    return (int32_t)count;
}

int32_t SystemNative_Write(intptr_t fd, const void* buffer, int32_t bufferSize) 
{
    assert(buffer != NULL || bufferSize == 0);
    assert(bufferSize >= 0);

    if (bufferSize < 0) 
    {
        errno = ERANGE;
        return -1;
    }

    ssize_t count;
    while ((count = write(fd, buffer, (uint32_t)bufferSize)) < 0 && errno == EINTR);

    assert(count >= -1 && count <= bufferSize);
    return (int32_t)count;
}

intptr_t SystemNative_Open(const char* path, int32_t flags, int32_t mode) 
{
	flags = ConvertOpenFlags(flags);
	if (flags == -1) 
	{
		errno = EINVAL;
		return -1;
	}

	int result;
	while ((result = open(path, flags, (mode_t)mode)) < 0 && errno == EINTR);

	return result;
}

int32_t SystemNative_PRead(intptr_t fd, void* buffer, int32_t bufferSize, int64_t fileOffset) 
{
	assert(buffer != NULL);
	assert(bufferSize >= 0);

	ssize_t count;
	while ((count = pread(fd, buffer, (uint32_t)bufferSize, (off_t)fileOffset)) < 0 && errno == EINTR);

	assert(count >= -1 && count <= bufferSize);
	return (int32_t)count;
}

int32_t SystemNative_PWrite(intptr_t fd, void* buffer, int32_t bufferSize, int64_t fileOffset) 
{
	assert(buffer != NULL);
	assert(bufferSize >= 0);

	ssize_t count;
	while ((count = pwrite(fd, buffer, (uint32_t)bufferSize, (off_t)fileOffset)) < 0 && errno == EINTR);

	assert(count >= -1 && count <= bufferSize);
	return (int32_t)count;
}

intptr_t SystemNative_Dup(intptr_t oldfd) 
{
	int result;
	while ((result = fcntl(oldfd, F_DUPFD_CLOEXEC, 0)) < 0 && errno == EINTR);

	return result;
}

int32_t SystemNative_FLock(intptr_t fd, int32_t operation) 
{
	int32_t result;
	while ((result = flock(fd, operation)) < 0 && errno == EINTR);

	return result;
}

int32_t SystemNative_Stat(const char* path, FileStatus* output) 
{
	struct stat64 result;
	int ret;

	while ((ret = stat64(path, &result)) < 0 && errno == EINTR);

	if (ret == 0)
		ConvertFileStatus(&result, output);

	return ret;
}

int32_t SystemNative_FStat(intptr_t fd, FileStatus* output) 
{
	struct stat64 result;
	int ret;

	while ((ret = fstat64(fd, &result)) < 0 && errno == EINTR);

	if (ret == 0)
		ConvertFileStatus(&result, output);

	return ret;
}

int32_t SystemNative_LStat(const char* path, FileStatus* output) 
{
	struct stat64 result;
	int ret = lstat64(path, &result);

	if (ret == 0)
		ConvertFileStatus(&result, output);

	return ret;
}

int64_t SystemNative_LSeek(intptr_t fd, int64_t offset, int32_t whence) 
{
    int64_t result;
    while ((result = lseek64(fd, (off_t)offset, whence)) < 0 && errno == EINTR);

    return result;
}

int32_t SystemNative_Poll(PollEvent* pollEvents, uint32_t eventCount, int32_t milliseconds, uint32_t* triggered) 
{
	if (pollEvents == NULL || triggered == NULL)
		return Error_EFAULT;

	if (milliseconds < -1)
		return Error_EINVAL;

	struct pollfd stackBuffer[(uint32_t)(2048/sizeof(struct pollfd))];
	int useStackBuffer = eventCount <= countof(stackBuffer);

	struct pollfd* pollfds = NULL;

	if (useStackBuffer) 
	{
		pollfds = &stackBuffer[0];
	}
	else 
	{
		pollfds = (struct pollfd*)calloc(eventCount, sizeof(*pollfds));
		if (pollfds == NULL)
			return Error_ENOMEM;
	}

	for (uint32_t i = 0; i < eventCount; i++) 
	{
		const PollEvent* event = &pollEvents[i];
		pollfds[i].fd = event->FileDescriptor;
		pollfds[i].events = ConvertPollEventsPalToPlatform(event->Events);
		pollfds[i].revents = 0;
	}

	int rv;
	while ((rv = poll(pollfds, (nfds_t)eventCount, milliseconds)) < 0 && errno == EINTR);

	if (rv < 0)
	{
		if (!useStackBuffer)
			free(pollfds);

		*triggered = 0;
		return SystemNative_ConvertErrorPlatformToPal(errno);
	}

	for (uint32_t i = 0; i < eventCount; i++) 
	{
		const struct pollfd* pfd = &pollfds[i];
		assert(pfd->fd == pollEvents[i].FileDescriptor);
		assert(pfd->events == ConvertPollEventsPalToPlatform(pollEvents[i].Events));
		pollEvents[i].TriggeredEvents = ConvertPollEventsPlatformToPal(pfd->revents);
	}

	*triggered = (uint32_t)rv;

	if (!useStackBuffer)
		free(pollfds);

	return Error_SUCCESS;
}

int32_t SystemNative_Unlink(const char* path) 
{
	int32_t result;
	while ((result = unlink(path)) < 0 && errno == EINTR);

	return result;
}
