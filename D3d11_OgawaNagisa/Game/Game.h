//=============================================================================
// Game.h
// 
//=============================================================================
#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl/client.h>

#define SAFE_RELEASE(p) if ((p) != nullptr) { (p)->Release(); (p) = nullptr; }

// アプリケーション全体を表します。
class Game {
public:
	// アプリケーションを初期化します。
	void Initialize(LPCWSTR windowTitle, int screenWidth, int screenHeight);
	// メッセージループを実行します。
	int Run();

private:
	// ウィンドウのタイトル
	LPCWSTR WindowTitle = L"タイトル";
	// ウィンドウの幅
	int ScreenWidth = 640;
	// ウィンドウの高さ
	int ScreenHeight = 480;
	// ウィンドウのハンドル
	HWND hWnd = NULL;

	// ウィンドウを作成
	bool InitWindow();

	// Direct3D 11のデバイス
	ID3D11Device* graphicsDevice = nullptr;
	// Direct3D 11のデバイス コンテキスト
	ID3D11DeviceContext* immediateContext = nullptr;
	// Direct3D 11の機能レベル
	D3D_FEATURE_LEVEL featureLevel = {};
	// 作成する機能レベル。希望順に指定する
	const D3D_FEATURE_LEVEL featureLevels[6] = {
		//D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};
	// スワップチェーン
	IDXGISwapChain* swapChain = nullptr;
	// レンダーターゲット
	ID3D11RenderTargetView* renderTargetViews[1];
	// バックバッファ－をシェーダーで利用するためのリソースビュー
	ID3D11ShaderResourceView* renderTargetResourceView = nullptr;
	// 深度ステンシルのフォーマット
	//const DXGI_FORMAT depthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	const DXGI_FORMAT depthStencilFormat = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
	// 深度ステンシル
	ID3D11DepthStencilView* depthStencilView = nullptr;
	// 深度ステンシルをシェーダーで使用するためのリソースビュー
	ID3D11ShaderResourceView* depthStencilResourceView = nullptr;
	// 画面クリアーに使用するカラー
	FLOAT clearColor[4] = { 53 / 255.0f, 70 / 255.0f, 166 / 255.0f };
	// ビューポート
	D3D11_VIEWPORT viewports[1] = {};

	// グラフィックデバイスを作成します
	bool InitGraphicsDevice();
	// グラフィックリソースを開放します
	void ReleaseGraphicsDevice();

};

// 位置座標のみを頂点情報に持つデータを表します。
struct VertexPosition
{
	DirectX::XMFLOAT3 position;	// 位置座標

	// この頂点情報をD3D11_INPUT_ELEMENT_DESCで表した配列を取得します。
	static const D3D11_INPUT_ELEMENT_DESC* GetInputElementDescs();
	// GetInputElementDescs()関数で取得される配列の要素数を取得します。
	static UINT GetInputElementDescsLength();
};

// 位置座標と法線ベクトルを頂点情報に持つデータを表します。
struct VertexPositionNormal
{
	DirectX::XMFLOAT3 position;	// 位置座標
	DirectX::XMFLOAT3 normal;	// 法線ベクトル

	// この頂点情報をD3D11_INPUT_ELEMENT_DESCで表した配列を取得します。
	static const D3D11_INPUT_ELEMENT_DESC* GetInputElementDescs();
	// GetInputElementDescs()関数で取得される配列の要素数を取得します。
	static UINT GetInputElementDescsLength();
};

// 位置座標と法線ベクトルとテクスチャー座標を頂点情報に持つデータを表します
struct VertexPositionNormalTexture
{
	DirectX::XMFLOAT3 position;	// 位置座標
	DirectX::XMFLOAT3 normal;	// 法線ベクトル
	DirectX::XMFLOAT2 texCoord;	// テクスチャー座標(UV)

	// この頂点情報をD3D11_INPUT_ELEMENT_DESCで表した配列を取得します。
	static const D3D11_INPUT_ELEMENT_DESC* GetInputElementDescs();
	// GetInputElementDescs()関数で取得される配列の要素数を取得します。
	static UINT GetInputElementDescsLength();

};

// 頂点シェーダ―を表します。
class BasicVertexShader
{
private:
	// D3D11のインターフェース
	ID3D11VertexShader* shader = nullptr;

public:
	// このクラスの新しいインスタンスを作成します
	static BasicVertexShader* Create(ID3D11Device* graphicsDevice);
	// リソースを開放します
	void Release();
	// D3D11のネイティブポインターを取得します
	ID3D11VertexShader* GetNativePointer();
	// このシェーダーのバイトコードを取得します。
	const BYTE* GetBytecode();
	// バイトコードのサイズを取得します。
	SIZE_T GetBytecodeLength();
};

// ジオメトリーシェーダーを表します。
class BasicGeometryShader
{
private:
	// D3D11のインターフェース
	ID3D11GeometryShader* shader = nullptr;

public:
	// このクラスの新しいインスタンスを作成します
	static BasicGeometryShader* Create(ID3D11Device* graphicsDevice);
	// リソースを開放します
	void Release();
	// D3D11のネイティブポインターを取得します
	ID3D11GeometryShader* GetNativePointer();
};

// ピクセルシェーダ―を表します。
class BasicPixelShader
{
private:
	// D3D11のインターフェース
	ID3D11PixelShader* shader = nullptr;

public:
	// このクラスの新しいインスタンスを作成します
	static BasicPixelShader* Create(ID3D11Device* graphicsDevice);
	// リソースを開放します
	void Release();
	// D3D11のネイティブポインターを取得します
	ID3D11PixelShader* GetNativePointer();
};

// 頂点バッファーを表します。
class VertexBuffer
{
	// D3D11リソース
	ID3D11Buffer* buffer = nullptr;

public:
	// このクラスの新しいインスタンスを作成します。
	static VertexBuffer* Create(ID3D11Device* graphicsDevice, UINT byteWidth);
	// リソースを開放します。
	void Release();
	// D3D11のネイティブポインターを取得します。
	ID3D11Buffer* GetNativePointer();

	// バッファーにデータを設定します。
	void SetData(void* data);
};

// インデックスバッファーを表します。
class IndexBuffer
{
	// D3D11リソース
	ID3D11Buffer* buffer = nullptr;

public:
	// このクラスの新しいインスタンスを作成します。
	static IndexBuffer* Create(ID3D11Device* graphicsDevice, UINT indexCount);
	// リソースを解放します。
	void Release();
	// D3D11のネイティブポインターを取得します。
	ID3D11Buffer* GetNativePointer();

	// バッファーにデータを設定します。
	void SetData(UINT32* data);
};

// 定数バッファーを表します。
class ConstantBuffer
{
	// D3D11リソース
	ID3D11Buffer* buffer = nullptr;

public:
	// このクラスの新しいインスタンスを作成します。
	static ConstantBuffer* Create(ID3D11Device* graphicsDevice, UINT byteWidth);
	// リソースを解放します。
	void Release();
	// D3D11のネイティブポインターを取得します。
	ID3D11Buffer* GetNativePointer();

	// バッファーにデータを設定します。
	void SetData(void* data);
};

// 入力レイアウトを表します。
class InputLayout
{
	// D3D11リソース
	ID3D11InputLayout* inputLayout = nullptr;

public:
	// このクラスの新しいインスタンスを作成します。
	static InputLayout* Create(
		ID3D11Device* graphicsDevice,
		const D3D11_INPUT_ELEMENT_DESC* inputElementDescs, UINT numElements,
		const void* shaderBytecodeWithInputSignature, SIZE_T bytecodeLength);
	// リソースを開放します。
	void Release();
	// D3D11のネイティブポインターを取得します。
	ID3D11InputLayout* GetNativePointer();
};

class Texture2D
{
	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView;
public:
	UINT width;
	// このクラスの新しいインスタンスを作成します。
	static Texture2D* Create(
		ID3D11Device* graphicsDevice,
		UINT width, UINT height, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, bool mipChain = true);
	// リソースを解放します。
	void Release();

	// テクスチャーのピクセルを変更します。
	void SetData(const void* data);

	// D3D11のネイティブポインターを取得します。
	ID3D11Texture2D* GetNativePointer();
	ID3D11SamplerState* GetSamplerState();
	ID3D11ShaderResourceView* GetShaderResourceView();
};

class ReadBitMap
{
	uint32_t* source;
public:
	UINT width;
	UINT height;
	// bmpからデータを読み込みます
	uint32_t* ReadFromBitMap(char* path);
	// リソースを開放します。
	void Release();
};