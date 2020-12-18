#pragma once
#include "Bullet.h"
#include "Enemy.h"

class Collision
{
public:
	///<summary>
	///’e‚Æ“G“–‚½‚è”»’è
	///</summary>
	///<param name ="bullet">’e</param>
	///<param name ="enemy">“G</param>
	///<refums>“–‚½‚Á‚Ä‚é‚©</refums>
	static bool SphereToSphere(Bullet& bullet, Enemy& enemy);
};