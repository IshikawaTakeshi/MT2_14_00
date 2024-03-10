#include <Novice.h>
#include <math.h>
#include "MatrixMath.h"

const char kWindowTitle[] = "AL2_02_イシカワタケシ_MT2_14_00";

struct Box {
	Vector2 pos;
	Vector2 size;
	Vector2 direction;
	Vector2 velosity;
	Vector2 acceleration;
	float mass;
	unsigned int color;
};

float Length(Vector2 vector) {
	return sqrtf((vector.x * vector.x) + (vector.y * vector.y));
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	
	//描画する範囲座標
	Vector2 rangePosLT = { -640.0f, 360.0f };
	Vector2 rangePosRB = { 640.0f, -360.0f };
	Vector2 viewportPos = {0, 0};
	Vector2 viewportSize = { 1280.0f, 720.0f };

	//カメラ座標
	Vector2 cameraPos = { 200.0f,200.0f };
	Matrix3x3 cameraMatrix = MakeWVPVpmatrix(
		MakeAffineMatrix({ 1.0f,1.0f }, 0.0f, cameraPos),
		cameraPos, rangePosLT, rangePosRB, viewportPos, viewportSize
	);

	//矩形
	Box box = {
		{50.0f,50.0f},
		{100.0f,100.0f},
		{0,0},
		{0,0},
		{0,0},
		10.0f,
		0xffffffff
	};

	//グリッド線の座標
	//X座標
	Vector2 gridStartPos0 = { -500.0f,-50.0f };
	Vector2 gridEndPos0 = { 1280.0f,-50.0f };
	Matrix3x3 gridStartMatrix0 = MakeTranslateMatrix(gridStartPos0);
	Matrix3x3 gridEndMatrix0 = MakeTranslateMatrix(gridEndPos0);
	gridStartPos0 = Transform({ 0,0 }, MakeWVPVpmatrix(gridStartMatrix0, cameraPos, rangePosLT, rangePosRB, viewportPos, viewportSize));
	gridEndPos0 = Transform({ 0,0 }, MakeWVPVpmatrix(gridEndMatrix0, cameraPos, rangePosLT, rangePosRB, viewportPos, viewportSize));

	//Y座標
	Vector2 gridStartPos1 = { 0.0f,-200.0f };
	Vector2 gridEndPos1 = { 0.0f,720.0f };
	Matrix3x3 gridStartMatrix1 = MakeTranslateMatrix(gridStartPos1);
	Matrix3x3 gridEndMatrix1 = MakeTranslateMatrix(gridEndPos1);
	gridStartPos1 = Transform({ 0,0 }, MakeWVPVpmatrix(gridStartMatrix1, cameraPos, rangePosLT, rangePosRB, viewportPos, viewportSize));
	gridEndPos1 = Transform({ 0,0 }, MakeWVPVpmatrix(gridEndMatrix1, cameraPos, rangePosLT, rangePosRB, viewportPos, viewportSize));


	//矩形の行列
	Matrix3x3 boxMatrix = MakeAffineMatrix({ 1.0f,1.0f }, 0.0f, box.pos);

	//重力加速度
	const Vector2 kGravity = { 0.0f,-9.8f };
	//摩擦の向き
	Vector2 frictDirection = { 0,0 };
	//動摩擦力
	Vector2 frictionalForce = { 0,0 };
	//動摩擦係数
	float miu = 0.4f;
	//垂直抗力の大きさ
	float magnitude = miu * Length({ -box.mass * kGravity.x,-box.mass * kGravity.y });
	
	//boxのレンダリングパイプライン
	box.pos = Transform({ 0,0 }, MakeWVPVpmatrix(boxMatrix, cameraPos, rangePosLT, rangePosRB, viewportPos, viewportSize));



	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

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

		//boxの初速度
		if (keys[DIK_SPACE] != 0 && preKeys[DIK_SPACE] == 0) {
			box.velosity.x = 70.0f;
		}

		//摩擦力の向き
		frictDirection.x = -box.velosity.x;
		frictDirection.y = -box.velosity.y;
		if (box.velosity.x != 0.0f) {
			frictDirection.x = frictDirection.x / Length(frictDirection);
			frictDirection.y = frictDirection.y / Length(frictDirection);
		}

		//boxが動いているかどうか
		if (fabs(box.velosity.x) > 0.01f || fabs(box.velosity.y) > 0.01f) {
			//動摩擦力
			frictionalForce.x = magnitude * frictDirection.x;
			frictionalForce.y = magnitude * frictDirection.y;
		}

		//動摩擦力によって発生する加速度
		box.acceleration.x = frictionalForce.x / box.mass;
		box.acceleration.y = frictionalForce.y / box.mass;
		
		//加速度の絶対値の計算
		if (fabs(box.acceleration.x / 60.0f) > fabs(box.velosity.x)) {
			box.acceleration.x = box.velosity.x * 60.0f;
		}

		//boxの移動
		box.velosity.x += box.acceleration.x / 60.0f;
		box.velosity.y += box.acceleration.y / 60.0f;
		box.pos.x += box.velosity.x / 60.0f;
		box.pos.y += box.velosity.y / 60.0f;
		
		boxMatrix = MakeAffineMatrix({ 1.0f,1.0f }, 0.0f, box.pos);
		
		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		Novice::DrawBox(
			static_cast<int>(box.pos.x),
			static_cast<int>(box.pos.y),
			static_cast<int>(box.size.x),
			static_cast<int>(box.size.y),
			0.0f,
			box.color,
			kFillModeSolid
		);

		Novice::ScreenPrintf(0, 50, "mass = %f", box.mass);
		Novice::ScreenPrintf(0, 100, "frictdirection = %f,%f", frictDirection.x, frictDirection.y);
		Novice::ScreenPrintf(0, 150, "boxVelosity = %f,%f", box.velosity.x, box.velosity.y);
		Novice::ScreenPrintf(0, 200, "boxacceleration = %f,%f", box.acceleration.x, box.acceleration.y);

		//グリッド線
		Novice::DrawLine(
			static_cast<int>(gridStartPos0.x),
			static_cast<int>(gridStartPos0.y),
			static_cast<int>(gridEndPos0.x),
			static_cast<int>(gridEndPos0.y),
			0xff0000ff
		);

		Novice::DrawLine(
			static_cast<int>(gridStartPos1.x),
			static_cast<int>(gridStartPos1.y),
			static_cast<int>(gridEndPos1.x),
			static_cast<int>(gridEndPos1.y),
			0x00ff00ff
		);


		///
		/// ↑描画処理ここまで
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
