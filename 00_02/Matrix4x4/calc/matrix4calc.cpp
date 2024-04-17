#include "matrix4calc.h"

Matrix4x4 Add(const Matrix4x4& _m1, const Matrix4x4& _m2)
{
	Matrix4x4 result{};
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			result.m[i][j] = _m1.m[i][j] + _m2.m[i][j];
		}
	}
	return result;
}
