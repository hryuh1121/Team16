#pragma once
#include "Object3d.h"
#include "Sprite.h"
#include"Player.h"

class Enemy
{
private:
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public :
	Enemy();
	~Enemy();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// ���t���[��
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

private:
	
	/// <summary>
	/// �G�l�~�[�ړ�
	/// </summary>
	void EnemyMove();

private:
	Object3d* enemy = nullptr;
	Model* model = nullptr;
	Player* player = nullptr;

	XMFLOAT3 position = { 0,0,0 };
};