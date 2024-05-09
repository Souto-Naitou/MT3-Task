#include "Object3d.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <Matrix4x4/calc/matrix4calc.h>
#include <Vector3/calc/vector3calc.h>
#include <Novice.h>

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
				_sphere.radius * std::cosf(lat + kLatEvery) * std::cosf(lon),
				_sphere.radius * std::sinf(lat + kLatEvery),
				_sphere.radius * std::cosf(lat + kLatEvery) * std::sinf(lon)
			};
			Vector3 c =
			{
				_sphere.radius * std::cosf(lat) * std::cosf(lon + kLonEvery),
				_sphere.radius * std::sinf(lat),
				_sphere.radius * std::cosf(lat) * std::sinf(lon + kLonEvery)
			};
			
			Vector3 ndcA = Transform(a, wvpMatrix);
			Vector3 ndcB = Transform(b, wvpMatrix);
			Vector3 ndcC = Transform(c, wvpMatrix);

			Vector3 screenA = Transform(ndcA, _viewportMatrix);
			Vector3 screenB = Transform(ndcB, _viewportMatrix);
			Vector3 screenC = Transform(ndcC, _viewportMatrix);

			// ab,bcで線を引く
			Novice::DrawLine(
				int(screenA.x), int(screenA.y),
				int(screenB.x), int(screenB.y),
				_color
			);
			Novice::DrawLine(
				int(screenB.x), int(screenB.y),
				int(screenC.x), int(screenC.y),
				_color
			);
		}
	}
}
