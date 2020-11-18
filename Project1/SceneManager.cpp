#include "SceneManager.h"
#include <memory>

//デストラクタ
SceneManager::~SceneManager()
{
	for (auto scene : _scenes) {
		delete scene.second;
	}
}

//クラスのインスタンスを取得
SceneManager& SceneManager::Instance()
{
	static std::unique_ptr<SceneManager> instance(new SceneManager);

	return *instance;
}

void SceneManager::AddScene(std::string sceneName, Scene * scene)
{
	if (_scenes.count(sceneName))
		return;
	_scenes[sceneName] = scene;
}

//開始シーンを設定
void SceneManager::StartScene(std::string sceneName)
{
	if (!_scenes.count(sceneName))
		return;

	_currentScene = _scenes[sceneName];
}


//シーン遷移
void SceneManager::LoadScene(std::string sceneName)
{
	if (!_scenes.count(sceneName))
		return;

	_nextScene = _scenes[sceneName];
	_state = SceneState::Release;

}

void SceneManager::Update(DirectXCommon* dxCommon, Input* input)
{
	//シーンを更新する
	SceneUpdate(dxCommon,input);

}

void SceneManager::Draw()
{
	if (_state == SceneState::Update) {
		_currentScene->Draw();
	}
}

//シーンの更新処置
void SceneManager::SceneUpdate(DirectXCommon* dxCommon,Input* input)
{
	if (!_currentScene)
		return;

	//初期化
	if (_state == SceneState::Initialize) {
		_currentScene->Initialize(dxCommon,input);
		_state = SceneState::Update;
	}

	//更新処理
	_currentScene->Update();

	//解放処理
	if (_state == SceneState::Release) {
		_currentScene->Release();
		NextScene();
		_state = SceneState::Initialize;
	}
}

//次のシーンに切り替え
void SceneManager::NextScene()
{
	_currentScene = _nextScene;
}
