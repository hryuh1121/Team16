#pragma once

#include<Windows.h>
#include <wrl.h>

#define DIRECTINPUT_VERSION 0x0800//DirextInput�̃o�[�W�����w��
#include<dinput.h>

class Input
{
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	//�����o�֐�
	Input();
	~Input();

	//������
	bool Initialize(HINSTANCE hInstance,HWND hwnd);

	//���t���[������
	void Update();

	//������Ă��邩
	bool PushKey(BYTE keyNum);

	//�����ꂽ�u�Ԃ�
	bool TriggerKey(BYTE keyNum);

	//�����o�ϐ�
private:
	//DirectInput�I�u�W�F�N�g�̐���
	ComPtr<IDirectInput8> dinput;
	//�L�[�{�[�h�f�o�C�X�̐���
	ComPtr<IDirectInputDevice8> devkeyboard;

	//�S�L�\�̓��͏�Ԃ��擾����
	BYTE key[256] = {};
	//1�t���[���O�̓��͏��
	BYTE keyPre[256] = {};

};