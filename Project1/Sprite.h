#pragma once


#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
using namespace DirectX;
using namespace Microsoft::WRL;


class Sprite
{

public:
	//頂点データ構造体
	struct VertexPosUv
	{
		XMFLOAT3 pos;//xyz座標
		XMFLOAT2 uv;//uv座標
	};

	//定数バッファ用データ構造体
	struct ConstBufferData
	{
		XMFLOAT4 color;//色
		XMMATRIX mat;//3D行列変換
	};


private://共有変数
	static ID3D12Device* device;// デバイス
	static UINT descriptorHandleIncrementSize;// デスクリプタサイズ
	static ComPtr<ID3D12RootSignature> spriteRootSignature;//ルートシグネチャ
	static ComPtr<ID3D12PipelineState>spritepipelineState;//パイプラインステート
	static XMMATRIX spriteMatProjection;//射影行列
	static ID3D12GraphicsCommandList* cmdList;	// コマンドリスト
	static ComPtr<ID3D12DescriptorHeap>spriteDescHeap;
	static const int SpriteSRVCount = 512;//テクスチャの最大枚数
	static ComPtr<ID3D12Resource>spriteTexBuff[SpriteSRVCount];



public:
	//パイプラインステートオブジェクト生成(共有変数の初期化)
	static bool StaticInitialize(ID3D12Device* device, int window_width, int window_height);

	//画像読み込み
	static bool SpriteLoadTexture(UINT texnumber, const wchar_t*filename);

	//描画前処理
	static void PreDraw(ID3D12GraphicsCommandList * cmdList);

	//描画後処理
	static void PostDraw();

	static Sprite* Create(UINT texNumber, XMFLOAT2 position, XMFLOAT4 color = { 1, 1, 1, 1 }, XMFLOAT2 anchorpoint = { 0.0f, 0.0f }, bool isFlipX = false, bool isFlipY = false);

private:
	//1枚ごとに持つ変数
	ComPtr<ID3D12Resource>spriteVertBuff;//頂点バッファ
	ComPtr<ID3D12Resource>spriteConstBuff;//定数バッファ
	D3D12_VERTEX_BUFFER_VIEW spriteVbView;//頂点バッファビュー
	float spriteRotation;//Z軸周りの回転角
	XMFLOAT2 spritePosition{};//座標
	XMFLOAT2 spriteSize = { 100.0f, 100.0f };//サイズ
	XMMATRIX spriteMatWorld{};//ワールド座標
	XMFLOAT4 spriteColor = { 1,1,1,1 };//色
	UINT texNumber = 0;//テクスチャ番号
	XMFLOAT2 anchorpoint = { 0.0f,0.0f };
	XMFLOAT4 color = { 1, 1, 1, 1 };//色
	bool isFlipX = false;//左右反転
	bool isFlipY = false;//上下反転
	XMFLOAT2 texBase = { 0,0 };
	XMFLOAT2 texSize = { 100.0f,100.0f };


public:
	//コンストラクタ
	Sprite(UINT texNumber, XMFLOAT2 position, XMFLOAT2 size, XMFLOAT4 color, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY);

	//初期化
	bool Initialize();

	//更新
	void Update();

	//描画
	void Draw();

	//座標設定
	void SetPosition(XMFLOAT2 position);

	//サイズ設定
	void SetSize(XMFLOAT2 size);

	//アンカーポイント
	void SetAnchorPoint(XMFLOAT2 anchorpoint);

	//左右反転
	void SetIsFlipX(bool isFlipX);

	//上下反転
	void SetIsFlipY(bool isFlipY);

	//テクスチャ範囲設定
	void SetTextureRect(XMFLOAT2 texBase, XMFLOAT2 texsize);

private: // メンバ関数

// 頂点データ転送
	void UpdateVertices();

};