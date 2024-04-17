#pragma once
#include "Matrix/4/matrix4x4.h"

Matrix4x4 Add(const Matrix4x4& _m1, const Matrix4x4& _m2);

Matrix4x4 Subtract(const Matrix4x4& _m1, const Matrix4x4& _m2);

Matrix4x4 Multiply(const Matrix4x4& _m1, const Matrix4x4& _m2);

Matrix4x4 Inverse(const Matrix4x4& _m);

Matrix4x4 Transpose(const Matrix4x4& _m);

// ’PˆÊs—ñ‚Ìì¬
Matrix4x4 MakeIdentity4x4();