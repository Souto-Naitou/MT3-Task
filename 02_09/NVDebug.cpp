#include "NVDebug.h"
#include <stdarg.h>

unsigned int NVDebug::currentRow = 0;
unsigned int NVDebug::offsetPositionX = NVDebug::fontWidth * 2;
unsigned int NVDebug::offsetPositionY = NVDebug::fontHeight * 2;

void NVDebug::Begin()
{
	currentRow = 0;
}

void NVDebug::ScreenPrint(const char* _text)
{
	Novice::ScreenPrintf(offsetPositionX, offsetPositionY + 20 * currentRow++, _text);
	return;
}

void NVDebug::ScreenPrint(signed int _posX, const char* _text)
{
	Novice::ScreenPrintf(_posX, offsetPositionY + 20 * currentRow++, _text);
	return;
}

void NVDebug::SetIndentForPX(signed int _px)
{
	offsetPositionX = _px;
	return;
}

void NVDebug::SetIndentForREM(signed int _rem)
{
	offsetPositionX = fontWidth * _rem;
	return;
}

void NVDebug::SetMarginTopForPX(signed int _px)
{
	offsetPositionY = _px;
	return;
}

void NVDebug::SetMarginTopForREM(signed int _rem)
{
	offsetPositionY = fontHeight * _rem;
	return;
}
