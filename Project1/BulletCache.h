#pragma once
#include <queue>
#include "Bullet.h"
#include <DirectXMath.h>
#include "Model.h"

class BulletCache
{
public:
	//�f�X�g���N�^
	~BulletCache();

	//�e�̐���
	Bullet* Instantiate(DirectX::XMFLOAT3 pos, Model* model);

	//�e���L���b�V��
	void Cache(Bullet* bullet);

private:
	std::queue <Bullet*> cache;


};
