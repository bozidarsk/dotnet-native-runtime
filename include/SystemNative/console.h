#ifndef SYSTEM_NATIVE_CONSOLE_H
#define SYSTEM_NATIVE_CONSOLE_H

#include <stdint.h>
#include <stddef.h>

/**
* Constants for terminal control codes.
*/
enum 
{
	PAL_VINTR = 0,
	PAL_VQUIT = 1,
	PAL_VERASE = 2,
	PAL_VKILL = 3,
	PAL_VEOF = 4,
	PAL_VTIME = 5,
	PAL_VMIN = 6,
	PAL_VSWTC = 7,
	PAL_VSTART = 8,
	PAL_VSTOP = 9,
	PAL_VSUSP = 10,
	PAL_VEOL = 11,
	PAL_VREPRINT = 12,
	PAL_VDISCARD = 13,
	PAL_VWERASE = 14,
	PAL_VLNEXT = 15,
	PAL_VEOL2 = 16
};

/*
 * Window Size of the terminal
 */
typedef struct
{
	uint16_t Row;
	uint16_t Col;
	uint16_t XPixel;
	uint16_t YPixel;
} WinSize;

int32_t SystemNative_IsATty(intptr_t fd);
void SystemNative_GetControlCharacters(int32_t* controlCharacterNames, uint8_t* controlCharacterValues, int32_t controlCharacterLength, uint8_t* posixDisableValue);
int32_t SystemNative_GetWindowSize(WinSize* windowSize);
int32_t SystemNative_SetWindowSize(WinSize* windowSize);
void SystemNative_SetKeypadXmit(const char* terminfoString);

#endif
