#include "matrix4out.h"

void MatrixScreenPrint(int _x, int _y, const Matrix4x4& _matrix)
{
	for (int row = 0; row < 4; ++row)
	{
		for (int column = 0; column < 4; ++column)
		{
			Novice::ScreenPrintf(
				_x + column * kColumnWidth, _y + row * kRowHeight, "%6.02f", _matrix.m[row][column]);
		}
	}
}
