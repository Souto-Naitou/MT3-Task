#pragma once
#include <stdint.h>
#include <Matrix4x4.h>
#include <Vector3.h>

struct Sphere
{
	Vector3 center;
	float radius;
};

/// <summary>
/// 直線
/// </summary>
struct Line
{
	Vector3 origin;	// 始点
	Vector3 diff;	// 終点への差分ベクトル
};

/// <summary>
/// 半直線
/// </summary>
struct Ray
{
	Vector3 origin;	// 始点
	Vector3 diff;	// 終点への差分ベクトル
};

/// <summary>
/// 線分
/// </summary>
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

struct Triangle
{
	Vector3 vertices[3]; // 頂点
};

struct AABB
{
	Vector3 min; // 最小点
	Vector3 max; // 最大点
};

Vector3 Perpendicular(const Vector3& _vector);

Plane MakePlane(const Triangle& _triangle);
void Clamp(AABB& _aabb);

void ScreenPrint(const Vector3& _pos, const Matrix4x4& _viewProjectionMatrix, const Matrix4x4& _viewportMatrix, const char* fmt);
void DrawLine(const Vector3& _origin, const Vector3& _diff, const Matrix4x4& _viewProjectionMatrix, const Matrix4x4& _viewportMatrix, uint32_t _color);
void DrawSphere(const Sphere& _sphere, const Matrix4x4& _viewProjectionMatrix, const Matrix4x4& _viewportMatrix, uint32_t _color);
void DrawPlane(const Plane& _plane, const Matrix4x4& _viewProjectionMatrix, const Matrix4x4& _viewportMatrix, uint32_t _color);
void DrawTriangle(const Triangle& _triangle, const Matrix4x4& _viewProjectionMatrix, const Matrix4x4& _viewportMatrix, uint32_t _color);
void DrawAABB(const AABB& _aabb, const Matrix4x4& _viewProjectionMatrix, const Matrix4x4& _viewportMatrix, uint32_t _color);

bool IsCollision(const Sphere& _s1, const Sphere& _s2);
bool IsCollision(const Sphere& _sphere, const Plane& _plane);
bool IsCollision(const Segment& _segment, const Plane& _plane);
bool IsCollision(const Line& _line, const Plane& _plane);
bool IsCollision(const Ray& _ray, const Plane& _plane);
bool IsCollision(const Segment& _segment, const Triangle& _triangle);
bool IsCollision(const Segment& _segment, const Triangle& _triangle, const Matrix4x4& _viewProjectionMatrix, const Matrix4x4& _viewportMatrix);
bool IsCollision(const AABB& _aabb1, const AABB& _aabb2);
bool IsCollision(const AABB& _aabb, const Sphere& _sphere);
