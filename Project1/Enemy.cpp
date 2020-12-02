#include "Enemy.h"
#include"SafeDelete.h"
#include"Camera.h"
#include<DirectXTex.h>

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
	safe_delete(enemy);
	safe_delete(model);
}

void Enemy::Initialize()
{
	model = Model::CreateFromOBJ(3, "triangle_mat2");

	enemy = Object3d::Create({ 0,0,800 });

	XMFLOAT3 scale = enemy->GetScale();
	scale = { 20,20,20 };
	enemy->SetScale(scale);

	enemy->SetModel(model);
}

void Enemy::Update()
{

	EnemyMove();
	enemy->Update();
}

void Enemy::Draw()
{
	enemy->Draw();
}

void Enemy::EnemyMove()
{
	XMFLOAT3 pos = enemy->GetPosition();
	pos.z -= 1;
	enemy->SetPosition(pos);
}
