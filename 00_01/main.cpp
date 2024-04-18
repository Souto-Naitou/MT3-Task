#include <Novice.h>

#include "Vector3/calc/vector3calc.h"
#include "vector3/output/vector3out.h"

const char kWindowTitle[] = "学籍番号";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};
	
	Vector3 v1{ 1.0f, 3.0f, -5.0f };
	Vector3 v2{ 4.0f, -1.0f, 2.0f };
	float k = { 4.0f };

	const int kRowHeight = 20;
	int rowCount = 0;
	
	// Calculation
	Vector3 resultAdd = Add(v1, v2);
	Vector3 resultSubtract = Subtract(v1, v2);
	Vector3 resultMultiply = Multiply(k, v1);
	float resultDot = Dot(v1, v2);
	float resultLength = Length(v1);
	Vector3 resultNormalize = Normalize(v2);

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///---------------------///
		/// ↓更新処理ここから
		

		rowCount = 0;

		
		/// ↑更新処理ここまで
		///---------------------///



		///---------------------///
		/// ↓描画処理ここから
		
		
		VectorScreenPrint(15, 15 + kRowHeight * rowCount++, resultAdd, "  : Add");
		VectorScreenPrint(15, 15 + kRowHeight * rowCount++, resultSubtract, "  : Subtract");
		VectorScreenPrint(15, 15 + kRowHeight * rowCount++, resultMultiply, "  : Multiply");
		Novice::ScreenPrintf(15, 15 + kRowHeight * rowCount++, "%0.2f  : Dot", resultDot);
		Novice::ScreenPrintf(15, 15 + kRowHeight * rowCount++, "%0.2f  : Length", resultLength);
		VectorScreenPrint(15, 15 + kRowHeight * rowCount++, resultNormalize, "  : Normalize");

		
		/// ↑描画処理ここまで
		///---------------------///

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
