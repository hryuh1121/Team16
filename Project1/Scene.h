#pragma once
#include "Input.h"
#include "DirectXCommon.h"

//シーン基底クラス
class Scene
{
public:
	//初期化
	virtual void Initialize(DirectXCommon* dxCommon, Input* input) = 0;

	//更新
	virtual void Update() = 0;

	//描画
	virtual void Draw() = 0;


	//解放
	virtual void Release() = 0;


};