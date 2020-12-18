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

public:
	Enemy();
	Enemy(XMFLOAT3 pos);
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

	const XMFLOAT3 GetPosition() { return position; }

	void Hit();//�����������̏���

private:

	/// <summary>
	/// �G�l�~�[�ړ�
	/// </summary>
	void EnemyMove();

private:
	Object3d* enemy = nullptr;
	Model* model = nullptr;
	Player* player = nullptr;

	bool isdead = false;

	XMFLOAT3 position = { 0,0,0 };
public:
	float radius = 10;//�����蔻��p���a
};