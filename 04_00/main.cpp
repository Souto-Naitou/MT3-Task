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

// 定数
const		char		kWindowTitle[]	= "学籍番号";
const		int			kWindowWidth	= 1280;
const		int			kWindowHeight	= 720;
const		Vector3		defaultRot		= {0.0f, 0.0f, 0.0f};
const       float       kDeltaTime      = 1.0f / 60.0f;

// 関数
void		Initialize();
void		ImGuiWnd();
void		Update();
void		Draw();

// キー状態変数
char		keys[256]{};
char		preKeys[256]{};

// カメラ・描画関係
Matrix4x4	cameraMatrix{};
Matrix4x4	viewMatrix{};
Matrix4x4	projectionMatrix{};
Matrix4x4	viewportMatrix{};
Matrix4x4	viewProjectionMatrix{};
Vector3		cameraPosition{};
Vector3		cameraRotation{};

// 変数など
float		cameraSpeed				= 0.01f;
bool		isCollision				= false;
bool		enableElementsNumber	= true;
bool		enableCollisionDebug	= false;

// 課題用
Spring springs{};
Ball ball{};
bool isStopSpring{};

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

    springs.anchor = { 0.0f, 0.0f, 0.0f };
    springs.naturalLength = 1.0f;
    springs.stiffness = 100.0f;
    springs.dampingCoefficient = 2.0f;

    ball.position = { 1.2f, 0.0f, 0.0f };
    ball.mass = 2.0f;
    ball.radius = 0.05f;
    ball.color = BLUE;
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

        if (ImGui::BeginTabItem("Objects")) 
        {
        	if (ImGui::BeginTabBar("TABBER_OBJECTS"))
        	{
        		if (ImGui::BeginTabItem("Spring&Ball"))
        		{
        			ImGui::Spacing();
        			
        			ImGui::PushID("SPRING_TABITEM");
                    ImGui::DragFloat("positionX", &ball.position.x, 0.01f);
                    ImGui::Checkbox("Stop", &isStopSpring);
        			ImGui::PopID();

        			ImGui::EndTabItem();
        		}

        		ImGui::EndTabBar();
        	}

        	ImGui::EndTabItem();
        }

        //if (ImGui::BeginTabItem("Collision"))
        //{
        //	ImGui::Spacing();
        //	ImGui::Text("Flags");

        //	ImGui::Checkbox("Enable draw element name", &enableElementsNumber);
        //	ImGui::Checkbox("Enable collision debug", &enableCollisionDebug);

        //	ImGui::EndTabItem();
        //}

        //if (ImGui::BeginTabItem("DebugText"))
        //{
        //    ImGui::Spacing();
        //    ImGui::Text("c:%f, %f, %f", c.x, c.y, c.z);
        //    ImGui::Text("d:%f, %f, %f", d.x, d.y, d.z);
        //    ImGui::Text("e:%f, %f, %f", e.x, e.y, e.z);

        //    ImGui::Spacing();

        //    ImGui::Text(
        //        "matrix:\n%f, %f, %f, %f\n%f, %f, %f, %f\n%f, %f, %f, %f\n%f, %f, %f, %f", 
        //        rotateMatrix.m[0][0], rotateMatrix.m[0][1], rotateMatrix.m[0][2], rotateMatrix.m[0][3],
        //        rotateMatrix.m[1][0], rotateMatrix.m[1][1], rotateMatrix.m[1][2], rotateMatrix.m[1][3],
        //        rotateMatrix.m[2][0], rotateMatrix.m[2][1], rotateMatrix.m[2][2], rotateMatrix.m[2][3],
        //        rotateMatrix.m[3][0], rotateMatrix.m[3][1], rotateMatrix.m[3][2], rotateMatrix.m[3][3]
        //        );

        //    ImGui::EndTabItem();

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

    if (!isStopSpring)
    {
        Vector3 diff = ball.position - springs.anchor;
        float length = Length(diff);
        if (length != 0.0f)
        {
            Vector3 direction = Normalize(diff);
            Vector3 restPosition = springs.anchor + direction * springs.naturalLength;
            Vector3 displacement = (ball.position - restPosition) * length;
            Vector3 restoringForce = displacement * (-springs.stiffness);
            Vector3 dampingForce = ball.velocity * (-springs.dampingCoefficient);
            Vector3 force = restoringForce + dampingForce;
            ball.acceleration = force * (1.0f / ball.mass);
        }


        // 加速度も速度もどちらも秒を基準とした値である
        // それが、1/60描画間(deltaTime)適用されたと考える
        ball.velocity += ball.acceleration * kDeltaTime;
        ball.position += ball.velocity * kDeltaTime;
    }

}

void Draw()
{
    DrawGrid(viewProjectionMatrix, viewportMatrix);

    DrawSphere({ .center = ball.position, .radius = ball.radius }, viewProjectionMatrix, viewportMatrix, ball.color);
    DrawLine(springs.anchor, (ball.position - springs.anchor), viewProjectionMatrix, viewportMatrix, WHITE);

    if (enableElementsNumber)
    {
        //Vector3 scrpt1 = obb.center;
        //Vector3 scrpt2 = segment.origin + Multiply(0.5f, segment.diff);

        //WorldPrint(scrpt1, viewProjectionMatrix, viewportMatrix, "OBB");
        //WorldPrint(scrpt2, viewProjectionMatrix, viewportMatrix, "Segment");
    }
}
