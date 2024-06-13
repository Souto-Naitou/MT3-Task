#include "Debug3D.h"
#include <Novice.h>
#include <stdint.h>
#include <Matrix4x4/calc/matrix4calc.h>
#include <Vector3/calc/vector3calc.h>
#include <Vector3.h>

void DrawGrid(const Matrix4x4& _viewProjectionMatrix, const Matrix4x4& _viewPortMatrix)
{
	const float kGridHalfWidth = 2.0f;										// Gridの半分の幅
	const uint32_t kSubdivision = 10u;										// 分割数
	const float kGridEvery = (kGridHalfWidth * 2.0f) / float(kSubdivision);	// 1つ分の長さ
	// 奥から手前への線を順々に引いていく
	Matrix4x4 worldMatrix = MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f,0.0f,0.0f });
	Matrix4x4 wvpMatrix = Multiply(worldMatrix, _viewProjectionMatrix);
	for (uint32_t xIndex = 0; xIndex <= kSubdivision; ++xIndex)
	{
		const Vector3 beginVertex{ kGridEvery * xIndex - kGridHalfWidth, 0, -kGridHalfWidth};
		const Vector3 endVertex{ kGridEvery * xIndex - kGridHalfWidth, 0, kGridHalfWidth};

		Vector3 ndcBeginVertex = Transform(beginVertex, wvpMatrix);
		Vector3 ndcEndVertex = Transform(endVertex, wvpMatrix);

		Vector3 screenBeginVertex = Transform(ndcBeginVertex, _viewPortMatrix);
		Vector3 screenEndVertex = Transform(ndcEndVertex, _viewPortMatrix);

		Novice::DrawLine(
			int(screenBeginVertex.x), int(screenBeginVertex.y),
			int(screenEndVertex.x), int(screenEndVertex.y),
			beginVertex.x == 0.0f ? 0x0000ffff : 0xAAAAAAFF
		);
	}
	// 左から右も同じように順々に引いていく
	for (uint32_t zIndex = 0; zIndex <= kSubdivision; ++zIndex)
	{
		const Vector3 beginVertex{ -kGridHalfWidth, 0, kGridEvery * zIndex - kGridHalfWidth};
		const Vector3 endVertex{ kGridHalfWidth, 0, kGridEvery * zIndex - kGridHalfWidth};

		Vector3 ndcBeginVertex = Transform(beginVertex, wvpMatrix);
		Vector3 ndcEndVertex = Transform(endVertex, wvpMatrix);

		Vector3 screenBeginVertex = Transform(ndcBeginVertex, _viewPortMatrix);
		Vector3 screenEndVertex = Transform(ndcEndVertex, _viewPortMatrix);

		Novice::DrawLine(
			int(screenBeginVertex.x), int(screenBeginVertex.y),
			int(screenEndVertex.x), int(screenEndVertex.y),
			beginVertex.z== 0.0f ? 0x0000ffff : 0xAAAAAAFF
		);
	}
}
