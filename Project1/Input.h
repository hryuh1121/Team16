#pragma once

#include<Windows.h>
#include <wrl.h>

#define DIRECTINPUT_VERSION 0x0800//DirextInputのバージョン指定
#include<dinput.h>

class Input
{
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	//メンバ関数
	Input();
	~Input();

	//初期化
	bool Initialize(HINSTANCE hInstance,HWND hwnd);

	//毎フレーム処理
	void Update();

	//押されているか
	bool PushKey(BYTE keyNum);

	//押された瞬間か
	bool TriggerKey(BYTE keyNum);

	//メンバ変数
private:
	//DirectInputオブジェクトの生成
	ComPtr<IDirectInput8> dinput;
	//キーボードデバイスの生成
	ComPtr<IDirectInputDevice8> devkeyboard;

	//全キ―の入力状態を取得する
	BYTE key[256] = {};
	//1フレーム前の入力状態
	BYTE keyPre[256] = {};

};