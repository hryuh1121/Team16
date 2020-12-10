#pragma once
#include "Input.h"
#include "DirectXCommon.h"
#include "Audio.h"

//�V�[�����N���X
class Scene
{
public:
	//������
	virtual void Initialize(DirectXCommon* dxCommon, Input* input,Audio* audio) = 0;

	//�X�V
	virtual void Update() = 0;

	//�`��
	virtual void Draw() = 0;


	//���
	virtual void Release() = 0;


};