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
	//キャッシュが残ってる場合それを使用
	if (cache.size() > 0)
	{
		//front関数を使用しキュー先頭のオブジェクト取得
		auto bullet = cache.front();

		//初期化処理を実行
		bullet->Initialize(model);
		//プレイヤーの位置に
		bullet->SetPosition(pos);

		//キューをポップする
		cache.pop();
		return bullet;
	}

	//インスタンスを生成する
	auto bullet = new Bullet(pos);

	//初期化処理を実行
	bullet->Initialize(model);

	return bullet;
}

void BulletCache::Cache(Bullet * bullet)
{

	cache.push(bullet);
}
