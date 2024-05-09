#pragma once
#include <stdint.h>
#include <Matrix4x4.h>
#include <Vector3.h>

struct Sphere
{
	Vector3 center;
	float radius;
};

void DrawSphere(const Sphere& _sphere, const Matrix4x4& _viewProjectionMatrix, const Matrix4x4& _viewportMatrix, uint32_t _color);