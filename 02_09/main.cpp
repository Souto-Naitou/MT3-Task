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

Matrix4x4	rotateMatrix{};
Vector3		rotate{};
OBB			obb
{
	.center = {-1.0f, 0.0f, 0.0f},
	.orientations =
	{
		{1.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 1.0f},
	},
	.size = {0.5f, 0.5f, 0.5f}
};

Segment		segment
{
	.origin = {-0.8f, -0.3f, 0.0f},
	.diff = {0.5f, 0.5f, 0.5f}
};

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
				if (ImGui::BeginTabItem("OBB"))
				{
					ImGui::Spacing();
					
					ImGui::Text("Transform");
					
					ImGui::PushID("OBB_TRANSFORM");
					ImGui::DragFloat3("Center", &obb.center.x, 0.01f);
					ImGui::Spacing();
					ImGui::Separator();
					ImGui::Spacing();
					ImGui::DragFloat3("Rotate", &rotate.x, 0.01f);
					ImGui::Spacing();
					ImGui::Separator();
					ImGui::Spacing();
					ImGui::DragFloat3("Orientation[0]", &obb.orientations[0].x, 0.01f);
					ImGui::DragFloat3("Orientation[1]", &obb.orientations[1].x, 0.01f);
					ImGui::DragFloat3("Orientation[2]", &obb.orientations[2].x, 0.01f);
					ImGui::Spacing();
					ImGui::Separator();
					ImGui::Spacing();
					ImGui::DragFloat3("Size", &obb.size.x, 0.01f);
					ImGui::PopID();

					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Segment"))
				{
					ImGui::Spacing();

					ImGui::Text("Transform");

					ImGui::PushID("SEGMENT_TRANSFORM");
					ImGui::DragFloat3("Origin", &segment.origin.x, 0.01f);
					ImGui::DragFloat3("Diff", &segment.diff.x, 0.01f);
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

	// 回転行列を作成
	rotateMatrix = Multiply(MakeRotateXMatrix(rotate.x), Multiply(MakeRotateYMatrix(rotate.y), MakeRotateZMatrix(rotate.z)));

	for (int i = 0; i < 3; i++)
	{
		obb.orientations[i].x = rotateMatrix.m[i][0];
		obb.orientations[i].y = rotateMatrix.m[i][1];
		obb.orientations[i].z = rotateMatrix.m[i][2];
	}
	
	if (enableCollisionDebug)
		isCollision = IsCollision(segment, obb, viewProjectionMatrix, viewportMatrix);
	else isCollision = IsCollision(segment, obb);
}

void Draw()
{
	DrawGrid(viewProjectionMatrix, viewportMatrix);
	DrawLine(segment.origin, segment.diff, viewProjectionMatrix, viewportMatrix, WHITE);
	DrawOBB(obb, viewProjectionMatrix, viewportMatrix, isCollision ? RED : WHITE);
	if (enableElementsNumber)
	{
		Vector3 scrpt1 = obb.center;
		Vector3 scrpt2 = segment.origin + Multiply(0.5f, segment.diff);

		WorldPrint(scrpt1, viewProjectionMatrix, viewportMatrix, "OBB");
		WorldPrint(scrpt2, viewProjectionMatrix, viewportMatrix, "Segment");
	}
}
