#include "Player.h"
#include <DirectXTex.h>
#include "SafeDelete.h"
#include "Camera.h"
#include"Bullet.h"

Player::Player()
{
}

Player::~Player()
{
	safe_delete(player);
	safe_delete(triangleModel);
}



void Player::Initialize(Input* input)
{
	this->input = input;

	
	triangleModel = Model::CreateFromOBJ(1,"triangle_mat");

	//実体生成
	//生成時にモデルデータも入れる
	player = Object3d::Create({ 0,0,0 },triangleModel);
	//スケール初期値
	XMFLOAT3 scale = player->GetScale();
	scale = {20,10,20};
	player->SetScale(scale);
}

void Player::Update()
{
	playerMove();
	playerShot();
	//シェーダー切り替え
	if (input->PushKey(DIK_K))
	{
		player->SetShader(L"BasicVertexShader.hlsl", "VSmain", L"BasicPixelShader.hlsl", "PSmain");
	}
	if (input->PushKey(DIK_J))
	{
		player->SetShader(L"OBJVertexShader.hlsl", "main", L"OBJPixelShader.hlsl", "main");
	}

	cameraPos();

	if (flag)
	{
		bullet = new Bullet(position);
		bullet->Initialize();
		bullet->Update();
	}
	player->Update();

	
}

void Player::Draw()
{
	player->Draw();
	if (flag)
		bullet->Draw();
}

void Player::playerShot()
{
	XMFLOAT3 vec = { 0,0,-1 };
	static int cnt = 0;
	if (input->PushKey(DIK_SPACE))
	{
		cnt++;
		flag = true;
	}
	else
	{
		cnt = 0;
		flag = false;
	}
}

void Player::playerMove()
{
	XMFLOAT3 pos = player->GetPosition();
	//矢印キーでプレイヤー前後左右移動
	if (input->PushKey(DIK_D)) {pos.x += 1;}
	else if(input->PushKey(DIK_A)) {pos.x -= 1;}
	if (input->PushKey(DIK_W)) { pos.y += 1; }
	else if (input->PushKey(DIK_S)) { pos.y -= 1; }

	if (pos.x >= 42) { pos.x -= 1; }
	else if (pos.x <= -42) { pos.x += 1; }
	if (pos.y >= 26) { pos.y -= 1; }
	else if (pos.y <= -24) { pos.y += 1; }

	XMFLOAT3 rot = player->GetRotation();
	if (input->PushKey(DIK_D)) { rot.y -= 0.5f; }
	else if (input->PushKey(DIK_A)) { rot.y += 0.5f; }
	if (input->PushKey(DIK_W)) { rot.x += 0.5f; }
	else if (input->PushKey(DIK_S)) { rot.x -= 0.5f; }

	if (rot.y <= -21)
	{
		rot.y += 0.5f;
	}
	else if (rot.y >= 21) { rot.y -= 0.5f; }
	if (rot.x <= 18) { rot.x += 0.5f; }
	else if (rot.x >= -18) { rot.x -= 0.5f; }
	player->SetRotation(rot);
	player->SetPosition(pos);
}

void Player::cameraPos()
{
	XMFLOAT3 pos = player->GetPosition();
	//注視点をプレイヤーの座標に設定
	//Camera::PlayerTarget(pos);
}


