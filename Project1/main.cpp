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

//Windows�A�v���ł̃G���g���[�|�C���g
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	WinApp* win = nullptr;
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;

	// �Q�[���E�B���h�E�̍쐬
	win = new WinApp();
	win->CreateGameWindow();

	//DirectX����������
	dxCommon = new DirectXCommon();
	dxCommon->Initialize(win);

#pragma region DirectX����������
	// ���͂̏�����
	input = new Input();
	if (!input->Initialize(win->GetInstance(), win->GetHwnd())) {
		assert(0);
		return 1;
	}
	// �X�v���C�g�ÓI������
	if (!Sprite::StaticInitialize(dxCommon->GetDevice(), WinApp::window_width, WinApp::window_height)) {
		assert(0);
		return 1;
	}

	//�J��������������
	Camera::InitializeCamera(WinApp::window_width, WinApp::window_height);

	// 3D�I�u�W�F�N�g�ÓI������
	if (!Object3d::StaticInitialize(dxCommon->GetDevice(), WinApp::window_width, WinApp::window_height)) {
		assert(0);
		return 1;
	}


	//�V�[����ǉ�����
	SceneManager::Instance().AddScene("Title", new Title);
	SceneManager::Instance().AddScene("GamePlay", new GamePlay);
	SceneManager::Instance().AddScene("Result", new Result);

	//�J�n�V�[����o�^
	SceneManager::Instance().StartScene("Title");


	//DirectX ���������� �����܂�
#pragma endregion

#pragma region �`�揉��������

#pragma endregion


	//���C�����[�v
	while (true)
	{
		//���b�Z�[�W����
		if (win->ProcessMessage()) { break; }

#pragma region DirextX���t���[������
		//DirectX ���t���[������ ��������
		input->Update();
		

		//�V�[���X�V
		SceneManager::Instance().Update(dxCommon,input);
#pragma region �`��O




		// 3.�`��R�}���h��������

		// �`��J�n
		dxCommon->PreDraw();


	
#pragma endregion

#pragma region �`��

		//���݂̃V�[���`��
		SceneManager::Instance().Draw();



		// 3.�`��R�}���h�����܂�
#pragma endregion


#pragma region �`���

	    // �`��I��
		dxCommon->PostDraw();
		//DirectX ���t���[������
#pragma endregion


	}

	safe_delete(input);
	safe_delete(dxCommon);
#pragma endregion
	// �E�B���h�E�N���X��o�^����
	// �Q�[���E�B���h�E�̔j��
	win->TerminateGameWindow();
	safe_delete(win);
	return 0;
}


