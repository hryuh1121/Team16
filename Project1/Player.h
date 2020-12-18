#pragma once
#include "Object3d.h"
#include "Sprite.h"
#include "Input.h"
#include <queue>
#include "BulletCache.h"

class Bullet;
class Player
{
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;


public:
	Player();
	~Player();

	//������
	void Initialize(Input* input);

	//���t���[��
	void Update();

	//�`��
	void Draw();

	void Shot(const XMFLOAT3& pos, Model* model);

	void ShotUpdate();



private:
	//�v���C���[�̈ړ�
	void playerMove();

	//�J�����ݒ�
	void cameraPos();

private://�����o�ϐ�
	Object3d* player = nullptr;
	Input* input = nullptr;
	Model* triangleModel = nullptr;

	Model* bulletModel = nullptr;

	//std::queue <Bullet*> bullets;


	BulletCache cache;

	float texNumber = 0;
	XMFLOAT3 position = { 0,0,0 };//���[�J�����W
public:
	std::vector<Bullet*> bullets;
};