#include "Result.h"
#include "SceneManager.h"
#include <cassert>


void Result::Initialize(DirectXCommon* dxCommon, Input* input)
{
	assert(input);
	assert(dxCommon);
	this->input = input;
	this->dxCommon = dxCommon;
}

void Result::Update()
{
	if (input->TriggerKey(DIK_SPACE))
	{
		OutputDebugStringA("���U���g\n");//�o�̓E�C���h�E�Ɂu�^�C�g���v�ƕ\��
		SceneManager::Instance().LoadScene("Title");
	}
}

void Result::Draw()
{
}

void Result::Release()
{
	//�������
}
