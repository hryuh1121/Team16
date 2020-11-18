#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
//#include <d3dx12.h>
#include "Model.h"


//3Dオブジェクト共有変数用クラス
class Object3d
{
private: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public: // サブクラス
	// 定数バッファ用データ構造体B0
	struct ConstBufferDataB0
	{
		//XMFLOAT4 color;	// 色 (RGBA)
		XMMATRIX mat;	// ３Ｄ変換行列
	};

public:
	//パイプラインステートオブジェクト生成関数
	static bool StaticInitialize(ID3D12Device* device, int window_width, int window_height);

	//パイプライン設定コマンド
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	//3Dオブジェクト生成
	static Object3d* Create(XMFLOAT3 position);
	

	//描画後処理
	static void PostDraw();

private://静的メンバ変数
	// 定数バッファの最大数
	static const int constantBufferNum = 512;
	//デバイス
	static ID3D12Device* device;
	//// デスクリプタサイズ
	//static UINT descriptorHandleIncrementSize;
	// コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootsignature;
	// パイプラインステートオブジェクト
    ComPtr<ID3D12PipelineState> pipelinestate;

	
private://静的メンバ関数
	//デスクリプタヒープの初期化
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

	//毎フレーム処理
	void Update();

	void Draw();


	//座標取得
	const XMFLOAT3& GetPosition() { return position; }
	//座標セット
	void SetPosition(XMFLOAT3 position) { this->position = position; }
	//スケール取得
	const XMFLOAT3& GetScale() { return position; }
	//スケールセット
	void SetScale(XMFLOAT3 scale) { this->scale = scale; }

private:

	Model* model = nullptr;
	ComPtr<ID3D12Resource> constBuffB0; // 定数バッファ
	XMFLOAT4 color = { 1,1,1,1 };//色
	XMFLOAT3 scale = { 1,1,1 };//ローカルスケール
	XMFLOAT3 rotation = { 0,0,0 };//X,Y,Z軸周りのローカル回転角
	XMFLOAT3 position = { 0,0,0 };//ローカル座標
	XMMATRIX matWorld;//ローカルワールド変換行列
	XMMATRIX matScale;
	XMMATRIX matRot;
	XMMATRIX matTrans;

};