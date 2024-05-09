#include <Novice.h>
#include "Vector3/calc/vector3calc.h"
#include "Vector3/output/vector3out.h"
#include "Matrix4x4/calc/matrix4calc.h"
#include "Matrix4x4/output/matrix4out.h"
#include <stdint.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <ImGuiManager.h>
#include "3d/DebugFunction/Debug3D.h"
#include "3d/Object/Object3d.h"

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
	
	Sphere sphere{};
	sphere.center = Vector3(0, 0, 0);
	sphere.radius = 1.0f;

	Matrix4x4 cameraMatrix{};
	Matrix4x4 viewMatrix{};
	Matrix4x4 projectionMatrix{};
	Matrix4x4 viewportMatrix{};
	Vector3 cameraPosition{};
	cameraPosition.x = 0.0f;
	cameraPosition.y = 1.9f;
	cameraPosition.z = -6.49f;
	Vector3 cameraRotation{};
	cameraRotation.x = 0.26f;
	cameraRotation.y = 0.0f;
	cameraRotation.z = 0.0f;


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
		
		}
		if (keys[DIK_S])
		{
			
		}
		if (keys[DIK_A])
		{
			
		}
		if (keys[DIK_D])
		{
			
		}

		cameraMatrix = MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, cameraRotation, cameraPosition);
		viewMatrix = Inverse(cameraMatrix);
		projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		viewportMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);

		///
		/// ↑更新処理ここまで
		///
		
		///
		/// ↓描画処理ここから
		///

		DrawGrid(Multiply(viewMatrix, projectionMatrix), viewportMatrix);
		DrawSphere(sphere, Multiply(viewMatrix, projectionMatrix), viewportMatrix, 0x000000ff);

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
			if (ImGui::TreeNode("Rotation"))
			{
				ImGui::SliderFloat("X", &cameraRotation.x, -2 * float(M_PI), 2 * float(M_PI));
				ImGui::SliderFloat("Y", &cameraRotation.y, -2 * float(M_PI), 2 * float(M_PI));
				ImGui::SliderFloat("Z", &cameraRotation.z, -2 * float(M_PI), 2 * float(M_PI));

				ImGui::TreePop();
			}

			ImGui::TreePop();
		}
		
		if (ImGui::TreeNode("Sphere")) {
			if (ImGui::TreeNode("Position"))
			{
				ImGui::SliderFloat("X", &sphere.center.x,-50.0f,50.0f);
				ImGui::SliderFloat("Y", &sphere.center.y, -50.0f, 50.0f);
				ImGui::SliderFloat("Z", &sphere.center.z, -50.0f, 50.0f);

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
