#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
//#include <d3dx12.h>
#include "Model.h"


//3D�I�u�W�F�N�g���L�ϐ��p�N���X
class Object3d
{
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public: // �T�u�N���X
	// �萔�o�b�t�@�p�f�[�^�\����B0
	struct ConstBufferDataB0
	{
		//XMFLOAT4 color;	// �F (RGBA)
		XMMATRIX mat;	// �R�c�ϊ��s��
	};

public:
	//�p�C�v���C���X�e�[�g�I�u�W�F�N�g�����֐�
	static bool StaticInitialize(ID3D12Device* device, int window_width, int window_height);

	//�p�C�v���C���ݒ�R�}���h
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	//3D�I�u�W�F�N�g����
	static Object3d* Create(XMFLOAT3 position);
	

	//�`��㏈��
	static void PostDraw();

private://�ÓI�����o�ϐ�
	// �萔�o�b�t�@�̍ő吔
	static const int constantBufferNum = 512;
	//�f�o�C�X
	static ID3D12Device* device;
	//// �f�X�N���v�^�T�C�Y
	//static UINT descriptorHandleIncrementSize;
	// �R�}���h���X�g
	static ID3D12GraphicsCommandList* cmdList;
	// ���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> rootsignature;
	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
    ComPtr<ID3D12PipelineState> pipelinestate;

	
private://�ÓI�����o�֐�
	//�f�X�N���v�^�q�[�v�̏�����
	static bool InitializeDescriptorHeap();
	

public:
	bool InitializeGraphicsPipeline();

	bool InitializeGraphicsPipeline(const LPCWSTR& vertexShadername, const LPCSTR& vsmain,
		const LPCWSTR& pixelShadername, const LPCSTR& psmain);

	Object3d(XMFLOAT3 position);

	void SetModel(Model* model);

	void SetShader(const LPCWSTR& vertexShadername, const LPCSTR& vsmain,
		const LPCWSTR& pixelShadername, const LPCSTR& psmain);

	bool Initialize();

	//���t���[������
	void Update();

	void Draw();


	//���W�擾
	const XMFLOAT3& GetPosition() { return position; }
	//���W�Z�b�g
	void SetPosition(XMFLOAT3 position) { this->position = position; }
	//�X�P�[���擾
	const XMFLOAT3& GetScale() { return position; }
	//�X�P�[���Z�b�g
	void SetScale(XMFLOAT3 scale) { this->scale = scale; }

private:

	Model* model = nullptr;
	ComPtr<ID3D12Resource> constBuffB0; // �萔�o�b�t�@
	XMFLOAT4 color = { 1,1,1,1 };//�F
	XMFLOAT3 scale = { 1,1,1 };//���[�J���X�P�[��
	XMFLOAT3 rotation = { 0,0,0 };//X,Y,Z������̃��[�J����]�p
	XMFLOAT3 position = { 0,0,0 };//���[�J�����W
	XMMATRIX matWorld;//���[�J�����[���h�ϊ��s��
	XMMATRIX matScale;
	XMMATRIX matRot;
	XMMATRIX matTrans;

};