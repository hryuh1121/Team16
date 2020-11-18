#include "WinApp.h"
#include "DirectXCommon.h"
#include <cassert>
#include "Input.h"
#include "SafeDelete.h"
#include "Object3d.h"

#include "SceneManager.h"
#include "Title.h"
#include "GamePlay.h"
#include "Result.h"
#include "Camera.h"

#include "ModelLoder.h"
#include "Model.h"

//Windowsアプリでのエントリーポイント
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	WinApp* win = nullptr;
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;

	// ゲームウィンドウの作成
	win = new WinApp();
	win->CreateGameWindow();

	//DirectX初期化処理
	dxCommon = new DirectXCommon();
	dxCommon->Initialize(win);

#pragma region DirectX初期化処理
	// 入力の初期化
	input = new Input();
	if (!input->Initialize(win->GetInstance(), win->GetHwnd())) {
		assert(0);
		return 1;
	}
	// スプライト静的初期化
	if (!Sprite::StaticInitialize(dxCommon->GetDevice(), WinApp::window_width, WinApp::window_height)) {
		assert(0);
		return 1;
	}

	//カメラ初期化処理
	Camera::InitializeCamera(WinApp::window_width, WinApp::window_height);

	// 3Dオブジェクト静的初期化
	if (!Object3d::StaticInitialize(dxCommon->GetDevice(), WinApp::window_width, WinApp::window_height)) {
		assert(0);
		return 1;
	}


	//シーンを追加する
	SceneManager::Instance().AddScene("Title", new Title);
	SceneManager::Instance().AddScene("GamePlay", new GamePlay);
	SceneManager::Instance().AddScene("Result", new Result);

	//開始シーンを登録
	SceneManager::Instance().StartScene("Title");


	//DirectX 初期化処理 ここまで
#pragma endregion

#pragma region 描画初期化処理

#pragma endregion


	//メインループ
	while (true)
	{
		//メッセージ処理
		if (win->ProcessMessage()) { break; }

#pragma region DirextX毎フレーム処理
		//DirectX 毎フレーム処理 ここから
		input->Update();
		

		//シーン更新
		SceneManager::Instance().Update(dxCommon,input);
#pragma region 描画前




		// 3.描画コマンドここから

		// 描画開始
		dxCommon->PreDraw();


	
#pragma endregion

#pragma region 描画

		//現在のシーン描画
		SceneManager::Instance().Draw();



		// 3.描画コマンドここまで
#pragma endregion


#pragma region 描画後

	    // 描画終了
		dxCommon->PostDraw();
		//DirectX 毎フレーム処理
#pragma endregion


	}

	safe_delete(input);
	safe_delete(dxCommon);
#pragma endregion
	// ウィンドウクラスを登録解除
	// ゲームウィンドウの破棄
	win->TerminateGameWindow();
	safe_delete(win);
	return 0;
}


