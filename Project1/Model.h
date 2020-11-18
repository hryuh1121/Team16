#pragma once

#include <Windows.h>
#include <d3dx12.h>
#include <DirectXMath.h>

class Model {
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public: // �T�u�N���X
	//���_�f�[�^�\����
	struct VertexPosNormalUv
	{
		XMFLOAT3 pos; // xyz���W
		XMFLOAT3 normal; // �@���x�N�g��
		XMFLOAT2 uv;  // uv���W
	};
	// �萔�o�b�t�@�p�f�[�^�\����B1
	struct ConstBufferDataB1
	{
		XMFLOAT3 ambient;//�A���r�G���g�W��
		float pad1;//�p�f�B���O
		XMFLOAT3 diffuse;//�f�B�t���[�Y�W��
		float pad2;//�p�f�B���O
		XMFLOAT3 specular;//�X�y�L�����[�W��
		float alpha;//�A���t�@
	};
	//�}�e���A��
	struct Material
	{
		std::string name;//�}�e���A����
		XMFLOAT3 ambient;//�A���r�G���g�e���x
		XMFLOAT3 diffuse;//�f�B�t���[�Y�e���x
		XMFLOAT3 specular;//�X�y�L�����[�e���x
		float alpha;//�A���t�@
		std::string textureFilename;//�e�N�X�`���t�@�C����
		//�R���X�g���N�^
		Material() {
			ambient = { 0.3f,0.3f,0.3f };
			diffuse = { 0.0f,0.0f,0.0f };
			specular = { 0.0f,0.0f,0.0f };
			alpha = 1.0f;
		}
	};

private://�ÓI�����o�֐�

	bool LoadTexture(UINT texNumber,const std::string& directoryPath, const std::string& filename);

	///�}�e���A���ǂݍ���
	void LoadMaterial(UINT texNumber,const std::string& directoryPath, const std::string& filename);
	//���f������
	void CreateModel(UINT texNumber,const std::string& modelname);
private:
	
	//�f�o�C�X
	static ID3D12Device* device;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	static CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	static CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	// �f�X�N���v�^�T�C�Y
	static UINT descriptorHandleIncrementSize;
	//�}�e���A��
	static Material material;


public:

	Model(UINT texNumber);

	static void StaticInitialize(ID3D12Device* device);

	static Model* CreateFromOBJ(UINT texNumber,const std::string& modelname);

	// �f�X�N���v�^�q�[�v
	static ComPtr<ID3D12DescriptorHeap> descHeap;

	//�萔�o�b�t�@������
	bool Initialize();

	//�萔�o�b�t�@�X�V�p
	void Update();

	void Draw(ID3D12GraphicsCommandList* cmdList,
		ComPtr<ID3D12Resource> constBuffB0);




private:
	// �e�N�X�`���o�b�t�@
	static const int SRVCount = 512;//���f���e�N�X�`���̍ő喇��
	ComPtr<ID3D12Resource> constBuffB1; // �萔�o�b�t�@
	ComPtr<ID3D12Resource> texbuff[SRVCount];
	ComPtr<ID3D12Resource> vertBuff;//���_�o�b�t�@
	ComPtr<ID3D12Resource> indexBuff;//�C���f�b�N�X�o�b�t�@
		// ���_�f�[�^�z��
	std::vector<VertexPosNormalUv> modelvertices;
	// ���_�C���f�b�N�X�z��
	std::vector<unsigned short> modelindices;
	D3D12_VERTEX_BUFFER_VIEW vbView;
	D3D12_INDEX_BUFFER_VIEW ibView;

	UINT texnumber = 0;

};