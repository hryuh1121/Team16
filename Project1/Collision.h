#pragma once
#include "Bullet.h"
#include "Enemy.h"

class Collision
{
public:
	///<summary>
	///�e�ƓG�����蔻��
	///</summary>
	///<param name ="bullet">�e</param>
	///<param name ="enemy">�G</param>
	///<refums>�������Ă邩</refums>
	static bool SphereToSphere(Bullet& bullet, Enemy& enemy);
};