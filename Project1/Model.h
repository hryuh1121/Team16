#pragma once

#include <Windows.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <unordered_map>
#include<functional>
#include<DirectXTex.h>
#include <map>

enum Modeldata {//�ǂݍ��ރ��f���f�[�^���ʗp
	PMD,
	OBJ,
};

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

	//PMD�p
	//�V�F�[�_���ɓ�������}�e���A���f�[�^
	struct MaterialForHlsl {
		DirectX::XMFLOAT3 pmddiffuse; //�f�B�t���[�Y�F
		float pmdalpha; // �f�B�t���[�Y��
		DirectX::XMFLOAT3 pmdspecular; //�X�y�L�����F
		float pmdspecularity;//�X�y�L�����̋���(��Z�l)
		DirectX::XMFLOAT3 pmdambient; //�A���r�G���g�F
	};
	//����ȊO�̃}�e���A���f�[�^
	struct AdditionalMaterial {
		std::string texPath;//�e�N�X�`���t�@�C���p�X
		int toonIdx; //�g�D�[���ԍ�
		bool edgeFlg;//�}�e���A�����̗֊s���t���O
	};
	//�܂Ƃ߂�����
	struct PMDMaterial {
		unsigned int indicesNum;//�C���f�b�N�X��
		MaterialForHlsl pmdmaterial;
		AdditionalMaterial pmdadditional;
	};

	struct Transform {
		//�����Ɏ����Ă�XMMATRIX�����o��16�o�C�g�A���C�����g�ł��邽��
		//Transform��new����ۂɂ�16�o�C�g���E�Ɋm�ۂ���
		void* operator new(size_t size);
		DirectX::XMMATRIX world;
	};

	struct SceneData
	{
		XMMATRIX view;//�r���[�s��
		XMMATRIX proj;//�v���W�F�N�V�����s��
		XMMATRIX viewproj;//�v���W�F�N�V�����s��
		XMFLOAT3 eye;//���_���W
	};

private://�ÓI�����o�֐�

	bool LoadTexture(UINT texNumber,const std::string& directoryPath, const std::string& filename);

	///�}�e���A���ǂݍ���
	void LoadMaterial(UINT texNumber,const std::string& directoryPath, const std::string& filename);
	//���f������
	void CreateModel(UINT texNumber,const std::string& modelname);

	//PMD�t�@�C���ǂݍ���
	void LoadPMDFile(const char* path);
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

	///@param texpath �e�N�X�`���t�@�C���p�X
	ComPtr<ID3D12Resource> GetTextureByPath(const char* texpath);

	//�e�N�X�`��������e�N�X�`���o�b�t�@�쐬�A���g���R�s�[
	ID3D12Resource* CreateTextureFromFile(const char* texpath);

	//�e�N�X�`�����[�_�e�[�u���̍쐬
	void CreateTextureLoaderTable();


	ID3D12Resource* CreateDefaultTexture(size_t width, size_t height);
	ID3D12Resource* CreateWhiteTexture();//���e�N�X�`���̐���
	ID3D12Resource*	CreateBlackTexture();//���e�N�X�`���̐���
	ID3D12Resource*	CreateGrayGradationTexture();//�O���[�e�N�X�`���̐���

public:

	Model(UINT texNumber);

	static void StaticInitialize(ID3D12Device* device);

	//OBJ���̐���
	static Model* CreateFromOBJ(UINT texNumber,const std::string& modelname);

	//PMD���̐���
	static Model* CreateFromPMD(UINT texNumber, const char* modelname);
	// �f�X�N���v�^�q�[�v
	static ComPtr<ID3D12DescriptorHeap> descHeap;

	//�萔�o�b�t�@������
	bool Initialize();

	//�萔�o�b�t�@�X�V�p
	void Update();

	void Draw(ID3D12GraphicsCommandList* cmdList,
		ComPtr<ID3D12Resource> constBuffB0);

	//�}�e���A�����e�N�X�`���̃r���[���쐬
	HRESULT CreateMaterialAndTextureView();

	//�ǂݍ��񂾃}�e���A�������ƂɃ}�e���A���o�b�t�@���쐬
	HRESULT CreateMaterialData();

	//���W�ϊ��p�r���[�̐���
	HRESULT CreateTransformView();

	//���f���f�[�^�̎�ގ擾
	Modeldata GetModeldata() { return modeldata; }


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

	//�}�e���A���֘A
	std::vector<PMDMaterial> materials;
	ComPtr<ID3D12Resource> materialBuff = nullptr;
	std::vector<ComPtr<ID3D12Resource>> textureResources;
	std::vector<ComPtr<ID3D12Resource>> sphResources;
	std::vector<ComPtr<ID3D12Resource>> spaResources;
	std::vector<ComPtr<ID3D12Resource>> toonResources;
	ComPtr< ID3D12DescriptorHeap> materialHeap = nullptr;//�}�e���A���q�[�v(5�Ԃ�)

	Modeldata modeldata;
	ComPtr<ID3D12Resource> cameraconstBuff; // �萔�o�b�t�@

	Transform transform;
	Transform* mappedTransform = nullptr;
	ComPtr<ID3D12Resource> transformBuff = nullptr;
	ComPtr<ID3D12DescriptorHeap> transformHeap = nullptr;//���W�ϊ��q�[�v

	//���[�h�p�e�[�u��
	using LoadLambda_t = std::function<HRESULT(const std::wstring& path, DirectX::TexMetadata*, DirectX::ScratchImage&)>;
	std::map <std::string, LoadLambda_t> loadLambdaTable;
	//�e�N�X�`���e�[�u��
	std::unordered_map<std::string, ComPtr<ID3D12Resource>> _textureTable;

	//PMD�p���ʃe�N�X�`��(���A���A�O���C�X�P�[���O���f�[�V����)
	ComPtr<ID3D12Resource> whiteTex = nullptr;
	ComPtr<ID3D12Resource> blackTex = nullptr;
	ComPtr<ID3D12Resource> gradTex = nullptr;

	
	unsigned int indicesNum;//�C���f�b�N�X��
	float angle = 0;

};