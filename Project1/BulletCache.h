#pragma once
#include <queue>
#include "Bullet.h"
#include <DirectXMath.h>
#include "Model.h"

class BulletCache
{
public:
	//デストラクタ
	~BulletCache();

	//弾の生成
	Bullet* Instantiate(DirectX::XMFLOAT3 pos, Model* model);

	//弾をキャッシュ
	void Cache(Bullet* bullet);

private:
	std::queue <Bullet*> cache;


};
