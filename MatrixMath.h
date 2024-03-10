#pragma once
#include "Vector2.h"
#include "Matrix3x3.h"
#include <assert.h>
#define _USE_MATH_DEFINES
#include <math.h>

//3x3行列の積
Matrix3x3 Multiply(Matrix3x3 matrix1, Matrix3x3 matrix2);

// 平行移動行列の作成関数
Matrix3x3 MakeTranslateMatrix(Vector2 translate);


/// <summary>
/// 3x3アフィン行列
/// </summary>
/// <param name="scale">拡縮の割合</param>
/// <param name="rotate">回転の割合</param>
/// <param name="translate">移動させる座標</param>
Matrix3x3 MakeAffineMatrix(Vector2 scale, float rotate, Vector2 translate);

//3x3行列に対応した2次元ベクトルを同次座標系として変換する関数
Vector2 Transform(Vector2 vector, Matrix3x3 matrix);

/// <summary>
///3x3正射影行列
/// </summary>
/// <param name="LT">描画する範囲の左上座標</param>
/// <param name="RB">描画する範囲の右下座標</param>
Matrix3x3 MakeOrthographicMatrix(Vector2 LT, Vector2 RB);


/// <summary>
///3x3ビューポート行列
/// </summary>
/// <param name="viewportLT">ビューポート原点</param>
/// <param name="size">描画するサイズ</param>
Matrix3x3 MakeViewportMatrix(Vector2 viewportLT, Vector2 size);

//3x3逆行列
Matrix3x3 InverseMatrix(Matrix3x3 matrix);

/// <summary>
///3x3レンダリングパイプライン
/// </summary>
/// <param name="worldMatrix">変換させる行列</param>
/// /// <param name="cameraPos">カメラの座標</param>
/// <param name="LT">描画する範囲の左上座標</param>
/// <param name="RB">描画する範囲の右下座標</param>
/// /// <param name="viewportLT">ビューポート原点</param>
/// <param name="size">描画するサイズ</param>
Matrix3x3 MakeWVPVpmatrix(Matrix3x3 worldMatrix, Vector2 cameraPos, Vector2 LT, Vector2 RB, Vector2 viewportLT, Vector2 size);
