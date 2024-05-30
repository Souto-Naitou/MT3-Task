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

const float cameraSpeed = 0.01f;
const Vector3 defaultRot = {0.0f, 0.0f, 0.0f};

void	Initialize();
void	ImGuiWnd();
void	Update();
void	Draw();

char keys[256] = { 0 };
char preKeys[256] = { 0 };

Matrix4x4	cameraMatrix{};
Matrix4x4	viewMatrix{};
Matrix4x4	projectionMatrix{};
Matrix4x4	viewportMatrix{};
Matrix4x4	viewProjectionMatrix{};
Vector3		cameraPosition{};
Vector3		cameraRotation{};

Plane		plane{};
Segment		segment{};

bool		isCollision = false;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

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

	plane.normal.y = 1.0f;
	plane.normal = Normalize(plane.normal);
	plane.distance = 1.0f;

	segment.diff = Vector3(1.0f, 1.0f, 0);
}

void ImGuiWnd()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImU32(0xff080808));
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImU32(0xff0C27D8));

	/// Window
	ImGui::SetNextWindowSize(ImVec2(300, 320));
	const ImGuiWindowFlags windowFlag = ImGuiWindowFlags_NoResize;
	ImGui::Begin("DebugWindow", (bool*)false, windowFlag);

	if (ImGui::BeginTabBar("ObjectsTab"))
	{
		if (ImGui::BeginTabItem("Camera"))
		{
			ImGui::PushID("CAMERA_TRANSLATION");
			ImGui::Spacing();
			ImGui::Text("Translate");
			ImGui::SliderFloat("X", &cameraPosition.x, -20.0f, 20.0f);
			ImGui::SliderFloat("Y", &cameraPosition.y, -20.0f, 20.0f);
			ImGui::SliderFloat("Z", &cameraPosition.z, -20.0f, 20.0f);
			ImGui::PopID();

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();
			
			ImGui::PushID("CAMERA_ROTATION");
			ImGui::Text("Rotation");
			ImGui::SliderFloat("X", &cameraRotation.x, -2 * float(M_PI), 2 * float(M_PI));
			ImGui::SliderFloat("Y", &cameraRotation.y, -2 * float(M_PI), 2 * float(M_PI));
			ImGui::SliderFloat("Z", &cameraRotation.z, -2 * float(M_PI), 2 * float(M_PI));
			ImGui::PopID();

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Segment")) {

			ImGui::Spacing();

			ImGui::Text("Origin Point");

			ImGui::PushID("SEGMENT_ORIGIN");
			ImGui::SliderFloat("X", &segment.origin.x, -3.0f, 3.0f);
			ImGui::SliderFloat("Y", &segment.origin.y, -3.0f, 3.0f);
			ImGui::SliderFloat("Z", &segment.origin.z, -3.0f, 3.0f);
			ImGui::PopID();

			ImGui::Spacing();
			
			ImGui::Text("Difference");

			ImGui::PushID("SEGMENT_DIFFERENCE");
			ImGui::SliderFloat("X", &segment.diff.x, -3.0f, 3.0f);
			ImGui::SliderFloat("Y", &segment.diff.y, -3.0f, 3.0f);
			ImGui::SliderFloat("Z", &segment.diff.z, -3.0f, 3.0f);
			ImGui::PopID();

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Plane")) {

			ImGui::Spacing();

			ImGui::Text("Transform");

			ImGui::PushID("PLANE_TRANSFORM");
			if (ImGui::DragFloat3("Normal", &plane.normal.x, 0.01f))
			{
				plane.normal = Normalize(plane.normal);
			}
			ImGui::DragFloat("Distance", &plane.distance, 0.01f);
			ImGui::PopID();

			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	ImGui::End();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar();
	///
}

void Update()
{
	if (keys[DIK_SPACE]) cameraPosition.y += cameraSpeed;
	if (keys[DIK_LSHIFT]) cameraPosition.y -= cameraSpeed;
	if (keys[DIK_A]) cameraPosition.x -= cameraSpeed;
	if (keys[DIK_D]) cameraPosition.x += cameraSpeed;
	if (keys[DIK_S]) cameraPosition.z -= cameraSpeed;
	if (keys[DIK_W]) cameraPosition.z += cameraSpeed;

	if (keys[DIK_UPARROW]) cameraRotation.x -= cameraSpeed;
	if (keys[DIK_DOWNARROW]) cameraRotation.x += cameraSpeed;
	if (keys[DIK_LEFTARROW]) cameraRotation.y -= cameraSpeed;
	if (keys[DIK_RIGHTARROW]) cameraRotation.y += cameraSpeed;
	if (keys[DIK_SEMICOLON]) cameraRotation.z -= cameraSpeed;
	if (keys[DIK_COLON]) cameraRotation.z += cameraSpeed;

	if (cameraPosition.z == 0) cameraPosition.z = 0.001f;
	cameraMatrix = MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, cameraRotation, cameraPosition);
	viewMatrix = Inverse(cameraMatrix);
	projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
	viewportMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);
	viewProjectionMatrix = Multiply(viewMatrix, projectionMatrix);

	isCollision = IsCollision(segment, plane);
}

void Draw()
{
	DrawGrid(viewProjectionMatrix, viewportMatrix);
	DrawLine(segment.origin, segment.diff, viewProjectionMatrix, viewportMatrix, isCollision ? RED : WHITE);
	DrawPlane(plane, viewProjectionMatrix, viewportMatrix, WHITE);
}
