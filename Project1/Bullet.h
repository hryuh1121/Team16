#pragma once
#include"Object3d.h"
#include"Sprite.h"
#include"Input.h"

using namespace DirectX;
class Bullet
{
private:
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略

	//using XMFLOAT3 = DirectX::XMFLOAT3;
	//using XMFLOAT4 = DirectX::XMFLOAT4;
	//using XMMATRIX = DirectX::XMMATRIX;
public:
	Bullet(Object3d *obj, XMFLOAT3& pos, XMFLOAT3& vec);
	Bullet(XMFLOAT3& pos);
	~Bullet();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model);

	/// <summary>
	/// 毎フレーム
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// ベクトルとベクトルの加算
	/// </summary>
	/// <param name="position1"></param>
	/// <param name="position2"></param>
	XMFLOAT3 VAdd(const XMFLOAT3& position1, const XMFLOAT3& position2);

	bool BulletSeizon();

	void SetPosition(XMFLOAT3 setpos);

	const XMFLOAT3& GetPosition() { return position; }

private:
	Object3d *bullet = nullptr;
	XMFLOAT3 position;
	Input* input = nullptr;
	Model* model = nullptr;
	bool bulletflag = false;
	bool seizonFlag = false;
	int seizonTime = 0;
public:
	float radius = 10;//当たり判定用半径

};