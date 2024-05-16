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
const int kWindowWidth = 1280;
const int kWindowHeight = 720;

void	Initialize();
void	ImGuiWnd();
void	Update();
void	Draw();

Matrix4x4	cameraMatrix{};
Matrix4x4	viewMatrix{};
Matrix4x4	projectionMatrix{};
Matrix4x4	viewportMatrix{};
Matrix4x4	viewProjectionMatrix{};
Vector3		cameraPosition{};
Vector3		cameraRotation{};

Segment		segment{ {-2.0f, -1.0f, 0.0f}, {3.0f, 2.0f, 2.0f} };
Vector3		point{ -1.5f, 0.6f, 0.6f };
Vector3		project{};
Vector3		closestPoint{};

Sphere		pointSphere{ point,0.01f };
Sphere		closestPointSphere;

Vector3		start{};
Vector3		end{};

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	char keys[256] = {0};
	char preKeys[256] = {0};

	Initialize();

	while (Novice::ProcessMessage() == 0) {
		Novice::BeginFrame();

		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		ImGuiWnd();
		Update();
		Draw();

		Novice::EndFrame();

		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	Novice::Finalize();
	return 0;
}

void Initialize()
{
	// 初期化
	cameraPosition.x = 0.0f;
	cameraPosition.y = 1.9f;
	cameraPosition.z = -6.49f;

	cameraRotation.x = 0.26f;
	cameraRotation.y = 0.0f;
	cameraRotation.z = 0.0f;

	project = Project(Subtract(point, segment.origin), segment.diff);
	closestPoint = ClosestPoint(point, segment);

	closestPointSphere = Sphere(closestPoint, 0.01f);
}

void ImGuiWnd()
{

	/// IMGUI

	ImGui::SetNextWindowPos(ImVec2(960, 80));
	ImGui::SetNextWindowSize(ImVec2(300, 320));
	ImGui::Begin("DebugWindow");

	if (ImGui::TreeNode("Camera")) {
		if (ImGui::TreeNode("Position"))
		{
			ImGui::SliderFloat("X", &cameraPosition.x, -20.0f, 20.0f);
			ImGui::SliderFloat("Y", &cameraPosition.y, -20.0f, 20.0f);
			ImGui::SliderFloat("Z", &cameraPosition.z, -20.0f, 20.0f);

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

	if (ImGui::TreeNode("ProjectVector"))
	{
		ImGui::InputFloat3("Project", &project.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
		ImGui::TreePop();
	}

	ImGui::End();

	///
}

void Update()
{
	if (cameraPosition.z == 0) cameraPosition.z = 0.001f;

	cameraMatrix = MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, cameraRotation, cameraPosition);
	viewMatrix = Inverse(cameraMatrix);
	projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
	viewportMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);
	viewProjectionMatrix = Multiply(viewMatrix, projectionMatrix);
	start = Transform(Transform(segment.origin, viewProjectionMatrix), viewportMatrix);
	end = Transform(Transform(Add(segment.origin, segment.diff), viewProjectionMatrix), viewportMatrix);
}

void Draw()
{
	DrawGrid(Multiply(viewMatrix, projectionMatrix), viewportMatrix);

	Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), WHITE);

	DrawSphere(pointSphere, viewProjectionMatrix, viewportMatrix, RED);
	DrawSphere(closestPointSphere, viewProjectionMatrix, viewportMatrix, BLACK);
}
