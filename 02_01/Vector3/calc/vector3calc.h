﻿#pragma once
#include <Matrix4x4.h>
#include "3d/Object/Object3d.h"
#include <Vector3.h>

// 加算
Vector3 Add(const Vector3& _v1, const Vector3& _v2);

// 減算
Vector3 Subtract(const Vector3& _v1, const Vector3& _v2);

// スカラー倍
Vector3 Multiply(float _scalar, const Vector3& _v);

// 内積
float Dot(const Vector3& _v1, const Vector3& _v2);

// 長さ（ノルム）
float Length(const Vector3& _v);

// 正規化
Vector3 Normalize(const Vector3& _v);

Vector3 Transform(const Vector3& _vector, const Matrix4x4& _matrix);

// クロス積
Vector3 Cross(const Vector3& _v1, const Vector3& _v2);

// ベクトル射影
Vector3 Project(const Vector3& _v1, const Vector3& _v2);

// 最接近点
Vector3 ClosestPoint(const Vector3& _point, const Segment& _segment);