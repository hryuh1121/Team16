#pragma once
#include "Scene.h"
#include <unordered_map>
#include "DirectXCommon.h"

enum SceneState
{
	Initialize,//������
	Update,//�X�V
	Release//���
};

//�V�[���Ǘ��N���X
class SceneManager
{
private:
	SceneManager() :_state(SceneState::Initialize),
		_currentScene(nullptr), _nextScene(nullptr) {}
	SceneManager(const SceneManager&) {}
	SceneManager& operator=(const SceneManager&) {}




public:
	//�f�X�g���N�^
	~SceneManager();

	//�C���X�^���X���擾
	static SceneManager& Instance();

	//�V�[����ǉ�����
	void AddScene(std::string sceneName, Scene* scene);

	//�J�n�V�[����ݒ�
	void StartScene(std::string sceneName);

	//�V�[���J��
	void LoadScene(std::string sceneName);

	//�X�V����
	void Update(DirectXCommon* dxCommon, Input* input);

	//�`�揈��
	void Draw();


private:
	//�V�[���̍X�V����
	void SceneUpdate(DirectXCommon* dxCommon,Input* input);

	//���̃V�[�������邩
	void NextScene();


private:
	DirectXCommon* dxCommon = nullptr;
	Scene* _currentScene;//���݂̃V�[��
	Scene* _nextScene;//���̃V�[��
	SceneState _state;//�V�[���̏��
	std::unordered_map<std::string, Scene*> _scenes;

};