#include "Collision.h"

bool Collision::SphereToSphere(Bullet& bullet, Enemy& enemy)
{
	XMFLOAT3 bulletpos = bullet.GetPosition();
	XMFLOAT3 enemypos = enemy.GetPosition();

	float X = bulletpos.x - enemypos.x;//x���W�̍�
	float Y = bulletpos.y - enemypos.y;//y���W�̍�
	float Z = bulletpos.z - enemypos.z;//z���W�̍�
	float R = bullet.radius + bullet.radius;//���a�̍��v

	if (sqrt(X*X + Y * Y + Z * Z) <= R)
	{
		return true;
	}


	return false;
}
