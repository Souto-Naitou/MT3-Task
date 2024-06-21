#pragma once
#include <Novice.h>

class NVDebug
{
public:
	NVDebug() = delete;
	~NVDebug() = delete;
	static void Begin();
	static void ScreenPrint(const char* _text);
	static void ScreenPrint(signed int _posX, const char* _text);
	static void SetIndentForREM(signed int _rem);
	static void SetIndentForPX(signed int _px);
	static void SetMarginTopForREM(signed int _rem);
	static void SetMarginTopForPX(signed int _px);

private:
	static unsigned int currentRow;
	static unsigned int offsetPositionX;
	static unsigned int offsetPositionY;
	static const unsigned int fontWidth = 15u;
	static const unsigned int fontHeight = 20u;
};