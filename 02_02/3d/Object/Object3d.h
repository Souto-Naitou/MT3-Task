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

struct Plane
{
	Vector3 normal; // 法線
	float distance; // 距離
};

Vector3 Perpendicular(const Vector3& _vector);


void DrawSphere(const Sphere& _sphere, const Matrix4x4& _viewProjectionMatrix, const Matrix4x4& _viewportMatrix, uint32_t _color);
void DrawPlane(const Plane& _plane, const Matrix4x4& _viewProjectionMatrix, const Matrix4x4& _viewportMatrix, uint32_t _color);

bool IsCollision(const Sphere& _s1, const Sphere& _s2);
bool IsCollision(const Sphere& _sphere, const Plane& _plane);