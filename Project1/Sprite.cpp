#include "Sprite.h"
#include <cassert>
#include <d3dx12.h>
#include <d3dcompiler.h>
#include <DirectXTex.h>

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

//共有変数の実体
ID3D12Device* Sprite::device;// デバイス
UINT Sprite::descriptorHandleIncrementSize;// デスクリプタサイズ
ID3D12GraphicsCommandList* Sprite::cmdList = nullptr;
ComPtr<ID3D12RootSignature> Sprite::spriteRootSignature;//ルートシグネチャ
ComPtr<ID3D12PipelineState> Sprite::spritepipelineState;//パイプラインステート
XMMATRIX Sprite::spriteMatProjection;//射影行列
ComPtr<ID3D12DescriptorHeap>Sprite::spriteDescHeap;
ComPtr<ID3D12Resource>Sprite::spriteTexBuff[SpriteSRVCount];


bool Sprite::StaticInitialize(ID3D12Device * device, int window_width, int window_height)
{
	//nullptrチェック
	assert(device);

	Sprite::device = device;

	// デスクリプタサイズを取得
	descriptorHandleIncrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob;//頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob;//ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob;//エラーオブジェクト

	//スプライト射影行列
	spriteMatProjection = XMMatrixOrthographicOffCenterLH(
		0.0f, window_width, window_height, 0.0f, 0.0f, 1.0f);
#pragma region 各シェーダの読み込みとコンパイル
	//頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"SpriteVertexShader.hlsl",//シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//インクルード可能にする
		"VSmain", "vs_5_0",//エントリーポイント名.シェーダ―モデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//デバッグ用設定
		0,
		&vsBlob, &errorBlob);
	if (FAILED(result)) {
		//errorBlobkからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		//エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		return false;
	}

	//ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"SpritePixelShader.hlsl",//シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//インクルード可能にする
		"PSmain", "ps_5_0",//エントリーポイント、シェーダ―モデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//デバッグ用設定
		0,
		&psBlob, &errorBlob);
	if (FAILED(result)) {
		//errorBlobkからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		//エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		return false;
	}
#pragma endregion

#pragma region 頂点シェーダに渡すための頂点データを整える
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{//xyz座標
			"POSITION",//セマンティック名
			0,//同じセマンティック名が複数あるときに使うインデックス
			DXGI_FORMAT_R32G32B32_FLOAT,//要素数とビット数を表す
			0,//入力スロットインデックス
			D3D12_APPEND_ALIGNED_ELEMENT,//データオフセット値
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,//入力データ種別
			0//一度に描画するインスタンス数
		},
	{//uv座標)
		"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
	},
		//座標以外に　色、テクスチャUVなどを渡す場合はさらに続ける
	};


#pragma endregion

#pragma region グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
#pragma region 頂点シェーダ,ピクセルシェーダ
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());
#pragma endregion

#pragma region ラスタライズステート
	//サンプルマスクとラスタライズステートの設定
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//標準設定
	//標準的な設定(背景カリング、塗りつぶし、深度クリッピング有効)
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;//背面カリングをしない
#pragma endregion

#pragma region デプスステンシルステート
		//06_01追加
		//グラフィックスパイプライン生成時に深度テストの設定をしておく
		//デプスステンシルステートの設定
		//07_02追加
		//標準的な設定(深度テストを行う、書き込み許可、深度が小さければ合格)
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;//常に上書きルール
#pragma endregion

#pragma region ブレンドステート
		//レンダーターゲットのブレンド設定(8個あるがいまは一つしか使わない)
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//標準設定
	blenddesc.BlendEnable = true;//ブレンドを有効にする
	/*半透明合成*/
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;//加算
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;//ソースのアルファ値
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;//1,0f-ソースのアルファ地

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;//加算
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;//ソースの値を100%使う
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;//デスとの値を0%使う

	//ブレンドステートに設定する
	gpipeline.BlendState.RenderTarget[0] = blenddesc;
#pragma endregion
	// 深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	//頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	//図形の形状を三角形に設定
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	gpipeline.NumRenderTargets = 1;//描画対象は一つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;//0～255指定のRGB
	gpipeline.SampleDesc.Count = 1;//1ピクセルにつき1回サンプリング


	//デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);//t0　レジスタ

	//ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootparams[2];
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);//定数バッファビューとして初期化(b0)
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

	//スタティックサンプラー
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0, D3D12_FILTER_MIN_MAG_MIP_POINT); // s0 レジスタ
#pragma region ルートシグネチャの生成

	ComPtr<ID3DBlob> rootSigBlob;
	//07_02追加
	//ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	//バージョン自動判定でのシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	if (FAILED(result)) {
		assert(0);
		return false;
	}

	//ルートシグネチャの生成
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&spriteRootSignature));
	if (FAILED(result)) {
		assert(0);
		return false;
	}

	//パイプラインにルートシグネイチャをセット
	gpipeline.pRootSignature = spriteRootSignature.Get();//物体毎変更

	//パイプラインステートの生成
	//ComPtr<ID3D12PipelineState> pipelinestate;//07_01追加
	result = device->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&spritepipelineState));//物体毎変更
#pragma endregion

#pragma endregion

#pragma region スプライト用デスクリプタヒープの生成
	//デスクリプタヒープを生成
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
	//nullptrチェック
	assert(device);

	HRESULT result;

	//WICテクスチャのロード
	TexMetadata metadata{};
	ScratchImage scratchImg{};
	result = LoadFromWICFile(
		filename,//「Resources」フォルダの「texture.png」
		WIC_FLAGS_NONE,
		&metadata, scratchImg);
	const Image* img = scratchImg.GetImage(0, 0, 0);//生データ抽出


	//リソース設定
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format,
		metadata.width,
		(UINT)metadata.height,
		(UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels
	);


	//テクスチャ用バッファの生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,//テクスチャ用指定
		nullptr,
		IID_PPV_ARGS(&spriteTexBuff[texnumber]));


	//テクスチャバッファにデータ転送
	result = spriteTexBuff[texnumber]->WriteToSubresource(
		0,
		nullptr,//全領域へコピー
		img->pixels,//texturedata,//元データアドレス
		(UINT)img->rowPitch,//1ラインサイズ//sizeof(XMFLOAT4)*texWidth,//1ラインサイズ
		(UINT)img->slicePitch//1枚サイズ//sizeof(XMFLOAT4)*texDataCount//全サイズ
	);

	//シェーダーリソースビュー作成
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};//設定構造体
	D3D12_RESOURCE_DESC resDesc = spriteTexBuff[texnumber]->GetDesc();

	srvDesc.Format = resDesc.Format;//DXGI_FORMAT_R32G32B32A32_FLOAT;//RGBA
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;

	//ヒープの2番目にシェーダーリソースビュー作成
	device->CreateShaderResourceView(spriteTexBuff[texnumber].Get(),//ビューと関連付けるバッファ
		&srvDesc,//テクスチャ設定情報
		CD3DX12_CPU_DESCRIPTOR_HANDLE(
			spriteDescHeap->GetCPUDescriptorHandleForHeapStart(),
			texnumber,
			descriptorHandleIncrementSize));

	return true;
}

void Sprite::PreDraw(ID3D12GraphicsCommandList * cmdList)
{
	// PreDrawとPostDrawがペアで呼ばれていなければエラー
	assert(Sprite::cmdList == nullptr);

	// コマンドリストをセット
	Sprite::cmdList = cmdList;
#pragma region スプライト用パイプライン設定コマンド
	//パイプラインステートの設定
	cmdList->SetPipelineState(spritepipelineState.Get());
	//ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(spriteRootSignature.Get());
	//プリミティブ形状を設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
#pragma endregion
}

void Sprite::PostDraw()
{
	// コマンドリストを解除
	Sprite::cmdList = nullptr;
}

Sprite * Sprite::Create(UINT texNumber, XMFLOAT2 position, XMFLOAT4 color, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY)
{
	// 仮サイズ
	XMFLOAT2 size = { 100.0f, 100.0f };

	if (spriteTexBuff[texNumber])
	{
		// テクスチャ情報取得
		D3D12_RESOURCE_DESC resDesc = spriteTexBuff[texNumber]->GetDesc();
		// スプライトのサイズをテクスチャのサイズに設定
		size = { (float)resDesc.Width, (float)resDesc.Height };
	}

	// Spriteのインスタンスを生成
	Sprite* sprite = new Sprite(texNumber, position, size, color, anchorpoint, isFlipX, isFlipY);
	if (sprite == nullptr) {
		return nullptr;
	}

	// 初期化
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
	// nullptrチェック
	assert(device);

	HRESULT result = S_FALSE;

	// 頂点バッファ生成
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

	// 頂点バッファへのデータ転送
	UpdateVertices();

	// 頂点バッファビューの作成
	spriteVbView.BufferLocation = spriteVertBuff->GetGPUVirtualAddress();
	spriteVbView.SizeInBytes = sizeof(VertexPosUv) * 4;
	spriteVbView.StrideInBytes = sizeof(VertexPosUv);

	// 定数バッファの生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff)&~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&spriteConstBuff));
	if (FAILED(result)) {
		assert(0);
		return false;
	}

	// 定数バッファにデータ転送
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
	// ワールド行列の更新
	this->spriteMatWorld = XMMatrixIdentity();
	this->spriteMatWorld *= XMMatrixRotationZ(XMConvertToRadians(spriteRotation));
	this->spriteMatWorld *= XMMatrixTranslation(spritePosition.x, spritePosition.y, 0.0f);

	// 定数バッファにデータ転送
	ConstBufferData* constMap = nullptr;
	HRESULT result = this->spriteConstBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) {
		constMap->color = this->spriteColor;
		constMap->mat = this->spriteMatWorld * spriteMatProjection;	// 行列の合成	
		this->spriteConstBuff->Unmap(0, nullptr);
	}

	// 頂点バッファの設定
	cmdList->IASetVertexBuffers(0, 1, &this->spriteVbView);

	ID3D12DescriptorHeap* ppHeaps[] = { spriteDescHeap.Get() };
	// デスクリプタヒープをセット
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	// 定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(0, this->spriteConstBuff->GetGPUVirtualAddress());
	// シェーダリソースビューをセット
	cmdList->SetGraphicsRootDescriptorTable(1, CD3DX12_GPU_DESCRIPTOR_HANDLE(spriteDescHeap->GetGPUDescriptorHandleForHeapStart(), this->texNumber, descriptorHandleIncrementSize));
	// 描画コマンド
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

	// 左下、左上、右下、右上
	enum { LB, LT, RB, RT };

	float left = (0.0f - anchorpoint.x) * spriteSize.x;
	float right = (1.0f - anchorpoint.x) * spriteSize.x;
	float top = (0.0f - anchorpoint.y) * spriteSize.y;
	float bottom = (1.0f - anchorpoint.y) * spriteSize.y;
	if (isFlipX)
	{// 左右入れ替え
		left = -left;
		right = -right;
	}

	if (isFlipY)
	{// 上下入れ替え
		top = -top;
		bottom = -bottom;
	}

	// 頂点データ
	VertexPosUv vertices[4];

	vertices[LB].pos = { left,	bottom,	0.0f }; // 左下
	vertices[LT].pos = { left,	top,	0.0f }; // 左上
	vertices[RB].pos = { right,	bottom,	0.0f }; // 右下
	vertices[RT].pos = { right,	top,	0.0f }; // 右上

	// テクスチャ情報取得
	if (spriteTexBuff[texNumber])
	{
		D3D12_RESOURCE_DESC resDesc = spriteTexBuff[texNumber]->GetDesc();

		float tex_left = texBase.x / resDesc.Width;
		float tex_right = (texBase.x + texSize.x) / resDesc.Width;
		float tex_top = texBase.y / resDesc.Height;
		float tex_bottom = (texBase.y + texSize.y) / resDesc.Height;

		vertices[LB].uv = { tex_left,	tex_bottom }; // 左下
		vertices[LT].uv = { tex_left,	tex_top }; // 左上
		vertices[RB].uv = { tex_right,	tex_bottom }; // 右下
		vertices[RT].uv = { tex_right,	tex_top }; // 右上
	}

	// 頂点バッファへのデータ転送
	VertexPosUv* vertMap = nullptr;
	result = spriteVertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		memcpy(vertMap, vertices, sizeof(vertices));
		spriteVertBuff->Unmap(0, nullptr);
	}
}
