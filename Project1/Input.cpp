#include "Input.h"
#include <cassert>

#pragma comment(lib, "dinput8.lib")

Input::Input()
{
}

Input::~Input()
{
}

bool Input::Initialize(HINSTANCE hInstance, HWND hwnd)
{
	HRESULT result;

	result = DirectInput8Create(
		hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr);

	result = dinput->CreateDevice(GUID_SysKeyboard, &devkeyboard, NULL);

	//���̓f�[�^�`���̃Z�b�g
	result = devkeyboard->SetDataFormat(&c_dfDIKeyboard);//�W���`��

	//�r�����䃌�x���̃��Z�b�g
	result = devkeyboard->SetCooperativeLevel(
		hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);

	return true;
}

void Input::Update()
{
	HRESULT result;

	//�L�[�{�[�h���̎擾�J�n
	result = devkeyboard->Acquire();

	//1�t���[���O�̃L�[
	memcpy(keyPre, key, sizeof(key));

	result = devkeyboard->GetDeviceState(sizeof(key), key);

}

bool Input::PushKey(BYTE keyNum)
{
	assert(0 <= keyNum && keyNum <= 256);
	if (key[keyNum]) {
		return true;
	}
	
	//�����Ă��Ȃ�
	return false;
}

bool Input::TriggerKey(BYTE keyNum)
{
	assert(0 <= keyNum && keyNum <= 256);
	if (!keyPre[keyNum] && key[keyNum]) {
		return true;
	}

	return false;
}
