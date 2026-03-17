#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#include "SystemNative.h"

static char* g_keypadXmit = NULL; // string used to enable application mode, from terminfo

static bool CheckInterrupted(ssize_t result) 
{
	return result < 0 && errno == EINTR;
}

static void WriteKeypadXmit(void) 
{
	// If a terminfo "application mode" keypad_xmit string has been supplied,
	// write it out to the terminal to enter the mode.
	if (g_keypadXmit != NULL) 
	{
		ssize_t ret;
		while (CheckInterrupted(ret = write(STDOUT_FILENO, g_keypadXmit, (size_t)(sizeof(char) * strlen(g_keypadXmit)))));
		assert(ret >= 0); // failure to change the mode should not prevent app from continuing
	}
}

static int TranslatePalControlCharacterName(int name) 
{
	switch (name) 
	{
#ifdef VINTR
		case PAL_VINTR: return VINTR;
#endif
#ifdef VQUIT
		case PAL_VQUIT: return VQUIT;
#endif
#ifdef VERASE
		case PAL_VERASE: return VERASE;
#endif
#ifdef VKILL
		case PAL_VKILL: return VKILL;
#endif
#ifdef VEOF
		case PAL_VEOF: return VEOF;
#endif
#ifdef VTIME
		case PAL_VTIME: return VTIME;
#endif
#ifdef VMIN
		case PAL_VMIN: return VMIN;
#endif
#ifdef VSWTC
		case PAL_VSWTC: return VSWTC;
#endif
#ifdef VSTART
		case PAL_VSTART: return VSTART;
#endif
#ifdef VSTOP
		case PAL_VSTOP: return VSTOP;
#endif
#ifdef VSUSP
		case PAL_VSUSP: return VSUSP;
#endif
#ifdef VEOL
		case PAL_VEOL: return VEOL;
#endif
#ifdef VREPRINT
		case PAL_VREPRINT: return VREPRINT;
#endif
#ifdef VDISCARD
		case PAL_VDISCARD: return VDISCARD;
#endif
#ifdef VWERASE
		case PAL_VWERASE: return VWERASE;
#endif
#ifdef VLNEXT
		case PAL_VLNEXT: return VLNEXT;
#endif
#ifdef VEOL2
		case PAL_VEOL2: return VEOL2;
#endif
		default: return -1;
	}
}

int32_t SystemNative_IsATty(intptr_t fd) 
{
	return isatty(fd);
}

void SystemNative_GetControlCharacters(int32_t* controlCharacterNames, uint8_t* controlCharacterValues, int32_t controlCharacterLength, uint8_t* posixDisableValue) 
{
	assert(controlCharacterNames != NULL);
	assert(controlCharacterValues != NULL);
	assert(controlCharacterLength >= 0);
	assert(posixDisableValue != NULL);

#ifdef _POSIX_VDISABLE
	*posixDisableValue = _POSIX_VDISABLE;
#else
	*posixDisableValue = 0;
#endif

	memset(controlCharacterValues, *posixDisableValue, sizeof(uint8_t) * (size_t)controlCharacterLength);

	if (controlCharacterLength > 0) 
	{
		struct termios newTermios;
		memset(&newTermios, 0, sizeof(struct termios));

		if (tcgetattr(STDIN_FILENO, &newTermios) >= 0) 
		{
			for (int i = 0; i < controlCharacterLength; i++) 
			{
				int name = TranslatePalControlCharacterName(controlCharacterNames[i]);
				if (name >= 0)
					controlCharacterValues[i] = newTermios.c_cc[name];
			}
		}
	}
}

int32_t SystemNative_GetWindowSize(WinSize* windowSize) 
{
	assert(windowSize != NULL);

	int error = ioctl(STDOUT_FILENO, TIOCGWINSZ, windowSize);

	if (error != 0)
		memset(windowSize, 0, sizeof(WinSize)); // managed out param must be initialized

	return error;
}

int32_t SystemNative_SetWindowSize(WinSize* windowSize) 
{
	assert(windowSize != NULL);

	return ioctl(STDOUT_FILENO, TIOCSWINSZ, windowSize);
}

void SystemNative_SetKeypadXmit(const char* terminfoString) 
{
	assert(terminfoString != NULL);

	if (g_keypadXmit != NULL) // should only happen if initialization initially failed
	{
		free(g_keypadXmit);
		assert(!"g_keypadXmit was already initialized");
	}

	// Store the string to use to enter application mode, then enter
	g_keypadXmit = strdup(terminfoString);
	WriteKeypadXmit();
}
