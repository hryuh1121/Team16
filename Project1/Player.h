#pragma once
#include "Object3d.h"
#include "Sprite.h"
#include "Input.h"

class Player
{
private: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	Player();
	~Player();

	//初期化
	void Initialize(Input* input);

	//毎フレーム
	void Update();

	//描画
	void Draw();

private:
	//プレイヤーの移動
	void playerMove();

	//カメラ設定
	void cameraPos();

private://メンバ変数
	Object3d* player = nullptr;
	Input* input = nullptr;
	Model* triangleModel = nullptr;

	float texNumber = 0;
	XMFLOAT3 position = { 0,0,0 };//ローカル座標

};