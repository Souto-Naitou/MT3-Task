#include <Novice.h>
#include "Vector3/calc/vector3calc.h"
#include "Vector3/output/vector3out.h"
#include "Matrix4x4/calc/matrix4calc.h"
#include "Matrix4x4/output/matrix4out.h"
#include <stdint.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <ImGuiManager.h>

const char kWindowTitle[] = "学籍番号";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	const int kWindowWidth = 1280;
	const int kWindowHeight = 720;
	float kMoveSpeed = 0.1f;

	Vector3 v1{ 1.2f, -3.9f, 2.5f };
	Vector3 v2{ 2.8f, 0.4f, -1.3f };
	Vector3 cross = Cross(v1, v2);

	Vector3 rotate{};
	Vector3 translate{};
	translate.z = 6.0f;

	Matrix4x4 rotateMatrix{};
	Matrix4x4 worldMatrix{};
	Matrix4x4 cameraMatrix{};
	Matrix4x4 viewMatrix{};
	Matrix4x4 projectionMatrix{};
	Matrix4x4 worldViewProjectionMatrix{};
	Matrix4x4 viewportMatrix{};
	Vector3 screenVertices[3];
	Vector3 cameraPosition{};
	cameraPosition.x = 0.0f;
	cameraPosition.y = 0.0f;
	cameraPosition.z = 1.0f;
	const Vector3 kLocalVertices[3] = {
		{ 0.0f, 0.5f, 0.0f },
		{ 0.866f, -1.0f, 0.0f },
		{ -0.866f, -1.0f, 0.0f }
	};


	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);


		///
		/// ↓更新処理ここから
		///

		if (cameraPosition.z == 0) cameraPosition.z = 0.001f;

		if (keys[DIK_W])
		{
			translate.z -= kMoveSpeed;
		}
		if (keys[DIK_S])
		{
			translate.z += kMoveSpeed;
		}
		if (keys[DIK_A])
		{
			translate.x -= kMoveSpeed;
		}
		if (keys[DIK_D])
		{
			translate.x += kMoveSpeed;
		}
		rotate.y += float(M_PI) / 180.0f;


		worldMatrix = MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, rotate, translate);
		cameraMatrix = MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, { 0.1f, 0.0f, 0.0f }, cameraPosition);
		viewMatrix = Inverse(cameraMatrix);
		projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
		//worldViewProjectionMatrix = Multiply(projectionMatrix, Multiply(viewMatrix, worldMatrix));
		viewportMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);
		for (uint32_t i = 0; i < 3; ++i)
		{
			Vector3 ndcVertex = Transform(kLocalVertices[i], worldViewProjectionMatrix);
			screenVertices[i] = Transform(ndcVertex, viewportMatrix);
		}

		///
		/// ↑更新処理ここまで
		///
		
		///
		/// ↓描画処理ここから
		///

		VectorScreenPrint(0, 0, cross, "Cross");

		Novice::DrawTriangle(
			int(screenVertices[0].x), int(screenVertices[0].y),
			int(screenVertices[1].x), int(screenVertices[1].y),
			int(screenVertices[2].x), int(screenVertices[2].y),
			RED,
			kFillModeSolid
		);

		///
		/// ↑描画処理ここまで
		///

		/// IMGUI

		ImGui::SetNextWindowPos(ImVec2(960, 80));
		ImGui::SetNextWindowSize(ImVec2(300, 320));
		ImGui::Begin("DebugWindow");

		if (ImGui::TreeNode("Camera")) {
			if (ImGui::TreeNode("Position"))
			{
				ImGui::InputFloat("X", &cameraPosition.x);
				ImGui::InputFloat("Y", &cameraPosition.y);
				ImGui::InputFloat("Z", &cameraPosition.z);

				ImGui::TreePop();
			}

			ImGui::TreePop();
		}
		
		if (ImGui::TreeNode("Triangle")) {
			if (ImGui::TreeNode("Translate"))
			{
				ImGui::Text("Position");
				ImGui::InputFloat("X", &translate.x);
				ImGui::InputFloat("Y", &translate.y);
				ImGui::InputFloat("Z", &translate.z);

				ImGui::Text("Moving");
				ImGui::InputFloat("Speed", &kMoveSpeed);

				ImGui::TreePop();
			}

			ImGui::TreePop();
		}
		
		ImGui::End();

		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
