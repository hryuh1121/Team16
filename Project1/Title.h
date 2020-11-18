#pragma once

#include "Scene.h"
#include "Sprite.h"
#include "DirectXCommon.h"

class Title :public Scene
{
public:
	void Initialize(DirectXCommon* dxCommon, Input* input);

	void Update();

	void Draw();

	void Release();

private:
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	Sprite* title = nullptr;

	bool isEnd = false;
};