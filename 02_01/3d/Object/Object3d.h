#pragma once
#include <stdint.h>
#include <Matrix4x4.h>
#include <Vector3.h>

struct Sphere
{
	Vector3 center;
	float radius;
};

struct Line
{
	Vector3 origin;	// 始点
	Vector3 diff;	// 終点への差分ベクトル
};

struct Ray
{
	Vector3 origin;	// 始点
	Vector3 diff;	// 終点への差分ベクトル
};

struct Segment
{
	Vector3 origin;	// 始点
	Vector3 diff;	// 終点への差分ベクトル
};

void DrawSphere(const Sphere& _sphere, const Matrix4x4& _viewProjectionMatrix, const Matrix4x4& _viewportMatrix, uint32_t _color);

bool IsCollision(const Sphere& _s1, const Sphere& _s2);