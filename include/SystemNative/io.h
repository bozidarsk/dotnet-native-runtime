#ifndef SYSTEM_NATIVE_IO_H
#define SYSTEM_NATIVE_IO_H

#include <stdint.h>
#include <stddef.h>

/**
 * Constants for protection argument to MMap.
 */
enum 
{
	PAL_PROT_NONE = 0,  // pages may not be accessed (unless combined with one of below)
	PAL_PROT_READ = 1,  // pages may be read
	PAL_PROT_WRITE = 2, // pages may be written
	PAL_PROT_EXEC = 4,  // pages may be executed
};

/**
 * Constants for flags argument passed to MMap.
 */
enum 
{
	// shared/private are mutually exclusive
	PAL_MAP_SHARED = 0x01,  // shared mapping
	PAL_MAP_PRIVATE = 0x02, // private copy-on-write-mapping

	PAL_MAP_ANONYMOUS = 0x10, // mapping is not backed by any file
};

/**
 * Constants for interpreting the flags passed to Open or ShmOpen.
 * There are several other values defined by POSIX but not implemented
 * everywhere. The set below is restricted to the current needs of
 * COREFX, which increases portability and speeds up conversion. We
 * can add more as needed.
 */
enum 
{
	// Access modes (mutually exclusive).
	PAL_O_RDONLY = 0x0000, // Open for read-only
	PAL_O_WRONLY = 0x0001, // Open for write-only
	PAL_O_RDWR = 0x0002,   // Open for read-write

	// Mask to get just the access mode. Some room is left for more.
	// POSIX also defines O_SEARCH and O_EXEC that are not available
	// everywhere.
	PAL_O_ACCESS_MODE_MASK = 0x000F,

	// Flags (combinable)
	// These numeric values are not defined by POSIX and vary across targets.
	PAL_O_CLOEXEC = 0x0010,  // Close-on-exec
	PAL_O_CREAT = 0x0020,    // Create file if it doesn't already exist
	PAL_O_EXCL = 0x0040,     // When combined with CREAT, fails if file already exists
	PAL_O_TRUNC = 0x0080,    // Truncate file to length 0 if it already exists
	PAL_O_SYNC = 0x0100,     // Block writes call will block until physically written
	PAL_O_NOFOLLOW = 0x0200, // Fails to open the target if it's a symlink, parent symlinks are allowed
};

/**
 * Constants for interpreting FileStatus.Flags.
 */
enum
{
    FILESTATUS_FLAGS_NONE = 0,
    FILESTATUS_FLAGS_HAS_BIRTHTIME = 1,
};

/**
 * File status returned by Stat or FStat.
 */
typedef struct 
{
    int32_t Flags;     // flags for testing if some members are present (see FileStatusFlags)
    int32_t Mode;      // file mode (see S_I* constants above for bit values)
    uint32_t Uid;      // user ID of owner
    uint32_t Gid;      // group ID of owner
    int64_t Size;      // total size, in bytes
    int64_t ATime;     // time of last access
    int64_t ATimeNsec; //     nanosecond part
    int64_t MTime;     // time of last modification
    int64_t MTimeNsec; //     nanosecond part
    int64_t CTime;     // time of last status change
    int64_t CTimeNsec; //     nanosecond part
    int64_t BirthTime; // time the file was created
    int64_t BirthTimeNsec; // nanosecond part
    int64_t Dev;       // ID of the device containing the file
    int64_t RDev;      // ID of the device if it is a special file
    int64_t Ino;       // inode number of the file
    uint32_t UserFlags; // user defined flags
} FileStatus;

/**
 * Our intermediate pollfd struct to normalize the data types
 */
typedef struct 
{
	int32_t FileDescriptor;  // The file descriptor to poll
	int16_t Events;          // The events to poll for
	int16_t TriggeredEvents; // The events that triggered the poll
} PollEvent;

/**
 * Constants passed to and from poll describing what to poll for and what
 * kind of data was received from poll.
 */
typedef enum 
{
	PAL_POLLIN = 0x0001,   /* non-urgent readable data available */
	PAL_POLLPRI = 0x0002,  /* urgent readable data available */
	PAL_POLLOUT = 0x0004,  /* data can be written without blocked */
	PAL_POLLERR = 0x0008,  /* an error occurred */
	PAL_POLLHUP = 0x0010,  /* the file descriptor hung up */
	PAL_POLLNVAL = 0x0020, /* the requested events were invalid */
} PollEvents;

int32_t SystemNative_Close(intptr_t fd);
int32_t SystemNative_Write(intptr_t fd, const void* buffer, int32_t bufferSize);
int32_t SystemNative_Read(intptr_t fd, void* buffer, int32_t bufferSize);
intptr_t SystemNative_Open(const char* path, int32_t flags, int32_t mode);
int32_t SystemNative_PRead(intptr_t fd, void* buffer, int32_t bufferSize, int64_t fileOffset);
int32_t SystemNative_PWrite(intptr_t fd, void* buffer, int32_t bufferSize, int64_t fileOffset);
intptr_t SystemNative_Dup(intptr_t oldfd);
int32_t SystemNative_FLock(intptr_t fd, int32_t operation);
int32_t SystemNative_Stat(const char* path, FileStatus* output);
int32_t SystemNative_FStat(intptr_t fd, FileStatus* output);
int32_t SystemNative_LStat(const char* path, FileStatus* output);
int64_t SystemNative_LSeek(intptr_t fd, int64_t offset, int32_t whence);
int32_t SystemNative_Poll(PollEvent* pollEvents, uint32_t eventCount, int32_t milliseconds, uint32_t* triggered);
int32_t SystemNative_Unlink(const char* path);

#endif
