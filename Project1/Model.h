#pragma once

#include <Windows.h>
#include <d3dx12.h>
#include <DirectXMath.h>

class Model {
private: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public: // サブクラス
	//頂点データ構造体
	struct VertexPosNormalUv
	{
		XMFLOAT3 pos; // xyz座標
		XMFLOAT3 normal; // 法線ベクトル
		XMFLOAT2 uv;  // uv座標
	};
	// 定数バッファ用データ構造体B1
	struct ConstBufferDataB1
	{
		XMFLOAT3 ambient;//アンビエント係数
		float pad1;//パディング
		XMFLOAT3 diffuse;//ディフューズ係数
		float pad2;//パディング
		XMFLOAT3 specular;//スペキュラー係数
		float alpha;//アルファ
	};
	//マテリアル
	struct Material
	{
		std::string name;//マテリアル名
		XMFLOAT3 ambient;//アンビエント影響度
		XMFLOAT3 diffuse;//ディフューズ影響度
		XMFLOAT3 specular;//スペキュラー影響度
		float alpha;//アルファ
		std::string textureFilename;//テクスチャファイル名
		//コンストラクタ
		Material() {
			ambient = { 0.3f,0.3f,0.3f };
			diffuse = { 0.0f,0.0f,0.0f };
			specular = { 0.0f,0.0f,0.0f };
			alpha = 1.0f;
		}
	};

private://静的メンバ関数

	bool LoadTexture(UINT texNumber,const std::string& directoryPath, const std::string& filename);

	///マテリアル読み込み
	void LoadMaterial(UINT texNumber,const std::string& directoryPath, const std::string& filename);
	//モデル生成
	void CreateModel(UINT texNumber,const std::string& modelname);
private:
	
	//デバイス
	static ID3D12Device* device;
	// シェーダリソースビューのハンドル(CPU)
	static CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// シェーダリソースビューのハンドル(CPU)
	static CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	// デスクリプタサイズ
	static UINT descriptorHandleIncrementSize;
	//マテリアル
	static Material material;


public:

	Model(UINT texNumber);

	static void StaticInitialize(ID3D12Device* device);

	static Model* CreateFromOBJ(UINT texNumber,const std::string& modelname);

	// デスクリプタヒープ
	static ComPtr<ID3D12DescriptorHeap> descHeap;

	//定数バッファ初期化
	bool Initialize();

	//定数バッファ更新用
	void Update();

	void Draw(ID3D12GraphicsCommandList* cmdList,
		ComPtr<ID3D12Resource> constBuffB0);




private:
	// テクスチャバッファ
	static const int SRVCount = 512;//モデルテクスチャの最大枚数
	ComPtr<ID3D12Resource> constBuffB1; // 定数バッファ
	ComPtr<ID3D12Resource> texbuff[SRVCount];
	ComPtr<ID3D12Resource> vertBuff;//頂点バッファ
	ComPtr<ID3D12Resource> indexBuff;//インデックスバッファ
		// 頂点データ配列
	std::vector<VertexPosNormalUv> modelvertices;
	// 頂点インデックス配列
	std::vector<unsigned short> modelindices;
	D3D12_VERTEX_BUFFER_VIEW vbView;
	D3D12_INDEX_BUFFER_VIEW ibView;

	UINT texnumber = 0;

};