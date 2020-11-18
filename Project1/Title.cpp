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

	//テクスチャ番号 左上座標
	title = Sprite::Create(1, { 0.0f,0.0f });

}

void Title::Update()
{
	if (input->TriggerKey(DIK_SPACE))
	{
		OutputDebugStringA("タイトル\n");//出力ウインドウに「タイトル」と表示
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
	//解放処理
	safe_delete(title);

}
