#include "Bullet.h"
#include"SafeDelete.h"
#include<DirectXTex.h>

XMFLOAT3 position;
XMFLOAT3 vecter;
Bullet::Bullet(Object3d* obj, XMFLOAT3 & pos, XMFLOAT3& vec)
{
	bullet = obj;
	//position = pos;
	vecter = vec;

}

Bullet::Bullet(XMFLOAT3& pos)
{
	position = pos;
}

Bullet::~Bullet()
{
	safe_delete(bullet);
	safe_delete(model);
}

void Bullet::Initialize(Model* model)
{
	this->input = input;
	bullet = Object3d::Create({ position.x,position.y,position.z }, model);
	//position = bullet->GetPosition();
	XMFLOAT3 scale = bullet->GetScale();
	scale = { 5,5,5 };
	bullet->SetScale(scale);
	//bullet->SetModel(model);
	bullet->SetPosition({ position.x,position.y,position.z });
}

void Bullet::Update()
{
	//position = bullet->GetPosition();
	//position = VAdd(position ,vecter);
	position.z += 2;
	bullet->SetPosition(position);
	bullet->Update();
}

void Bullet::Draw()
{
	bullet->Draw();
}


XMFLOAT3 Bullet::VAdd(const XMFLOAT3 & position1, const XMFLOAT3 & position2)
{
	XMFLOAT3 pos;
	pos.x = position1.x + position2.x;
	pos.y = position1.y + position2.y;
	pos.z = position1.z + position2.z;
	return pos;
}

bool Bullet::BulletSeizon()
{
	return seizonFlag;
}

void Bullet::SetPosition(XMFLOAT3 setpos)
{
	bullet->SetPosition(setpos);
}
