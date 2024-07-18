#include "matrix4out.h"

static const int kRowHeight = 20;
static const int kColumnWidth = 60;

void MatrixScreenPrint(int _x, int _y, const Matrix4x4& _matrix, const char* _label)
{
	Novice::ScreenPrintf(_x, _y, _label);
	for (int row = 1; row < 5; ++row)
	{
		for (int column = 0; column < 4; ++column)
		{
			Novice::ScreenPrintf(
				_x + column * kColumnWidth, _y + row * kRowHeight, "%6.02f", _matrix.m[row-1][column]);
		}
	}
}

int GetColumnWidth() { return kColumnWidth; };
int GetRowHeight() { return kRowHeight; };