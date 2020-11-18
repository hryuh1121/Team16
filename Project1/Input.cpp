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

	//入力データ形式のセット
	result = devkeyboard->SetDataFormat(&c_dfDIKeyboard);//標準形式

	//排他制御レベルのリセット
	result = devkeyboard->SetCooperativeLevel(
		hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);

	return true;
}

void Input::Update()
{
	HRESULT result;

	//キーボード情報の取得開始
	result = devkeyboard->Acquire();

	//1フレーム前のキー
	memcpy(keyPre, key, sizeof(key));

	result = devkeyboard->GetDeviceState(sizeof(key), key);

}

bool Input::PushKey(BYTE keyNum)
{
	assert(0 <= keyNum && keyNum <= 256);
	if (key[keyNum]) {
		return true;
	}
	
	//押していない
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
