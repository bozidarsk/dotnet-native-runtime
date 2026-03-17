#ifndef SYSTEM_NATIVE_ENV_H
#define SYSTEM_NATIVE_ENV_H

#include <stdint.h>
#include <stddef.h>

/**
* Passwd struct
*/
typedef struct
{
    char* Name;
    char* Password;
    uint32_t UserId;
    uint32_t GroupId;
    char* UserInfo;
    char* HomeDirectory;
    char* Shell;
} Passwd;

/**
 * Error codes returned via ConvertErrno.
 *
 * Only the names (without the PAL_ prefix) are specified by POSIX.
 *
 * The values chosen below are simply assigned arbitrarily (originally
 * in alphabetical order they appear in the spec, but they can't change so
 * add new values to the end!).
 *
 * Also, the values chosen are deliberately outside the range of
 * typical UNIX errnos (small numbers), HRESULTs (negative for errors)
 * and Win32 errors (0x0000 - 0xFFFF). This isn't required for
 * correctness, but may help debug a caller that is interpreting a raw
 * int incorrectly.
 *
 * Wherever the spec says "x may be the same value as y", we do use
 * the same value so that callers cannot not take a dependency on
 * being able to distinguish between them.
 */
typedef enum 
{
	Error_SUCCESS = 0,

	Error_E2BIG = 0x10001,           // Argument list too long.
	Error_EACCES = 0x10002,          // Permission denied.
	Error_EADDRINUSE = 0x10003,      // Address in use.
	Error_EADDRNOTAVAIL = 0x10004,   // Address not available.
	Error_EAFNOSUPPORT = 0x10005,    // Address family not supported.
	Error_EAGAIN = 0x10006,          // Resource unavailable, try again (same value as EWOULDBLOCK),
	Error_EALREADY = 0x10007,        // Connection already in progress.
	Error_EBADF = 0x10008,           // Bad file descriptor.
	Error_EBADMSG = 0x10009,         // Bad message.
	Error_EBUSY = 0x1000A,           // Device or resource busy.
	Error_ECANCELED = 0x1000B,       // Operation canceled.
	Error_ECHILD = 0x1000C,          // No child processes.
	Error_ECONNABORTED = 0x1000D,    // Connection aborted.
	Error_ECONNREFUSED = 0x1000E,    // Connection refused.
	Error_ECONNRESET = 0x1000F,      // Connection reset.
	Error_EDEADLK = 0x10010,         // Resource deadlock would occur.
	Error_EDESTADDRREQ = 0x10011,    // Destination address required.
	Error_EDOM = 0x10012,            // Mathematics argument out of domain of function.
	Error_EDQUOT = 0x10013,          // Reserved.
	Error_EEXIST = 0x10014,          // File exists.
	Error_EFAULT = 0x10015,          // Bad address.
	Error_EFBIG = 0x10016,           // File too large.
	Error_EHOSTUNREACH = 0x10017,    // Host is unreachable.
	Error_EIDRM = 0x10018,           // Identifier removed.
	Error_EILSEQ = 0x10019,          // Illegal byte sequence.
	Error_EINPROGRESS = 0x1001A,     // Operation in progress.
	Error_EINTR = 0x1001B,           // Interrupted function.
	Error_EINVAL = 0x1001C,          // Invalid argument.
	Error_EIO = 0x1001D,             // I/O error.
	Error_EISCONN = 0x1001E,         // Socket is connected.
	Error_EISDIR = 0x1001F,          // Is a directory.
	Error_ELOOP = 0x10020,           // Too many levels of symbolic links.
	Error_EMFILE = 0x10021,          // File descriptor value too large.
	Error_EMLINK = 0x10022,          // Too many links.
	Error_EMSGSIZE = 0x10023,        // Message too large.
	Error_EMULTIHOP = 0x10024,       // Reserved.
	Error_ENAMETOOLONG = 0x10025,    // Filename too long.
	Error_ENETDOWN = 0x10026,        // Network is down.
	Error_ENETRESET = 0x10027,       // Connection aborted by network.
	Error_ENETUNREACH = 0x10028,     // Network unreachable.
	Error_ENFILE = 0x10029,          // Too many files open in system.
	Error_ENOBUFS = 0x1002A,         // No buffer space available.
	Error_ENODEV = 0x1002C,          // No such device.
	Error_ENOENT = 0x1002D,          // No such file or directory.
	Error_ENOEXEC = 0x1002E,         // Executable file format error.
	Error_ENOLCK = 0x1002F,          // No locks available.
	Error_ENOLINK = 0x10030,         // Reserved.
	Error_ENOMEM = 0x10031,          // Not enough space.
	Error_ENOMSG = 0x10032,          // No message of the desired type.
	Error_ENOPROTOOPT = 0x10033,     // Protocol not available.
	Error_ENOSPC = 0x10034,          // No space left on device.
	Error_ENOSYS = 0x10037,          // Function not supported.
	Error_ENOTCONN = 0x10038,        // The socket is not connected.
	Error_ENOTDIR = 0x10039,         // Not a directory or a symbolic link to a directory.
	Error_ENOTEMPTY = 0x1003A,       // Directory not empty.
	Error_ENOTRECOVERABLE = 0x1003B, // State not recoverable.
	Error_ENOTSOCK = 0x1003C,        // Not a socket.
	Error_ENOTSUP = 0x1003D,         // Not supported (same value as EOPNOTSUP).
	Error_ENOTTY = 0x1003E,          // Inappropriate I/O control operation.
	Error_ENXIO = 0x1003F,           // No such device or address.
	Error_EOVERFLOW = 0x10040,       // Value too large to be stored in data type.
	Error_EOWNERDEAD = 0x10041,      // Previous owner died.
	Error_EPERM = 0x10042,           // Operation not permitted.
	Error_EPIPE = 0x10043,           // Broken pipe.
	Error_EPROTO = 0x10044,          // Protocol error.
	Error_EPROTONOSUPPORT = 0x10045, // Protocol not supported.
	Error_EPROTOTYPE = 0x10046,      // Protocol wrong type for socket.
	Error_ERANGE = 0x10047,          // Result too large.
	Error_EROFS = 0x10048,           // Read-only file system.
	Error_ESPIPE = 0x10049,          // Invalid seek.
	Error_ESRCH = 0x1004A,           // No such process.
	Error_ESTALE = 0x1004B,          // Reserved.
	Error_ETIMEDOUT = 0x1004D,       // Connection timed out.
	Error_ETXTBSY = 0x1004E,         // Text file busy.
	Error_EXDEV = 0x1004F,           // Cross-device link.
	Error_ESOCKTNOSUPPORT = 0x1005E, // Socket type not supported.
	Error_EPFNOSUPPORT = 0x10060,    // Protocol family not supported.
	Error_ESHUTDOWN = 0x1006C,       // Socket shutdown.
	Error_EHOSTDOWN = 0x10070,       // Host is down.
	Error_ENODATA = 0x10071,         // No data available.

	// Error codes to track errors beyond kernel.
	Error_EHOSTNOTFOUND = 0x20001,   // Name lookup failed.
	Error_ESOCKETERROR = 0x20002,    // Unidentified socket error.

	// POSIX permits these to have the same value and we make them
	// always equal so that we cannot introduce a dependency on
	// distinguishing between them that would not work on all
	// platforms.
	Error_EOPNOTSUPP = Error_ENOTSUP, // Operation not supported on socket
	Error_EWOULDBLOCK = Error_EAGAIN, // Operation would block

	// This one is not part of POSIX, but is a catch-all for the case
	// where we cannot convert the raw errno value to something above.
	Error_ENONSTANDARD = 0x1FFFF,
} Error;

/*
  Some pal errors don't have a corresponding errno value.
  We define values for these errors.
  We want these values to be distinct from real errno values.
  We base of the Error enum values which are chosen to be out of the
  typical errno range, and make them negative because POSIX
  requires errno values to be positive.
*/
#define EHOSTNOTFOUND (-Error_EHOSTNOTFOUND)
#define ESOCKETERROR  (-Error_ESOCKETERROR)

int32_t SystemNative_ConvertErrorPlatformToPal(int32_t platformErrno);
int32_t SystemNative_ConvertErrorPalToPlatform(int32_t error);
void SystemNative_SetErrNo(int32_t errorCode);
int32_t SystemNative_GetErrNo(void);
const char* SystemNative_StrErrorR(int32_t platformErrno, char* buffer, int32_t bufferSize);
char* SystemNative_GetEnv(const char* variable);
char** SystemNative_GetEnviron(void);
void SystemNative_FreeEnviron(char** environ);
int32_t SystemNative_SchedGetCpu(void);
__attribute__((noreturn)) void SystemNative_Exit(int32_t exitCode);
__attribute__((noreturn)) void SystemNative_Abort(void);
char* SystemNative_GetCwd(char* buffer, int32_t bufferSize);
char* SystemNative_GetProcessPath(void);
int32_t SystemNative_SetEUid(uint32_t euid);
uint32_t SystemNative_GetEUid(void);
int32_t SystemNative_SetEGid(uint32_t egid);
uint32_t SystemNative_GetEGid(void);
int32_t SystemNative_GetPwUidR(uint32_t uid, Passwd* pwd, char* buf, int32_t buflen);
int32_t SystemNative_GetPwNamR(const char* name, Passwd* pwd, char* buf, int32_t buflen);
uint64_t SystemNative_GetTimestamp(void);
void SystemNative_GetNonCryptographicallySecureRandomBytes(uint8_t* buffer, int32_t bufferLength);

#endif
