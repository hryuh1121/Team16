#include "Enemy.h"
#include"SafeDelete.h"
#include"Camera.h"
#include<DirectXTex.h>

Enemy::Enemy()
{
}

Enemy::Enemy(XMFLOAT3 pos)
{
	this->position = pos;
}

Enemy::~Enemy()
{
	safe_delete(enemy);
	safe_delete(model);
}

void Enemy::Initialize()
{
	model = Model::CreateFromOBJ(3, "triangle_mat2");

	enemy = Object3d::Create({ 0,0,0 }, model);

	XMFLOAT3 scale = enemy->GetScale();
	scale = { 20,20,20 };
	enemy->SetScale(scale);
	isdead = false;
}

void Enemy::Update()
{
	enemy->SetPosition(position);
	EnemyMove();
	enemy->Update();
}

void Enemy::Draw()
{
	enemy->Draw();
}

void Enemy::Hit()
{
	isdead = true;
}

void Enemy::EnemyMove()
{
	position.z -= 1;
	enemy->SetPosition(position);
}
