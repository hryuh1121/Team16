#include "Title.h"
#include "SceneManager.h"
#include <cassert>
#include "SafeDelete.h"

void Title::Initialize(DirectXCommon* dxCommon, Input* input)
{
	assert(input);
	assert(dxCommon);
	this->input = input;
	this->dxCommon = dxCommon;

	Sprite::SpriteLoadTexture(1, L"Resources/LineBlack.png");
	Sprite::SpriteLoadTexture(0, L"Resources/Title.png");

	//�e�N�X�`���ԍ� ������W
	title = Sprite::Create(1, { 0.0f,0.0f });

}

void Title::Update()
{
	if (input->TriggerKey(DIK_SPACE))
	{
		OutputDebugStringA("�^�C�g��\n");//�o�̓E�C���h�E�Ɂu�^�C�g���v�ƕ\��
		SceneManager::Instance().LoadScene("GamePlay");
	}
}

void Title::Draw()
{
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();
	Sprite::PreDraw(cmdList);

	title->Draw();

	Sprite::PostDraw();
}

void Title::Release()
{
	//�������
	safe_delete(title);

}
