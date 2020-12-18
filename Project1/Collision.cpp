#include "Collision.h"

bool Collision::SphereToSphere(Bullet& bullet, Enemy& enemy)
{
	XMFLOAT3 bulletpos = bullet.GetPosition();
	XMFLOAT3 enemypos = enemy.GetPosition();

	float X = bulletpos.x - enemypos.x;//x座標の差
	float Y = bulletpos.y - enemypos.y;//y座標の差
	float Z = bulletpos.z - enemypos.z;//z座標の差
	float R = bullet.radius + bullet.radius;//半径の合計

	if (sqrt(X*X + Y * Y + Z * Z) <= R)
	{
		return true;
	}


	return false;
}
