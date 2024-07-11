#include <Novice.h>
#include "Vector3/calc/vector3calc.h"
#include "Vector3/output/vector3out.h"
#include "Matrix4x4/calc/matrix4calc.h"
#include "Matrix4x4/output/matrix4out.h"
#include <stdint.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <ImGuiManager.h>
#include <string>
#include "3d/DebugFunction/Debug3D.h"
#include "3d/Object/Object3d.h"
#include "NVDebug.h"

const		char		kWindowTitle[]	= "学籍番号";
const		int			kWindowWidth	= 1280;
const		int			kWindowHeight	= 720;
const		Vector3		defaultRot		= {0.0f, 0.0f, 0.0f};


void		Initialize();
void		ImGuiWnd();
void		Update();
void		Draw();

char		keys[256]{};
char		preKeys[256]{};

Matrix4x4	cameraMatrix{};
Matrix4x4	viewMatrix{};
Matrix4x4	projectionMatrix{};
Matrix4x4	viewportMatrix{};
Matrix4x4	viewProjectionMatrix{};
Vector3		cameraPosition{};
Vector3		cameraRotation{};

Vector3 translate[3] =
{
	{0.2f, 1.0f, 0.0f},
	{0.4f, 0.0f, 0.0f},
	{0.3f, 0.0f, 0.0f},
};
Vector3 rotate[3] =
{
	{0.0f, 0.0f, -6.8f},
	{0.0f, 0.0f, -1.4f},
	{0.0f, 0.0f, 0.0f},
};
Vector3 scales[3] =
{
	{1.0f, 1.0f, 1.0f},
	{1.0f, 1.0f, 1.0f},
	{1.0f, 1.0f, 1.0f},
};

Matrix4x4 sholderMatrix = {};
Matrix4x4 elbowMatrix = {};
Matrix4x4 handMatrix = {};

Matrix4x4 sholderWorldMatrix = {};
Matrix4x4 elbowWorldMatrix = {};
Matrix4x4 handWorldMatrix = {};

Vector3 sholderTranslate = {};
Vector3 elbowTranslate = {};
Vector3 handTranslate = {};

float		cameraSpeed				= 0.01f;
bool		isCollision				= false;
bool		enableElementsNumber	= true;
bool		enableCollisionDebug	= false;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	Initialize();

	while (Novice::ProcessMessage() == 0) {
		Novice::BeginFrame();
		NVDebug::Begin();

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
}

void ImGuiWnd()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImU32(0xff080808));
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImU32(0xff0C27D8));
	ImGui::PushStyleColor(ImGuiCol_TabActive, ImU32(0xff0C27D8));
	ImGui::PushStyleColor(ImGuiCol_TabHovered, ImU32(0xff273fd8));

	/// Window
	ImGui::SetNextWindowSize(ImVec2(320, 320));
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

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			ImGui::PushID("CAMERA_SPEED");
			ImGui::DragFloat("MoveSpeed", &cameraSpeed, 0.005f, 0.0f, 1.0f, "%.3f");
			ImGui::PopID();

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Objects")) {

			if (ImGui::BeginTabBar("TABBER_OBJECTS"))
			{
				if (ImGui::BeginTabItem("Sholder"))
				{
					ImGui::Spacing();
					
					ImGui::PushID("SHOLDER_TRANSFORM");
					ImGui::Text("Transform");
					ImGui::DragFloat3("Scale", &scales[0].x, 0.01f);
					ImGui::Spacing();
					ImGui::DragFloat3("Rotate", &rotate[0].x, 0.01f);
					ImGui::Spacing();
					ImGui::DragFloat3("Translate", &translate[0].x, 0.01f);
					ImGui::Spacing();
					ImGui::PopID();

					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Elbow"))
				{
					ImGui::Spacing();

					ImGui::PushID("ELBOW_TRANSFORM");
					ImGui::Text("Transform");
					ImGui::DragFloat3("Scale", &scales[1].x, 0.01f);
					ImGui::Spacing();
					ImGui::DragFloat3("Rotate", &rotate[1].x, 0.01f);
					ImGui::Spacing();
					ImGui::DragFloat3("Translate", &translate[1].x, 0.01f);
					ImGui::Spacing();
					ImGui::PopID();

					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Hand"))
				{
					ImGui::Spacing();

					ImGui::PushID("HAND_TRANSFORM");
					ImGui::Text("Transform");
					ImGui::DragFloat3("Scale", &scales[2].x, 0.01f);
					ImGui::Spacing();
					ImGui::DragFloat3("Rotate", &rotate[2].x, 0.01f);
					ImGui::Spacing();
					ImGui::DragFloat3("Translate", &translate[2].x, 0.01f);
					ImGui::Spacing();
					ImGui::PopID();

					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Collision"))
		{
			ImGui::Spacing();
			ImGui::Text("Flags");

			ImGui::Checkbox("Enable draw element name", &enableElementsNumber);
			ImGui::Checkbox("Enable collision debug", &enableCollisionDebug);

			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	ImGui::End();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
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

	// local
	sholderMatrix = MakeAffineMatrix(scales[0], rotate[0], translate[0]);
	elbowMatrix = MakeAffineMatrix(scales[1], rotate[1], translate[1]);
	handMatrix = MakeAffineMatrix(scales[2], rotate[2], translate[2]);

	// world
	sholderWorldMatrix = sholderMatrix;
	elbowWorldMatrix = Multiply(elbowMatrix, sholderWorldMatrix);
	handWorldMatrix = Multiply(handMatrix, elbowWorldMatrix);

	sholderTranslate = { sholderMatrix.m[3][0],sholderMatrix.m[3][1],sholderMatrix.m[3][2] };
	elbowTranslate = { elbowWorldMatrix.m[3][0],elbowWorldMatrix.m[3][1],elbowWorldMatrix.m[3][2] };
	handTranslate = { handWorldMatrix.m[3][0],handWorldMatrix.m[3][1],handWorldMatrix.m[3][2] };
}

void Draw()
{
	DrawGrid(viewProjectionMatrix, viewportMatrix);

	DrawSphere({ .center = sholderTranslate, .radius = 0.075f }, viewProjectionMatrix, viewportMatrix, RED);
	DrawLine(sholderTranslate, Subtract(elbowTranslate, sholderTranslate), viewProjectionMatrix, viewportMatrix, WHITE);
	DrawSphere({ .center = elbowTranslate, .radius = 0.075f }, viewProjectionMatrix, viewportMatrix, GREEN);
	DrawLine(elbowTranslate, Subtract(handTranslate, elbowTranslate), viewProjectionMatrix, viewportMatrix, WHITE);
	DrawSphere({ .center = handTranslate, .radius = 0.075f }, viewProjectionMatrix, viewportMatrix, BLUE);

	if (enableElementsNumber)
	{
		//Vector3 scrpt1 = obb.center;
		//Vector3 scrpt2 = segment.origin + Multiply(0.5f, segment.diff);

		//WorldPrint(scrpt1, viewProjectionMatrix, viewportMatrix, "OBB");
		//WorldPrint(scrpt2, viewProjectionMatrix, viewportMatrix, "Segment");
	}
}
