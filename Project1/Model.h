#pragma once

#include <Windows.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <unordered_map>
#include<functional>
#include<DirectXTex.h>
#include <map>

enum Modeldata {//読み込むモデルデータ判別用
	PMD,
	OBJ,
};

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

	//PMD用
	//シェーダ側に投げられるマテリアルデータ
	struct MaterialForHlsl {
		DirectX::XMFLOAT3 pmddiffuse; //ディフューズ色
		float pmdalpha; // ディフューズα
		DirectX::XMFLOAT3 pmdspecular; //スペキュラ色
		float pmdspecularity;//スペキュラの強さ(乗算値)
		DirectX::XMFLOAT3 pmdambient; //アンビエント色
	};
	//それ以外のマテリアルデータ
	struct AdditionalMaterial {
		std::string texPath;//テクスチャファイルパス
		int toonIdx; //トゥーン番号
		bool edgeFlg;//マテリアル毎の輪郭線フラグ
	};
	//まとめたもの
	struct PMDMaterial {
		unsigned int indicesNum;//インデックス数
		MaterialForHlsl pmdmaterial;
		AdditionalMaterial pmdadditional;
	};

	struct Transform {
		//内部に持ってるXMMATRIXメンバが16バイトアライメントであるため
		//Transformをnewする際には16バイト境界に確保する
		void* operator new(size_t size);
		DirectX::XMMATRIX world;
	};

	struct SceneData
	{
		XMMATRIX view;//ビュー行列
		XMMATRIX proj;//プロジェクション行列
		XMMATRIX viewproj;//プロジェクション行列
		XMFLOAT3 eye;//視点座標
	};

private://静的メンバ関数

	bool LoadTexture(UINT texNumber,const std::string& directoryPath, const std::string& filename);

	///マテリアル読み込み
	void LoadMaterial(UINT texNumber,const std::string& directoryPath, const std::string& filename);
	//モデル生成
	void CreateModel(UINT texNumber,const std::string& modelname);

	//PMDファイル読み込み
	void LoadPMDFile(const char* path);
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

	///@param texpath テクスチャファイルパス
	ComPtr<ID3D12Resource> GetTextureByPath(const char* texpath);

	//テクスチャ名からテクスチャバッファ作成、中身をコピー
	ID3D12Resource* CreateTextureFromFile(const char* texpath);

	//テクスチャローダテーブルの作成
	void CreateTextureLoaderTable();


	ID3D12Resource* CreateDefaultTexture(size_t width, size_t height);
	ID3D12Resource* CreateWhiteTexture();//白テクスチャの生成
	ID3D12Resource*	CreateBlackTexture();//黒テクスチャの生成
	ID3D12Resource*	CreateGrayGradationTexture();//グレーテクスチャの生成

public:

	Model(UINT texNumber);

	static void StaticInitialize(ID3D12Device* device);

	//OBJ実体生成
	static Model* CreateFromOBJ(UINT texNumber,const std::string& modelname);

	//PMD実体生成
	static Model* CreateFromPMD(UINT texNumber, const char* modelname);
	// デスクリプタヒープ
	static ComPtr<ID3D12DescriptorHeap> descHeap;

	//定数バッファ初期化
	bool Initialize();

	//定数バッファ更新用
	void Update();

	void Draw(ID3D12GraphicsCommandList* cmdList,
		ComPtr<ID3D12Resource> constBuffB0);

	//マテリアル＆テクスチャのビューを作成
	HRESULT CreateMaterialAndTextureView();

	//読み込んだマテリアルをもとにマテリアルバッファを作成
	HRESULT CreateMaterialData();

	//座標変換用ビューの生成
	HRESULT CreateTransformView();

	//モデルデータの種類取得
	Modeldata GetModeldata() { return modeldata; }


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

	//マテリアル関連
	std::vector<PMDMaterial> materials;
	ComPtr<ID3D12Resource> materialBuff = nullptr;
	std::vector<ComPtr<ID3D12Resource>> textureResources;
	std::vector<ComPtr<ID3D12Resource>> sphResources;
	std::vector<ComPtr<ID3D12Resource>> spaResources;
	std::vector<ComPtr<ID3D12Resource>> toonResources;
	ComPtr< ID3D12DescriptorHeap> materialHeap = nullptr;//マテリアルヒープ(5個ぶん)

	Modeldata modeldata;
	ComPtr<ID3D12Resource> cameraconstBuff; // 定数バッファ

	Transform transform;
	Transform* mappedTransform = nullptr;
	ComPtr<ID3D12Resource> transformBuff = nullptr;
	ComPtr<ID3D12DescriptorHeap> transformHeap = nullptr;//座標変換ヒープ

	//ロード用テーブル
	using LoadLambda_t = std::function<HRESULT(const std::wstring& path, DirectX::TexMetadata*, DirectX::ScratchImage&)>;
	std::map <std::string, LoadLambda_t> loadLambdaTable;
	//テクスチャテーブル
	std::unordered_map<std::string, ComPtr<ID3D12Resource>> _textureTable;

	//PMD用共通テクスチャ(白、黒、グレイスケールグラデーション)
	ComPtr<ID3D12Resource> whiteTex = nullptr;
	ComPtr<ID3D12Resource> blackTex = nullptr;
	ComPtr<ID3D12Resource> gradTex = nullptr;

	
	unsigned int indicesNum;//インデックス数
	float angle = 0;

};