#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <sched.h>
#include <unistd.h>
#include <pwd.h>
#include <time.h>
#include <netdb.h>
#include <sys/auxv.h>

#include "SystemNative.h"

enum 
{
	MicroSecondsToNanoSeconds = 1000,   // 10^3
	SecondsToNanoSeconds = 1000000000,  // 10^9
	SecondsToTicks = 10000000,          // 10^7
	TicksToNanoSeconds = 100,           // 10^2
};

static int32_t ConvertNativePasswdToPalPasswd(int error, struct passwd* nativePwd, struct passwd* result, Passwd* pwd) 
{
	// positive error number returned -> failure other than entry-not-found
	if (error != 0) 
	{
		assert(error > 0);
		memset(pwd, 0, sizeof(Passwd)); // managed out param must be initialized
		return error;
	}

	// 0 returned with null result -> entry-not-found
	if (result == NULL) 
	{
		memset(pwd, 0, sizeof(Passwd)); // managed out param must be initialized
		return -1; // shim convention for entry-not-found
	}

	// 0 returned with non-null result (guaranteed to be set to pwd arg) -> success
	assert(result == nativePwd);
	pwd->Name = nativePwd->pw_name;
	pwd->Password = nativePwd->pw_passwd;
	pwd->UserId = nativePwd->pw_uid;
	pwd->GroupId = nativePwd->pw_gid;
	pwd->UserInfo = nativePwd->pw_gecos;
	pwd->HomeDirectory = nativePwd->pw_dir;
	pwd->Shell = nativePwd->pw_shell;

	return 0;
}

static bool TryConvertErrorToGai(int32_t error, int32_t* gaiError) 
{
	assert(gaiError != NULL);

	switch (error) 
	{
		case EHOSTNOTFOUND:
			*gaiError = EAI_NONAME;
			return true;
		default:
			*gaiError = error;
			return false;
	}
}

int32_t SystemNative_ConvertErrorPlatformToPal(int32_t platformErrno) 
{
	switch (platformErrno) 
	{
		case 0:
			return Error_SUCCESS;
		case E2BIG:
			return Error_E2BIG;
		case EACCES:
			return Error_EACCES;
		case EADDRINUSE:
			return Error_EADDRINUSE;
		case EADDRNOTAVAIL:
			return Error_EADDRNOTAVAIL;
		case EAFNOSUPPORT:
			return Error_EAFNOSUPPORT;
		case EAGAIN:
			return Error_EAGAIN;
		case EALREADY:
			return Error_EALREADY;
		case EBADF:
			return Error_EBADF;
		case EBADMSG:
			return Error_EBADMSG;
		case EBUSY:
			return Error_EBUSY;
		case ECANCELED:
			return Error_ECANCELED;
		case ECHILD:
			return Error_ECHILD;
		case ECONNABORTED:
			return Error_ECONNABORTED;
		case ECONNREFUSED:
			return Error_ECONNREFUSED;
		case ECONNRESET:
			return Error_ECONNRESET;
		case EDEADLK:
			return Error_EDEADLK;
		case EDESTADDRREQ:
			return Error_EDESTADDRREQ;
		case EDOM:
			return Error_EDOM;
		case EDQUOT:
			return Error_EDQUOT;
		case EEXIST:
			return Error_EEXIST;
		case EFAULT:
			return Error_EFAULT;
		case EFBIG:
			return Error_EFBIG;
		case EHOSTUNREACH:
			return Error_EHOSTUNREACH;
		case EIDRM:
			return Error_EIDRM;
		case EILSEQ:
			return Error_EILSEQ;
		case EINPROGRESS:
			return Error_EINPROGRESS;
		case EINTR:
			return Error_EINTR;
		case EINVAL:
			return Error_EINVAL;
		case EIO:
			return Error_EIO;
		case EISCONN:
			return Error_EISCONN;
		case EISDIR:
			return Error_EISDIR;
		case ELOOP:
			return Error_ELOOP;
		case EMFILE:
			return Error_EMFILE;
		case EMLINK:
			return Error_EMLINK;
		case EMSGSIZE:
			return Error_EMSGSIZE;
		case EMULTIHOP:
			return Error_EMULTIHOP;
		case ENAMETOOLONG:
			return Error_ENAMETOOLONG;
		case ENETDOWN:
			return Error_ENETDOWN;
		case ENETRESET:
			return Error_ENETRESET;
		case ENETUNREACH:
			return Error_ENETUNREACH;
		case ENFILE:
			return Error_ENFILE;
		case ENOBUFS:
			return Error_ENOBUFS;
		case ENODEV:
			return Error_ENODEV;
		case ENOENT:
			return Error_ENOENT;
		case ENOEXEC:
			return Error_ENOEXEC;
		case ENOLCK:
			return Error_ENOLCK;
		case ENOLINK:
			return Error_ENOLINK;
		case ENOMEM:
			return Error_ENOMEM;
		case ENOMSG:
			return Error_ENOMSG;
		case ENOPROTOOPT:
			return Error_ENOPROTOOPT;
		case ENOSPC:
			return Error_ENOSPC;
		case ENOSYS:
			return Error_ENOSYS;
		case ENOTCONN:
			return Error_ENOTCONN;
		case ENOTDIR:
			return Error_ENOTDIR;
#if ENOTEMPTY != EEXIST // AIX defines this
		case ENOTEMPTY:
			return Error_ENOTEMPTY;
#endif
#ifdef ENOTRECOVERABLE // not available in NetBSD
		case ENOTRECOVERABLE:
			return Error_ENOTRECOVERABLE;
#endif
		case ENOTSOCK:
			return Error_ENOTSOCK;
		case ENOTSUP:
			return Error_ENOTSUP;
		case ENOTTY:
			return Error_ENOTTY;
		case ENXIO:
			return Error_ENXIO;
		case EOVERFLOW:
			return Error_EOVERFLOW;
#ifdef EOWNERDEAD // not available in NetBSD
		case EOWNERDEAD:
			return Error_EOWNERDEAD;
#endif
		case EPERM:
			return Error_EPERM;
		case EPIPE:
			return Error_EPIPE;
		case EPROTO:
			return Error_EPROTO;
		case EPROTONOSUPPORT:
			return Error_EPROTONOSUPPORT;
		case EPROTOTYPE:
			return Error_EPROTOTYPE;
		case ERANGE:
			return Error_ERANGE;
		case EROFS:
			return Error_EROFS;
		case ESPIPE:
			return Error_ESPIPE;
		case ESRCH:
			return Error_ESRCH;
		case ESTALE:
			return Error_ESTALE;
		case ETIMEDOUT:
			return Error_ETIMEDOUT;
		case ETXTBSY:
			return Error_ETXTBSY;
		case EXDEV:
			return Error_EXDEV;
#ifdef ESOCKTNOSUPPORT
		case ESOCKTNOSUPPORT:
			return Error_ESOCKTNOSUPPORT;
#endif
#ifdef EPFNOSUPPORT // not available in WASI
		case EPFNOSUPPORT:
			return Error_EPFNOSUPPORT;
#endif
#ifdef ESHUTDOWN // not available in WASI
		case ESHUTDOWN:
			return Error_ESHUTDOWN;
#endif
#ifdef EHOSTDOWN // not available in WASI
		case EHOSTDOWN:
			return Error_EHOSTDOWN;
#endif
#ifdef ENODATA // not available in FreeBSD
		case ENODATA:
			return Error_ENODATA;
#endif

// #if because these will trigger duplicate case label warnings when
// they have the same value, which is permitted by POSIX and common.
#if EOPNOTSUPP != ENOTSUP
		case EOPNOTSUPP:
			return Error_EOPNOTSUPP;
#endif
#if EWOULDBLOCK != EAGAIN
		case EWOULDBLOCK:
			return Error_EWOULDBLOCK;
#endif
	}

	return Error_ENONSTANDARD;
}

int32_t SystemNative_ConvertErrorPalToPlatform(int32_t error) 
{
	switch (error) 
	{
		case Error_SUCCESS:
			return 0;
		case Error_E2BIG:
			return E2BIG;
		case Error_EACCES:
			return EACCES;
		case Error_EADDRINUSE:
			return EADDRINUSE;
		case Error_EADDRNOTAVAIL:
			return EADDRNOTAVAIL;
		case Error_EAFNOSUPPORT:
			return EAFNOSUPPORT;
		case Error_EAGAIN:
			return EAGAIN;
		case Error_EALREADY:
			return EALREADY;
		case Error_EBADF:
			return EBADF;
		case Error_EBADMSG:
			return EBADMSG;
		case Error_EBUSY:
			return EBUSY;
		case Error_ECANCELED:
			return ECANCELED;
		case Error_ECHILD:
			return ECHILD;
		case Error_ECONNABORTED:
			return ECONNABORTED;
		case Error_ECONNREFUSED:
			return ECONNREFUSED;
		case Error_ECONNRESET:
			return ECONNRESET;
		case Error_EDEADLK:
			return EDEADLK;
		case Error_EDESTADDRREQ:
			return EDESTADDRREQ;
		case Error_EDOM:
			return EDOM;
		case Error_EDQUOT:
			return EDQUOT;
		case Error_EEXIST:
			return EEXIST;
		case Error_EFAULT:
			return EFAULT;
		case Error_EFBIG:
			return EFBIG;
		case Error_EHOSTUNREACH:
			return EHOSTUNREACH;
		case Error_EIDRM:
			return EIDRM;
		case Error_EILSEQ:
			return EILSEQ;
		case Error_EINPROGRESS:
			return EINPROGRESS;
		case Error_EINTR:
			return EINTR;
		case Error_EINVAL:
			return EINVAL;
		case Error_EIO:
			return EIO;
		case Error_EISCONN:
			return EISCONN;
		case Error_EISDIR:
			return EISDIR;
		case Error_ELOOP:
			return ELOOP;
		case Error_EMFILE:
			return EMFILE;
		case Error_EMLINK:
			return EMLINK;
		case Error_EMSGSIZE:
			return EMSGSIZE;
		case Error_EMULTIHOP:
			return EMULTIHOP;
		case Error_ENAMETOOLONG:
			return ENAMETOOLONG;
		case Error_ENETDOWN:
			return ENETDOWN;
		case Error_ENETRESET:
			return ENETRESET;
		case Error_ENETUNREACH:
			return ENETUNREACH;
		case Error_ENFILE:
			return ENFILE;
		case Error_ENOBUFS:
			return ENOBUFS;
		case Error_ENODEV:
			return ENODEV;
		case Error_ENOENT:
			return ENOENT;
		case Error_ENOEXEC:
			return ENOEXEC;
		case Error_ENOLCK:
			return ENOLCK;
		case Error_ENOLINK:
			return ENOLINK;
		case Error_ENOMEM:
			return ENOMEM;
		case Error_ENOMSG:
			return ENOMSG;
		case Error_ENOPROTOOPT:
			return ENOPROTOOPT;
		case Error_ENOSPC:
			return ENOSPC;
		case Error_ENOSYS:
			return ENOSYS;
		case Error_ENOTCONN:
			return ENOTCONN;
		case Error_ENOTDIR:
			return ENOTDIR;
		case Error_ENOTEMPTY:
			return ENOTEMPTY;
#ifdef ENOTRECOVERABLE // not available in NetBSD
		case Error_ENOTRECOVERABLE:
			return ENOTRECOVERABLE;
#endif
		case Error_ENOTSOCK:
			return ENOTSOCK;
		case Error_ENOTSUP:
			return ENOTSUP;
		case Error_ENOTTY:
			return ENOTTY;
		case Error_ENXIO:
			return ENXIO;
		case Error_EOVERFLOW:
			return EOVERFLOW;
#ifdef EOWNERDEAD // not available in NetBSD
		case Error_EOWNERDEAD:
			return EOWNERDEAD;
#endif
		case Error_EPERM:
			return EPERM;
		case Error_EPIPE:
			return EPIPE;
		case Error_EPROTO:
			return EPROTO;
		case Error_EPROTONOSUPPORT:
			return EPROTONOSUPPORT;
		case Error_EPROTOTYPE:
			return EPROTOTYPE;
		case Error_ERANGE:
			return ERANGE;
		case Error_EROFS:
			return EROFS;
		case Error_ESPIPE:
			return ESPIPE;
		case Error_ESRCH:
			return ESRCH;
		case Error_ESTALE:
			return ESTALE;
		case Error_ETIMEDOUT:
			return ETIMEDOUT;
		case Error_ETXTBSY:
			return ETXTBSY;
		case Error_EXDEV:
			return EXDEV;
#ifdef EPFNOSUPPORT // not available in WASI
		case Error_EPFNOSUPPORT:
			return EPFNOSUPPORT;
#endif
#ifdef ESOCKTNOSUPPORT
		case Error_ESOCKTNOSUPPORT:
			return ESOCKTNOSUPPORT;
#endif
#ifdef ESHUTDOWN // not available in WASI
		case Error_ESHUTDOWN:
			return ESHUTDOWN;
#endif
#ifdef EHOSTDOWN // not available in WASI
		case Error_EHOSTDOWN:
			return EHOSTDOWN;
#endif
#ifdef ENODATA // not available in FreeBSD
		case Error_ENODATA:
			return ENODATA;
#endif
		case Error_EHOSTNOTFOUND:
			return EHOSTNOTFOUND;
		case Error_ESOCKETERROR:
			return ESOCKETERROR;

		case Error_ENONSTANDARD:
			break; // fall through to assert
	}

	// We should not use this function to round-trip platform -> pal
	// -> platform. It's here only to synthesize a platform number
	// from the fixed set above. Note that the assert is outside the
	// switch rather than in a default case block because not
	// having a default will trigger a warning (as error) if there's
	// an enum value we haven't handled. Should that trigger, make
	// note that there is probably a corresponding missing case in the
	// other direction above, but the compiler can't warn in that case
	// because the platform values are not part of an enum.
	assert(!"Unknown error code.");
	return -1;
}

void SystemNative_SetErrNo(int32_t errorCode) 
{
	errno = errorCode;
}

int32_t SystemNative_GetErrNo(void) 
{
	return errno;
}

const char* SystemNative_StrErrorR(int32_t platformErrno, char* buffer, int32_t bufferSize) 
{
	assert(buffer != NULL);
	assert(bufferSize > 0);

	if (bufferSize < 0)
		return NULL;

	if (platformErrno < 0) 
	{
		// Not a system error
		int32_t gaiError;
		if (TryConvertErrorToGai(platformErrno, &gaiError)) 
		{
			strncpy(buffer, gai_strerror(gaiError), (size_t)bufferSize);
			return buffer;
		}
		else if (platformErrno == ESOCKETERROR) 
		{
			strncpy(buffer, "Unknown socket error.", (size_t)bufferSize);
			return buffer;
		}
	}

// Note that we must use strerror_r because plain strerror is not
// thread-safe.
//
// However, there are two versions of strerror_r:
//	- GNU:   char* strerror_r(int, char*, size_t);
//	- POSIX: int   strerror_r(int, char*, size_t);
//
// The former may or may not use the supplied buffer, and returns
// the error message string. The latter stores the error message
// string into the supplied buffer and returns an error code.

#if (_POSIX_C_SOURCE < 200112L) || _GNU_SOURCE
	const char* message = strerror_r(platformErrno, buffer, (uint32_t)bufferSize);
	assert(message != NULL);
	return message;
#else
	int error = strerror_r(platformErrno, buffer, (uint32_t) bufferSize);
	if (error == ERANGE) 
	{
		// Buffer is too small to hold the entire message, but has
		// still been filled to the extent possible and null-terminated.
		return NULL;
	}

	// The only other valid error codes are 0 for success or EINVAL for
	// an unknown error, but in the latter case a reasonable string (e.g
	// "Unknown error: 0x123") is returned.
	assert(error == 0 || error == EINVAL); // invalid error
	return buffer;
#endif
}

char* SystemNative_GetEnv(const char* variable) 
{
	return getenv(variable);
}

char** SystemNative_GetEnviron(void) 
{
	static char** environ;
	return environ;
}

void SystemNative_FreeEnviron(char** environ) 
{
	// no op
	(void)environ;
}

int32_t SystemNative_SchedGetCpu(void) 
{
	return sched_getcpu();
}

__attribute__((noreturn))
void SystemNative_Exit(int32_t exitCode) 
{
	exit(exitCode);
}

__attribute__((noreturn))
void SystemNative_Abort(void) 
{
	abort();
}

char* SystemNative_GetCwd(char* buffer, int32_t bufferSize) 
{
	assert(bufferSize >= 0);

	if (bufferSize < 0) 
	{
		errno = EINVAL;
		return NULL;
	}

	return getcwd(buffer, (size_t)bufferSize);
}

char* SystemNative_GetProcessPath(void) 
{
	const char* symlinkEntrypointExecutable = "/proc/self/exe";

	// Resolve the symlink to the executable from /proc
	char* path = realpath(symlinkEntrypointExecutable, NULL);
	if (path)
		return path;

	// fallback to AT_EXECFN, which does not work properly in rare cases
	// when .NET process is set as interpreter (shebang).
	const char* exePath = (const char *)(getauxval(AT_EXECFN));
	if (exePath)
		return realpath(exePath, NULL);

	return NULL;
}

int32_t SystemNative_SetEUid(uint32_t euid) 
{
	return seteuid(euid);
}

uint32_t SystemNative_GetEUid(void) 
{
	return geteuid();
}

int32_t SystemNative_SetEGid(uint32_t egid) 
{
	return setegid(egid);
}

uint32_t SystemNative_GetEGid(void) 
{
	return getegid();
}

int32_t SystemNative_GetPwUidR(uint32_t uid, Passwd* pwd, char* buf, int32_t buflen) 
{
	assert(pwd != NULL);
	assert(buf != NULL);
	assert(buflen >= 0);

	if (buflen < 0)
		return EINVAL;

	struct passwd nativePwd;
	struct passwd* result;

	int error;
	while ((error = getpwuid_r(uid, &nativePwd, buf, (size_t)buflen, &result)) == EINTR);

	return ConvertNativePasswdToPalPasswd(error, &nativePwd, result, pwd);
}

int32_t SystemNative_GetPwNamR(const char* name, Passwd* pwd, char* buf, int32_t buflen) 
{
	assert(pwd != NULL);
	assert(buf != NULL);
	assert(buflen >= 0);

	if (buflen < 0)
		return EINVAL;

	struct passwd nativePwd;
	struct passwd* result;

	int error;
	while ((error = getpwnam_r(name, &nativePwd, buf, (size_t)buflen, &result)) == EINTR);

	return ConvertNativePasswdToPalPasswd(error, &nativePwd, result, pwd);
}

uint64_t SystemNative_GetTimestamp(void) 
{
	struct timespec ts;

	int result = clock_gettime(CLOCK_MONOTONIC, &ts);
	assert(result == 0); // only possible errors are if MONOTONIC isn't supported or &ts is an invalid address
	(void)result; // suppress unused parameter warning in release builds

	return ((uint64_t)(ts.tv_sec) * SecondsToNanoSeconds) + (uint64_t)(ts.tv_nsec);
}

void SystemNative_GetNonCryptographicallySecureRandomBytes(uint8_t* buffer, int32_t bufferLength) 
{
	assert(buffer != NULL);

	arc4random_buf(buffer, (size_t)bufferLength);
}
