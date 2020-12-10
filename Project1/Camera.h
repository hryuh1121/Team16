#pragma once
#include <DirectXMath.h>

//カメラ制御クラス
class Camera {

private:

	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

	
	// 注視点座標
	static XMFLOAT3 target;
	// 上方向ベクトル
	static XMFLOAT3 up;

	//視点セット
	static void Seteye(XMFLOAT3 eye);
	static const XMFLOAT3& Geteye() { return eye; }

	//注視点セット
	static void SetTarget(XMFLOAT3 target);
	static const XMFLOAT3& GetTarget() { return target; }



	//ビュー行列更新
	static void UpdateView();

public:
	//初期化設定
	static void InitializeCamera(int window_width, int window_height);
	//カメラ移動(視点&注視点)
	static void CameraMove(XMFLOAT3 move);
	//カメラ注視点をプレイヤーに
	static void PlayerTarget(XMFLOAT3 playerpos);
	// ビュー行列
	static XMMATRIX matView;
	// 射影行列
	static XMMATRIX matProjection;
	// 視点座標
	static XMFLOAT3 eye;
};