#include "Object3d.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <Matrix4x4/calc/matrix4calc.h>
#include <Vector3/calc/vector3calc.h>
#include <Novice.h>

Vector3 Perpendicular(const Vector3& _vector)
{
	if (_vector.x != 0.0f || _vector.y != 0.0f)
	{
		return { -_vector.y, _vector.x, 0.0f };
	}
	return { 0.0f, -_vector.z, _vector.y };
}

void DrawSphere(const Sphere& _sphere, const Matrix4x4& _viewProjectionMatrix, const Matrix4x4& _viewportMatrix, uint32_t _color)
{
	const float pi = float(M_PI);
	const uint32_t kSubdivision = 16;
	const float kLonEvery = pi * 2 / kSubdivision;
	const float kLatEvery = pi / kSubdivision;
	const Matrix4x4 worldMatrix = MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, _sphere.center);
	const Matrix4x4 wvpMatrix = Multiply(worldMatrix, _viewProjectionMatrix);
	// 緯度の方向に分割 -pi / 2 ~ pi / 2
	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex)
	{
		float lat = -pi / 2.0f + kLatEvery * latIndex; // 現在の緯度
		// 経度方向に分割 0 ~ 2pi
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex)
		{
			float lon = lonIndex * kLonEvery; // 現在の経度
			// world座標系でa,b,cを求める
			Vector3 a =
			{
				_sphere.radius * std::cosf(lat) * std::cosf(lon),
				_sphere.radius * std::sinf(lat),
				_sphere.radius * std::cosf(lat) * std::sin(lon)
			};
			Vector3 b =
			{
				_sphere.radius* std::cosf(lat+kLatEvery)* std::cosf(lon),
				_sphere.radius* std::sinf(lat+kLatEvery),
				_sphere.radius* std::cosf(lat+kLatEvery)* std::sinf(lon)
			};
			Vector3 c =
			{
				_sphere.radius* std::cosf(lat)* std::cosf(lon + kLonEvery),
				_sphere.radius* std::sinf(lat),
				_sphere.radius* std::cosf(lat)* std::sinf(lon + kLonEvery)
			};
			
			Vector3 ndcA = Transform(a, wvpMatrix);
			Vector3 ndcB = Transform(b, wvpMatrix);
			Vector3 ndcC = Transform(c, wvpMatrix);

			Vector3 screenA = Transform(ndcA, _viewportMatrix);
			Vector3 screenB = Transform(ndcB, _viewportMatrix);
			Vector3 screenC = Transform(ndcC, _viewportMatrix);

			// ab,acで線を引く
			Novice::DrawLine(
				int(screenA.x), int(screenA.y),
				int(screenB.x), int(screenB.y),
				_color
			);
			Novice::DrawLine(
				int(screenA.x), int(screenA.y),
				int(screenC.x), int(screenC.y),
				_color
			);
		}
	}
}

void DrawPlane(const Plane& _plane, const Matrix4x4& _viewProjectionMatrix, const Matrix4x4& _viewportMatrix, uint32_t _color)
{
	Vector3 center = Multiply(_plane.distance, _plane.normal);
	Vector3 perpendiculars[4];
	perpendiculars[0] = Normalize(Perpendicular(_plane.normal));
	perpendiculars[1] = { -perpendiculars[0].x, -perpendiculars[0].y, -perpendiculars[0].z };
	perpendiculars[2] = Cross(_plane.normal, perpendiculars[0]);
	perpendiculars[3] = { -perpendiculars[2].x, -perpendiculars[2].y, -perpendiculars[2].z };

	Vector3 points[4];
	for (int32_t index = 0; index < 4; ++index)
	{
		Vector3 extend = Multiply(2.0f, perpendiculars[index]);
		Vector3 point = Add(center, extend);
		points[index] = Transform(Transform(point, _viewProjectionMatrix), _viewportMatrix);
	}

	Novice::DrawLine(int(points[0].x), int(points[0].y), int(points[2].x), int(points[2].y), _color);
	Novice::DrawLine(int(points[1].x), int(points[1].y), int(points[3].x), int(points[3].y), _color);
	Novice::DrawLine(int(points[2].x), int(points[2].y), int(points[1].x), int(points[1].y), _color);
	Novice::DrawLine(int(points[3].x), int(points[3].y), int(points[0].x), int(points[0].y), _color);
	
}

bool IsCollision(const Sphere& _s1, const Sphere& _s2)
{
	float distance = Length(Subtract(_s1.center, _s2.center));
	if (distance <= _s1.radius + _s2.radius)
	{
		return true;
	}
	return false;
}

bool IsCollision(const Sphere& _sphere, const Plane& _plane)
{
	float k = Dot(_plane.normal, _sphere.center) - _plane.distance;
	k = sqrtf(powf(k, 2));
	return k <= _sphere.radius;
}
