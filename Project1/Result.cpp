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
		OutputDebugStringA("リザルト\n");//出力ウインドウに「タイトル」と表示
		SceneManager::Instance().LoadScene("Title");
	}
}

void Result::Draw()
{
}

void Result::Release()
{
	//解放処理
}
