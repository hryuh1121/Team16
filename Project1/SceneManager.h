#pragma once
#include "Scene.h"
#include <unordered_map>
#include "DirectXCommon.h"

enum SceneState
{
	Initialize,//初期化
	Update,//更新
	Release//解放
};

//シーン管理クラス
class SceneManager
{
private:
	SceneManager() :_state(SceneState::Initialize),
		_currentScene(nullptr), _nextScene(nullptr) {}
	SceneManager(const SceneManager&) {}
	SceneManager& operator=(const SceneManager&) {}




public:
	//デストラクタ
	~SceneManager();

	//インスタンスを取得
	static SceneManager& Instance();

	//シーンを追加する
	void AddScene(std::string sceneName, Scene* scene);

	//開始シーンを設定
	void StartScene(std::string sceneName);

	//シーン遷移
	void LoadScene(std::string sceneName);

	//更新処理
	void Update(DirectXCommon* dxCommon, Input* input);

	//描画処理
	void Draw();


private:
	//シーンの更新処理
	void SceneUpdate(DirectXCommon* dxCommon,Input* input);

	//次のシーンがあるか
	void NextScene();


private:
	DirectXCommon* dxCommon = nullptr;
	Scene* _currentScene;//現在のシーン
	Scene* _nextScene;//次のシーン
	SceneState _state;//シーンの状態
	std::unordered_map<std::string, Scene*> _scenes;

};