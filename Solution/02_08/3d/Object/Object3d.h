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

struct OBB
{
	Vector3 center; // 中心点
	Vector3 orientations[3]; // 座標軸。正規化・直交必須
	Vector3 size; // 座標軸方向の長さの半分。中心から面までの距離
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
void DrawOBB(const OBB& _obb, const Matrix4x4& _viewProjectionMatrix, const Matrix4x4& _viewportMatrix, uint32_t _color);

bool IsCollision(const Sphere& _s1, const Sphere& _s2);
bool IsCollision(const Sphere& _sphere, const Plane& _plane);
bool IsCollision(const Segment& _segment, const Plane& _plane);
bool IsCollision(const Line& _line, const Plane& _plane);
bool IsCollision(const Ray& _ray, const Plane& _plane);
bool IsCollision(const Segment& _segment, const Triangle& _triangle);
bool IsCollision(const Segment& _segment, const Triangle& _triangle, const Matrix4x4& _viewProjectionMatrix, const Matrix4x4& _viewportMatrix);
bool IsCollision(const AABB& _aabb1, const AABB& _aabb2);
bool IsCollision(const AABB& _aabb, const Sphere& _sphere);
bool IsCollision(const AABB& _aabb, const Segment& _segment);
bool IsCollision(const AABB& _aabb, const Line& _line);
bool IsCollision(const AABB& _aabb, const Ray& _ray);
bool IsCollision(const OBB& _obb, const Sphere& _sphere);
