#pragma once
#include "Bullet.h"
#include "Enemy.h"

class Collision
{
public:
	///<summary>
	///弾と敵当たり判定
	///</summary>
	///<param name ="bullet">弾</param>
	///<param name ="enemy">敵</param>
	///<refums>当たってるか</refums>
	static bool SphereToSphere(Bullet& bullet, Enemy& enemy);
};