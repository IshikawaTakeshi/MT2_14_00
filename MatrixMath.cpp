#include "MatrixMath.h"

//3x3行列の積
Matrix3x3 Multiply(Matrix3x3 matrix1, Matrix3x3 matrix2) {
	Matrix3x3 result = { 0 };
	for (int r = 0; r < 3; r++) {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				result.m[r][i] += matrix1.m[r][j] * matrix2.m[j][i];
			}
		}
	}
	return result;
}

// 平行移動行列の作成関数
Matrix3x3 MakeTranslateMatrix(Vector2 translate) {
	Matrix3x3 result = {
		1.0f,0,0,
		0,1.0f,0,
		translate.x,translate.y,1.0f
	};

	return result;
}

//3x3アフィン行列
Matrix3x3 MakeAffineMatrix(Vector2 scale, float theta, Vector2 translate) {
	Matrix3x3 result = {
		scale.x * static_cast<float>(cos(theta)),
		scale.x * static_cast<float>(sin(theta)),
		0,
		-scale.y * static_cast<float>(sin(theta)),
		scale.y * static_cast<float>(cos(theta)),
		0,
		translate.x,
		translate.y,
		1.0f,
	};
	return result;
}

/// <summary>
///3x3行列に対応した2次元ベクトルを同次座標系として変換する関数
/// </summary>
/// <param name="vector">描画する原点座標</param>
/// <param name="matrix">変換させる行列</param>
Vector2 Transform(Vector2 vector, Matrix3x3 matrix) {
	Vector2 result;
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + 1.0f * matrix.m[2][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + 1.0f * matrix.m[2][1];
	float w = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + 1.0f * matrix.m[2][2];
	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	return result;
}


/// <summary>
///3x3正射影行列
/// </summary>
/// <param name="LT">描画する範囲の左上座標</param>
/// <param name="RB">描画する範囲の右下座標</param>
Matrix3x3 MakeOrthographicMatrix(Vector2 LT, Vector2 RB) {
	Matrix3x3 result{
		2.0f / (RB.x - LT.x),0,0,
		0,2.0f / (LT.y - RB.y),0,
		(LT.x + RB.x) / (LT.x - RB.x),
		(LT.y + RB.y) / (RB.y - LT.y),
		1
	};
	return result;
}


/// <summary>
///3x3ビューポート行列
/// </summary>
/// <param name="viewportLT">ビューポート原点</param>
/// <param name="size">描画するサイズ</param>
Matrix3x3 MakeViewportMatrix(Vector2 viewportLT,Vector2 size) {
	Matrix3x3 result{
		size.x / 2.0f,0,0,
		0,-size.y / 2.0f,0,
		viewportLT.x + (size.x / 2.0f),
		viewportLT.y + (size.y / 2.0f),
		1
	};
	return result;
}

//3x3逆行列
Matrix3x3 InverseMatrix(Matrix3x3 matrix) {
	float determinant =
		matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][2]
		+ matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][0]
		+ matrix.m[0][2] * matrix.m[1][0] * matrix.m[2][1]
		- matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][0]
		- matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][2]
		- matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][1];

	Matrix3x3 result;
	result.m[0][0] = 1.0f / determinant * (matrix.m[1][1] * matrix.m[2][2] - matrix.m[1][2] * matrix.m[2][1]);
	result.m[0][1] = 1.0f / determinant * ((matrix.m[0][1] * matrix.m[2][2] - matrix.m[0][2] * matrix.m[2][1]) * -1.0f);
	result.m[0][2] = 1.0f / determinant * (matrix.m[0][1] * matrix.m[1][2] - matrix.m[0][2] * matrix.m[1][1]);
	result.m[1][0] = 1.0f / determinant * ((matrix.m[1][0] * matrix.m[2][2] - matrix.m[1][2] * matrix.m[2][0]) * -1.0f);
	result.m[1][1] = 1.0f / determinant * (matrix.m[0][0] * matrix.m[2][2] - matrix.m[0][2] * matrix.m[2][0]);
	result.m[1][2] = 1.0f / determinant * ((matrix.m[0][0] * matrix.m[1][2] - matrix.m[0][2] * matrix.m[1][0]) * -1.0f);
	result.m[2][0] = 1.0f / determinant * (matrix.m[1][0] * matrix.m[2][1] - matrix.m[1][1] * matrix.m[2][0]);
	result.m[2][1] = 1.0f / determinant * ((matrix.m[0][0] * matrix.m[2][1] - matrix.m[0][1] * matrix.m[2][0]) * -1.0f);
	result.m[2][2] = 1.0f / determinant * (matrix.m[0][0] * matrix.m[1][1] - matrix.m[0][1] * matrix.m[1][0]);

	return result;
}

/// <summary>
///3x3レンダリングパイプライン
/// </summary>
/// <param name="worldMatrix">変換させる行列</param>
/// /// <param name="cameraPos">カメラの座標</param>
/// <param name="LT">描画する範囲の左上座標</param>
/// <param name="RB">描画する範囲の右下座標</param>
/// /// <param name="viewportLT">ビューポート原点</param>
/// <param name="size">描画するサイズ</param>
Matrix3x3 MakeWVPVpmatrix(Matrix3x3 worldMatrix,Vector2 cameraPos,Vector2 LT, Vector2 RB, Vector2 viewportLT, Vector2 size) {
	//行列の生成

	Matrix3x3 cameraMatrix = MakeAffineMatrix({ 1.0f,1.0f },0.0f,cameraPos);
	Matrix3x3 viewMatrix = InverseMatrix(cameraMatrix);
	Matrix3x3 orthoMatrix = MakeOrthographicMatrix(LT, RB);
	Matrix3x3 viewportMatrix = MakeViewportMatrix(viewportLT, size);
	Matrix3x3 wvpVpmatrix;

	//行列の結合
	wvpVpmatrix = Multiply(worldMatrix, viewMatrix);
	wvpVpmatrix = Multiply(wvpVpmatrix, orthoMatrix);
	wvpVpmatrix = Multiply(wvpVpmatrix, viewportMatrix);

	return wvpVpmatrix;
}
