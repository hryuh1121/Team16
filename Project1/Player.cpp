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


	triangleModel = Model::CreateFromOBJ(1, "triangle_mat");

	//実体生成
	//生成時にモデルデータも入れる
	player = Object3d::Create({ 0,0,0 }, triangleModel);
	//スケール初期値
	XMFLOAT3 scale = player->GetScale();
	scale = { 20,10,20 };
	player->SetScale(scale);

	//弾のモデルデータ
	bulletModel = Model::CreateFromOBJ(4, "untitled");

}

void Player::Update()
{
	playerMove();
	cameraPos();


	XMFLOAT3 pos = player->GetPosition();

	player->Update();

	if (input->TriggerKey(DIK_SPACE))
		Shot(pos, bulletModel);

	ShotUpdate();
}

void Player::Draw()
{
	player->Draw();

	for (auto shot : bullets) {
		shot->Draw();
	}
}

void Player::Shot(const XMFLOAT3 &pos, Model* model)
{
	Bullet* shot = cache.Instantiate(pos, model);

	bullets.push_back(shot);
}

void Player::ShotUpdate()
{
	//イテレータを使用してすべての弾を処理
	for (auto it = bullets.begin(); it != bullets.end();) {

		//弾を更新
		(*it)->Update();
		//次のイテレータを参照
		++it;
	}
}

void Player::playerMove()
{
	XMFLOAT3 pos = player->GetPosition();
	//矢印キーでプレイヤー前後左右移動
	if (input->PushKey(DIK_D)) { pos.x += 1; }
	else if (input->PushKey(DIK_A)) { pos.x -= 1; }
	if (input->PushKey(DIK_W)) { pos.y += 1; }
	else if (input->PushKey(DIK_S)) { pos.y -= 1; }

	pos.z += 1;

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
	Camera::PlayerTarget(pos);
}


