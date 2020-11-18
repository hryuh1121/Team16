#include "Player.h"
#include <DirectXTex.h>
#include "SafeDelete.h"
#include "Camera.h"

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

	//���̐���
	player = Object3d::Create({ 0,0,0 });
	//�X�P�[�������l
	XMFLOAT3 scale = player->GetScale();
	scale = {20,20,20};
	player->SetScale(scale);
	

	player->SetModel(triangleModel);
}

void Player::Update()
{
	playerMove();

	//�V�F�[�_�[�؂�ւ�
	if (input->PushKey(DIK_K))
	{
		player->SetShader(L"BasicVertexShader.hlsl", "VSmain", L"BasicPixelShader.hlsl", "PSmain");
	}
	if (input->PushKey(DIK_J))
	{
		player->SetShader(L"OBJVertexShader.hlsl", "main", L"OBJPixelShader.hlsl", "main");
	}

	cameraPos();

	player->Update();

	
}

void Player::Draw()
{
	player->Draw();
}

void Player::playerMove()
{
	XMFLOAT3 pos = player->GetPosition();
	//���L�[�Ńv���C���[�O�㍶�E�ړ�
	if (input->PushKey(DIK_D)) {pos.x += 1;}
	else if(input->PushKey(DIK_A)) {pos.x -= 1;}
	if (input->PushKey(DIK_W)) { pos.z += 1; }
	else if (input->PushKey(DIK_S)) { pos.z -= 1; }

	player->SetPosition(pos);
}

void Player::cameraPos()
{
	XMFLOAT3 pos = player->GetPosition();
	//�����_���v���C���[�̍��W�ɐݒ�
	Camera::PlayerTarget(pos);
}


