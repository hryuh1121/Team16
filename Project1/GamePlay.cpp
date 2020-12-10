#include "GamePlay.h"
#include "SceneManager.h"
#include <cassert>
#include "SafeDelete.h"
#include "Camera.h"

void GamePlay::Initialize(DirectXCommon* dxCommon, Input* input,Audio* audio)
{
	assert(input);
	assert(dxCommon);
	assert(audio);
	this->input = input;
	this->dxCommon = dxCommon;
	this->audio = audio;

	player = new Player();

	player->Initialize(input);

	enemy = new Enemy();
	enemy->Initialize();

	skyModel = Model::CreateFromOBJ(0,"skydome");
	ground = Model::CreateFromOBJ(2, "ground");
	mikuModel = Model::CreateFromPMD(3, "Model/初音ミク.pmd");//PMD読み込み

	skydome = Object3d::Create({ 0,0,9200 },skyModel);
	miku = Object3d::Create({ 0,0,0 }, mikuModel);
}

void GamePlay::Update()
{
	player->Update();
	enemy->Update();
	skydome->Update();
	miku->Update();

	if (input->TriggerKey(DIK_K))
	{
		audio->PlayWave("Resources/Alarm01.wav");
	}

	//// カメラ移動
	//if (input->PushKey(DIK_W) || input->PushKey(DIK_S) || input->PushKey(DIK_D) || input->PushKey(DIK_A))
	//{
	//	if (input->PushKey(DIK_W)) { Camera::CameraMove({ 0.0f,+1.0f,0.0f }); }
	//	else if (input->PushKey(DIK_S)) { Camera::CameraMove({ 0.0f,-1.0f,0.0f }); }
	//	if (input->PushKey(DIK_D)) { Camera::CameraMove({ +1.0f,0.0f,0.0f }); }
	//	else if (input->PushKey(DIK_A)) { Camera::CameraMove({ -1.0f,0.0f,0.0f }); }
	//}


	//スペースキーが押されたら
	if (input->TriggerKey(DIK_SPACE))
	{
 		OutputDebugStringA("ゲームプレイ\n");//出力ウインドウに「ゲームプレイ」と表示
		SceneManager::Instance().LoadScene("Result");//シーンをリザルトへ
	}
}

void GamePlay::Draw()
{
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();
	Object3d::PreDraw(cmdList);
	player->Draw();
	enemy->Draw();
	skydome->Draw();
	miku->Draw();
	Object3d::PostDraw();
}

void GamePlay::Release()
{
	//解放処理
	safe_delete(player);
	safe_delete(enemy);
	safe_delete(skydome);
	safe_delete(skyModel);
	safe_delete(ground);
	safe_delete(miku);
	safe_delete(mikuModel);
}
