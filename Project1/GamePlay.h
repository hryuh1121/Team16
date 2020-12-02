#pragma once

#include "Scene.h"
#include "Sprite.h"
#include "Object3d.h"
#include "Player.h"
#include "Model.h"
#include"Enemy.h"
#include"Bullet.h"

class GamePlay :public Scene
{

public:
	void Initialize(DirectXCommon* dxCommon, Input* input);

	void Update();

	void Draw();

	void Release();

private:
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;

	Player* player = nullptr;
	Enemy* enemy = nullptr;

	Object3d* skydome = nullptr;
	Object3d* triangle = nullptr;


	Model* skyModel = nullptr;
	Model* triangleModel = nullptr;
	Model* ground = nullptr;

	bool isEnd = false;
};