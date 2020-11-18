#include "Sprite.h"
#include <cassert>
#include <d3dx12.h>
#include <d3dcompiler.h>
#include <DirectXTex.h>

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

//���L�ϐ��̎���
ID3D12Device* Sprite::device;// �f�o�C�X
UINT Sprite::descriptorHandleIncrementSize;// �f�X�N���v�^�T�C�Y
ID3D12GraphicsCommandList* Sprite::cmdList = nullptr;
ComPtr<ID3D12RootSignature> Sprite::spriteRootSignature;//���[�g�V�O�l�`��
ComPtr<ID3D12PipelineState> Sprite::spritepipelineState;//�p�C�v���C���X�e�[�g
XMMATRIX Sprite::spriteMatProjection;//�ˉe�s��
ComPtr<ID3D12DescriptorHeap>Sprite::spriteDescHeap;
ComPtr<ID3D12Resource>Sprite::spriteTexBuff[SpriteSRVCount];


bool Sprite::StaticInitialize(ID3D12Device * device, int window_width, int window_height)
{
	//nullptr�`�F�b�N
	assert(device);

	Sprite::device = device;

	// �f�X�N���v�^�T�C�Y���擾
	descriptorHandleIncrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob;//���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob;//�s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob;//�G���[�I�u�W�F�N�g

	//�X�v���C�g�ˉe�s��
	spriteMatProjection = XMMatrixOrthographicOffCenterLH(
		0.0f, window_width, window_height, 0.0f, 0.0f, 1.0f);
#pragma region �e�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	//���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"SpriteVertexShader.hlsl",//�V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//�C���N���[�h�\�ɂ���
		"VSmain", "vs_5_0",//�G���g���[�|�C���g��.�V�F�[�_�\���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//�f�o�b�O�p�ݒ�
		0,
		&vsBlob, &errorBlob);
	if (FAILED(result)) {
		//errorBlobk����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		//�G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		return false;
	}

	//�s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"SpritePixelShader.hlsl",//�V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//�C���N���[�h�\�ɂ���
		"PSmain", "ps_5_0",//�G���g���[�|�C���g�A�V�F�[�_�\���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//�f�o�b�O�p�ݒ�
		0,
		&psBlob, &errorBlob);
	if (FAILED(result)) {
		//errorBlobk����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		//�G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		return false;
	}
#pragma endregion

#pragma region ���_�V�F�[�_�ɓn�����߂̒��_�f�[�^�𐮂���
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{//xyz���W
			"POSITION",//�Z�}���e�B�b�N��
			0,//�����Z�}���e�B�b�N������������Ƃ��Ɏg���C���f�b�N�X
			DXGI_FORMAT_R32G32B32_FLOAT,//�v�f���ƃr�b�g����\��
			0,//���̓X���b�g�C���f�b�N�X
			D3D12_APPEND_ALIGNED_ELEMENT,//�f�[�^�I�t�Z�b�g�l
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,//���̓f�[�^���
			0//��x�ɕ`�悷��C���X�^���X��
		},
	{//uv���W)
		"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
	},
		//���W�ȊO�Ɂ@�F�A�e�N�X�`��UV�Ȃǂ�n���ꍇ�͂���ɑ�����
	};


#pragma endregion

#pragma region �O���t�B�b�N�X�p�C�v���C���ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
#pragma region ���_�V�F�[�_,�s�N�Z���V�F�[�_
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());
#pragma endregion

#pragma region ���X�^���C�Y�X�e�[�g
	//�T���v���}�X�N�ƃ��X�^���C�Y�X�e�[�g�̐ݒ�
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//�W���ݒ�
	//�W���I�Ȑݒ�(�w�i�J�����O�A�h��Ԃ��A�[�x�N���b�s���O�L��)
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;//�w�ʃJ�����O�����Ȃ�
#pragma endregion

#pragma region �f�v�X�X�e���V���X�e�[�g
		//06_01�ǉ�
		//�O���t�B�b�N�X�p�C�v���C���������ɐ[�x�e�X�g�̐ݒ�����Ă���
		//�f�v�X�X�e���V���X�e�[�g�̐ݒ�
		//07_02�ǉ�
		//�W���I�Ȑݒ�(�[�x�e�X�g���s���A�������݋��A�[�x����������΍��i)
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;//��ɏ㏑�����[��
#pragma endregion

#pragma region �u�����h�X�e�[�g
		//�����_�[�^�[�Q�b�g�̃u�����h�ݒ�(8���邪���܂͈�����g��Ȃ�)
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//�W���ݒ�
	blenddesc.BlendEnable = true;//�u�����h��L���ɂ���
	/*����������*/
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;//���Z
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;//�\�[�X�̃A���t�@�l
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;//1,0f-�\�[�X�̃A���t�@�n

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;//���Z
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;//�\�[�X�̒l��100%�g��
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;//�f�X�Ƃ̒l��0%�g��

	//�u�����h�X�e�[�g�ɐݒ肷��
	gpipeline.BlendState.RenderTarget[0] = blenddesc;
#pragma endregion
	// �[�x�o�b�t�@�̃t�H�[�}�b�g
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	//���_���C�A�E�g�̐ݒ�
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	//�}�`�̌`����O�p�`�ɐݒ�
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	gpipeline.NumRenderTargets = 1;//�`��Ώۂ͈��
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;//0�`255�w���RGB
	gpipeline.SampleDesc.Count = 1;//1�s�N�Z���ɂ�1��T���v�����O


	//�f�X�N���v�^�����W
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);//t0�@���W�X�^

	//���[�g�p�����[�^
	CD3DX12_ROOT_PARAMETER rootparams[2];
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);//�萔�o�b�t�@�r���[�Ƃ��ď�����(b0)
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

	//�X�^�e�B�b�N�T���v���[
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0, D3D12_FILTER_MIN_MAG_MIP_POINT); // s0 ���W�X�^
#pragma region ���[�g�V�O�l�`���̐���

	ComPtr<ID3DBlob> rootSigBlob;
	//07_02�ǉ�
	//���[�g�V�O�l�`���̐ݒ�
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	//�o�[�W������������ł̃V���A���C�Y
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	if (FAILED(result)) {
		assert(0);
		return false;
	}

	//���[�g�V�O�l�`���̐���
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&spriteRootSignature));
	if (FAILED(result)) {
		assert(0);
		return false;
	}

	//�p�C�v���C���Ƀ��[�g�V�O�l�C�`�����Z�b�g
	gpipeline.pRootSignature = spriteRootSignature.Get();//���̖��ύX

	//�p�C�v���C���X�e�[�g�̐���
	//ComPtr<ID3D12PipelineState> pipelinestate;//07_01�ǉ�
	result = device->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&spritepipelineState));//���̖��ύX
#pragma endregion

#pragma endregion

#pragma region �X�v���C�g�p�f�X�N���v�^�q�[�v�̐���
	//�f�X�N���v�^�q�[�v�𐶐�
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NumDescriptors = SpriteSRVCount;
	result = device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&spriteDescHeap));
	if (FAILED(result)) {
		assert(0);
		return false;
	}
#pragma endregion

	return true;

}

bool Sprite::SpriteLoadTexture(UINT texnumber, const wchar_t * filename)
{
	//nullptr�`�F�b�N
	assert(device);

	HRESULT result;

	//WIC�e�N�X�`���̃��[�h
	TexMetadata metadata{};
	ScratchImage scratchImg{};
	result = LoadFromWICFile(
		filename,//�uResources�v�t�H���_�́utexture.png�v
		WIC_FLAGS_NONE,
		&metadata, scratchImg);
	const Image* img = scratchImg.GetImage(0, 0, 0);//���f�[�^���o


	//���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format,
		metadata.width,
		(UINT)metadata.height,
		(UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels
	);


	//�e�N�X�`���p�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,//�e�N�X�`���p�w��
		nullptr,
		IID_PPV_ARGS(&spriteTexBuff[texnumber]));


	//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
	result = spriteTexBuff[texnumber]->WriteToSubresource(
		0,
		nullptr,//�S�̈�փR�s�[
		img->pixels,//texturedata,//���f�[�^�A�h���X
		(UINT)img->rowPitch,//1���C���T�C�Y//sizeof(XMFLOAT4)*texWidth,//1���C���T�C�Y
		(UINT)img->slicePitch//1���T�C�Y//sizeof(XMFLOAT4)*texDataCount//�S�T�C�Y
	);

	//�V�F�[�_�[���\�[�X�r���[�쐬
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};//�ݒ�\����
	D3D12_RESOURCE_DESC resDesc = spriteTexBuff[texnumber]->GetDesc();

	srvDesc.Format = resDesc.Format;//DXGI_FORMAT_R32G32B32A32_FLOAT;//RGBA
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;

	//�q�[�v��2�ԖڂɃV�F�[�_�[���\�[�X�r���[�쐬
	device->CreateShaderResourceView(spriteTexBuff[texnumber].Get(),//�r���[�Ɗ֘A�t����o�b�t�@
		&srvDesc,//�e�N�X�`���ݒ���
		CD3DX12_CPU_DESCRIPTOR_HANDLE(
			spriteDescHeap->GetCPUDescriptorHandleForHeapStart(),
			texnumber,
			descriptorHandleIncrementSize));

	return true;
}

void Sprite::PreDraw(ID3D12GraphicsCommandList * cmdList)
{
	// PreDraw��PostDraw���y�A�ŌĂ΂�Ă��Ȃ���΃G���[
	assert(Sprite::cmdList == nullptr);

	// �R�}���h���X�g���Z�b�g
	Sprite::cmdList = cmdList;
#pragma region �X�v���C�g�p�p�C�v���C���ݒ�R�}���h
	//�p�C�v���C���X�e�[�g�̐ݒ�
	cmdList->SetPipelineState(spritepipelineState.Get());
	//���[�g�V�O�l�`���̐ݒ�
	cmdList->SetGraphicsRootSignature(spriteRootSignature.Get());
	//�v���~�e�B�u�`���ݒ�
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
#pragma endregion
}

void Sprite::PostDraw()
{
	// �R�}���h���X�g������
	Sprite::cmdList = nullptr;
}

Sprite * Sprite::Create(UINT texNumber, XMFLOAT2 position, XMFLOAT4 color, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY)
{
	// ���T�C�Y
	XMFLOAT2 size = { 100.0f, 100.0f };

	if (spriteTexBuff[texNumber])
	{
		// �e�N�X�`�����擾
		D3D12_RESOURCE_DESC resDesc = spriteTexBuff[texNumber]->GetDesc();
		// �X�v���C�g�̃T�C�Y���e�N�X�`���̃T�C�Y�ɐݒ�
		size = { (float)resDesc.Width, (float)resDesc.Height };
	}

	// Sprite�̃C���X�^���X�𐶐�
	Sprite* sprite = new Sprite(texNumber, position, size, color, anchorpoint, isFlipX, isFlipY);
	if (sprite == nullptr) {
		return nullptr;
	}

	// ������
	if (!sprite->Initialize()) {
		delete sprite;
		assert(0);
		return nullptr;
	}

	return sprite;
}

Sprite::Sprite(UINT texNumber, XMFLOAT2 position,XMFLOAT2 size,XMFLOAT4 color, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY)
{
	this->spritePosition = position;
	this->spriteSize = size;
	this->anchorpoint = anchorpoint;
	this->spriteMatWorld = XMMatrixIdentity();
	this->color = color;
	this->texNumber = texNumber;
	this->isFlipX = isFlipX;
	this->isFlipY = isFlipY;
	this->texSize = size;
}

bool Sprite::Initialize()
{
	// nullptr�`�F�b�N
	assert(device);

	HRESULT result = S_FALSE;

	// ���_�o�b�t�@����
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(VertexPosUv) * 4),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&spriteVertBuff));
	if (FAILED(result)) {
		assert(0);
		return false;
	}

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	UpdateVertices();

	// ���_�o�b�t�@�r���[�̍쐬
	spriteVbView.BufferLocation = spriteVertBuff->GetGPUVirtualAddress();
	spriteVbView.SizeInBytes = sizeof(VertexPosUv) * 4;
	spriteVbView.StrideInBytes = sizeof(VertexPosUv);

	// �萔�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff)&~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&spriteConstBuff));
	if (FAILED(result)) {
		assert(0);
		return false;
	}

	// �萔�o�b�t�@�Ƀf�[�^�]��
	ConstBufferData* constMap = nullptr;
	result = spriteConstBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) {
		constMap->color = color;
		constMap->mat = spriteMatProjection;
		spriteConstBuff->Unmap(0, nullptr);
	}

	return true;
}

void Sprite::Update()
{
}

void Sprite::Draw()
{
	// ���[���h�s��̍X�V
	this->spriteMatWorld = XMMatrixIdentity();
	this->spriteMatWorld *= XMMatrixRotationZ(XMConvertToRadians(spriteRotation));
	this->spriteMatWorld *= XMMatrixTranslation(spritePosition.x, spritePosition.y, 0.0f);

	// �萔�o�b�t�@�Ƀf�[�^�]��
	ConstBufferData* constMap = nullptr;
	HRESULT result = this->spriteConstBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) {
		constMap->color = this->spriteColor;
		constMap->mat = this->spriteMatWorld * spriteMatProjection;	// �s��̍���	
		this->spriteConstBuff->Unmap(0, nullptr);
	}

	// ���_�o�b�t�@�̐ݒ�
	cmdList->IASetVertexBuffers(0, 1, &this->spriteVbView);

	ID3D12DescriptorHeap* ppHeaps[] = { spriteDescHeap.Get() };
	// �f�X�N���v�^�q�[�v���Z�b�g
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	// �萔�o�b�t�@�r���[���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, this->spriteConstBuff->GetGPUVirtualAddress());
	// �V�F�[�_���\�[�X�r���[���Z�b�g
	cmdList->SetGraphicsRootDescriptorTable(1, CD3DX12_GPU_DESCRIPTOR_HANDLE(spriteDescHeap->GetGPUDescriptorHandleForHeapStart(), this->texNumber, descriptorHandleIncrementSize));
	// �`��R�}���h
	cmdList->DrawInstanced(4, 1, 0, 0);
}

void Sprite::SetPosition(XMFLOAT2 position)
{
}

void Sprite::SetSize(XMFLOAT2 size)
{
}

void Sprite::SetAnchorPoint(XMFLOAT2 anchorpoint)
{
}

void Sprite::SetIsFlipX(bool isFlipX)
{
}

void Sprite::SetIsFlipY(bool isFlipY)
{
}

void Sprite::SetTextureRect(XMFLOAT2 texBase, XMFLOAT2 texsize)
{
}

void Sprite::UpdateVertices()
{
	HRESULT result = S_FALSE;

	// �����A����A�E���A�E��
	enum { LB, LT, RB, RT };

	float left = (0.0f - anchorpoint.x) * spriteSize.x;
	float right = (1.0f - anchorpoint.x) * spriteSize.x;
	float top = (0.0f - anchorpoint.y) * spriteSize.y;
	float bottom = (1.0f - anchorpoint.y) * spriteSize.y;
	if (isFlipX)
	{// ���E����ւ�
		left = -left;
		right = -right;
	}

	if (isFlipY)
	{// �㉺����ւ�
		top = -top;
		bottom = -bottom;
	}

	// ���_�f�[�^
	VertexPosUv vertices[4];

	vertices[LB].pos = { left,	bottom,	0.0f }; // ����
	vertices[LT].pos = { left,	top,	0.0f }; // ����
	vertices[RB].pos = { right,	bottom,	0.0f }; // �E��
	vertices[RT].pos = { right,	top,	0.0f }; // �E��

	// �e�N�X�`�����擾
	if (spriteTexBuff[texNumber])
	{
		D3D12_RESOURCE_DESC resDesc = spriteTexBuff[texNumber]->GetDesc();

		float tex_left = texBase.x / resDesc.Width;
		float tex_right = (texBase.x + texSize.x) / resDesc.Width;
		float tex_top = texBase.y / resDesc.Height;
		float tex_bottom = (texBase.y + texSize.y) / resDesc.Height;

		vertices[LB].uv = { tex_left,	tex_bottom }; // ����
		vertices[LT].uv = { tex_left,	tex_top }; // ����
		vertices[RB].uv = { tex_right,	tex_bottom }; // �E��
		vertices[RT].uv = { tex_right,	tex_top }; // �E��
	}

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	VertexPosUv* vertMap = nullptr;
	result = spriteVertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		memcpy(vertMap, vertices, sizeof(vertices));
		spriteVertBuff->Unmap(0, nullptr);
	}
}
