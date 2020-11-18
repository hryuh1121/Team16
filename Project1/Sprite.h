#pragma once


#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
using namespace DirectX;
using namespace Microsoft::WRL;


class Sprite
{

public:
	//���_�f�[�^�\����
	struct VertexPosUv
	{
		XMFLOAT3 pos;//xyz���W
		XMFLOAT2 uv;//uv���W
	};

	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		XMFLOAT4 color;//�F
		XMMATRIX mat;//3D�s��ϊ�
	};


private://���L�ϐ�
	static ID3D12Device* device;// �f�o�C�X
	static UINT descriptorHandleIncrementSize;// �f�X�N���v�^�T�C�Y
	static ComPtr<ID3D12RootSignature> spriteRootSignature;//���[�g�V�O�l�`��
	static ComPtr<ID3D12PipelineState>spritepipelineState;//�p�C�v���C���X�e�[�g
	static XMMATRIX spriteMatProjection;//�ˉe�s��
	static ID3D12GraphicsCommandList* cmdList;	// �R�}���h���X�g
	static ComPtr<ID3D12DescriptorHeap>spriteDescHeap;
	static const int SpriteSRVCount = 512;//�e�N�X�`���̍ő喇��
	static ComPtr<ID3D12Resource>spriteTexBuff[SpriteSRVCount];



public:
	//�p�C�v���C���X�e�[�g�I�u�W�F�N�g����(���L�ϐ��̏�����)
	static bool StaticInitialize(ID3D12Device* device, int window_width, int window_height);

	//�摜�ǂݍ���
	static bool SpriteLoadTexture(UINT texnumber, const wchar_t*filename);

	//�`��O����
	static void PreDraw(ID3D12GraphicsCommandList * cmdList);

	//�`��㏈��
	static void PostDraw();

	static Sprite* Create(UINT texNumber, XMFLOAT2 position, XMFLOAT4 color = { 1, 1, 1, 1 }, XMFLOAT2 anchorpoint = { 0.0f, 0.0f }, bool isFlipX = false, bool isFlipY = false);

private:
	//1�����ƂɎ��ϐ�
	ComPtr<ID3D12Resource>spriteVertBuff;//���_�o�b�t�@
	ComPtr<ID3D12Resource>spriteConstBuff;//�萔�o�b�t�@
	D3D12_VERTEX_BUFFER_VIEW spriteVbView;//���_�o�b�t�@�r���[
	float spriteRotation;//Z������̉�]�p
	XMFLOAT2 spritePosition{};//���W
	XMFLOAT2 spriteSize = { 100.0f, 100.0f };//�T�C�Y
	XMMATRIX spriteMatWorld{};//���[���h���W
	XMFLOAT4 spriteColor = { 1,1,1,1 };//�F
	UINT texNumber = 0;//�e�N�X�`���ԍ�
	XMFLOAT2 anchorpoint = { 0.0f,0.0f };
	XMFLOAT4 color = { 1, 1, 1, 1 };//�F
	bool isFlipX = false;//���E���]
	bool isFlipY = false;//�㉺���]
	XMFLOAT2 texBase = { 0,0 };
	XMFLOAT2 texSize = { 100.0f,100.0f };


public:
	//�R���X�g���N�^
	Sprite(UINT texNumber, XMFLOAT2 position, XMFLOAT2 size, XMFLOAT4 color, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY);

	//������
	bool Initialize();

	//�X�V
	void Update();

	//�`��
	void Draw();

	//���W�ݒ�
	void SetPosition(XMFLOAT2 position);

	//�T�C�Y�ݒ�
	void SetSize(XMFLOAT2 size);

	//�A���J�[�|�C���g
	void SetAnchorPoint(XMFLOAT2 anchorpoint);

	//���E���]
	void SetIsFlipX(bool isFlipX);

	//�㉺���]
	void SetIsFlipY(bool isFlipY);

	//�e�N�X�`���͈͐ݒ�
	void SetTextureRect(XMFLOAT2 texBase, XMFLOAT2 texsize);

private: // �����o�֐�

// ���_�f�[�^�]��
	void UpdateVertices();

};