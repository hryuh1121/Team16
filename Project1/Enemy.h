#pragma once
#include "Object3d.h"
#include "Sprite.h"
#include"Player.h"

class Enemy
{
private:
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	Enemy();
	Enemy(XMFLOAT3 pos);
	~Enemy();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	const XMFLOAT3 GetPosition() { return position; }

	void Hit();//当たった時の処理

private:

	/// <summary>
	/// エネミー移動
	/// </summary>
	void EnemyMove();

private:
	Object3d* enemy = nullptr;
	Model* model = nullptr;
	Player* player = nullptr;

	bool isdead = false;

	XMFLOAT3 position = { 0,0,0 };
public:
	float radius = 10;//当たり判定用半径
};