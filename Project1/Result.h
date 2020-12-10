#pragma once
#include "Scene.h"

class Result :public Scene
{
public:
	void Initialize(DirectXCommon* dxCommon, Input* input,Audio* audio);

	void Update();

	void Draw();

	void Release();

private:
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;

	bool isEnd = false;
};