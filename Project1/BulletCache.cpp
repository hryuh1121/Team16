#include "BulletCache.h"

BulletCache::~BulletCache()
{
	while (!cache.empty()) {
		auto bullet = cache.front();
		delete bullet;
		cache.pop();
	}
}

Bullet * BulletCache::Instantiate(DirectX::XMFLOAT3 pos, Model* model)
{
	//�L���b�V�����c���Ă�ꍇ������g�p
	if (cache.size() > 0)
	{
		//front�֐����g�p���L���[�擪�̃I�u�W�F�N�g�擾
		auto bullet = cache.front();

		//���������������s
		bullet->Initialize(model);
		//�v���C���[�̈ʒu��
		bullet->SetPosition(pos);

		//�L���[���|�b�v����
		cache.pop();
		return bullet;
	}

	//�C���X�^���X�𐶐�����
	auto bullet = new Bullet(pos);

	//���������������s
	bullet->Initialize(model);

	return bullet;
}

void BulletCache::Cache(Bullet * bullet)
{

	cache.push(bullet);
}
