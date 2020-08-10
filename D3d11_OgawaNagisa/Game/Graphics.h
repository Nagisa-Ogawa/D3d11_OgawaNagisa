//=============================================================================
// Graphics.h
// 
//=============================================================================
#pragma once

#include <vector>
#include <d3d11.h>
#include <DirectXMath.h>

class DXGIOutput
{
public:
	// このクラスのインスタンスを作成します。
	static DXGIOutput* Create(IDXGIOutput* output);
	// デストラクター
	~DXGIOutput();

private:
	IDXGIOutput* dxgiOutput = nullptr;
	DXGI_OUTPUT_DESC dxgiOutputDesc = {};
	UINT numModes = 0;
	const DXGI_MODE_DESC* dxgiModeDescs = nullptr;

	// このクラスのインスタンスを初期化します。
	DXGIOutput(IDXGIOutput* output, const DXGI_OUTPUT_DESC& outputDesc,
		UINT numModes, const DXGI_MODE_DESC* modeDescs);
};

class GraphicsPart
{
public:
	// このクラスのインスタンスを作成します。
	static GraphicsPart* Create(IDXGIAdapter1* adapter);

	// デストラクター
	~GraphicsPart();

	// Direct3D 11のデバイスを取得します。
	ID3D11Device* GetGraphicsDevice();
	// Direct3D 11の機能レベルを取得します。
	D3D_FEATURE_LEVEL GetFeatureLevel() const;
	// Direct3D 11のデバイス コンテキストを取得します。
	ID3D11DeviceContext* GetImmediateContext();

private:
	// 
	IDXGIAdapter1* dxgiAdapter = nullptr;
	// 
	DXGI_ADAPTER_DESC1 adapterDesc = {};
	// Direct3D 11のデバイス
	ID3D11Device* graphicsDevice = nullptr;
	// Direct3D 11の機能レベル
	D3D_FEATURE_LEVEL featureLevel = {};
	// Direct3D 11のデバイス コンテキスト
	ID3D11DeviceContext* immediateContext = nullptr;
	// 
	std::vector<DXGIOutput*> dxgiOutputs;

	// このクラスのインスタンスを初期化します。
	GraphicsPart(
		IDXGIAdapter1* adapter, const DXGI_ADAPTER_DESC1& adapterDesc,
		ID3D11Device* graphicsDevice, const D3D_FEATURE_LEVEL& featureLevel,
		ID3D11DeviceContext* immediateContext);
};

class Graphics
{
public:
	// グラフィックス機能を初期化します。
	static void Initialize();
	// グラフィックスリソースを解放します。
	static void Release();

	// Direct3D 11のデバイスを取得します。
	static ID3D11Device* GetGraphicsDevice();
	// Direct3D 11のデバイス コンテキストを取得します。
	static ID3D11DeviceContext* GetImmediateContext();
	// レンダーターゲットを取得します。
	static ID3D11RenderTargetView* GetRenderTargetView();
	// 深度ステンシルを取得します。
	static ID3D11DepthStencilView* GetDepthStencilView();
	// スワップチェーンを取得します。
	static IDXGISwapChain* GetSwapChain();

private:
	// このクラスのシングルトンインスタンスを取得します。
	static Graphics& GetInstance();

	std::vector<GraphicsPart*> graphicsParts;

	// Direct3D 11のデバイス
	ID3D11Device* graphicsDevice = nullptr;
	// Direct3D 11のデバイス コンテキスト
	ID3D11DeviceContext* immediateContext = nullptr;
	// Direct3D 11の機能レベル
	D3D_FEATURE_LEVEL featureLevel = {};

	// スワップチェーン
	IDXGISwapChain* swapChain = nullptr;

	// レンダーターゲット
	ID3D11RenderTargetView* renderTargetView = nullptr;
	// バックバッファーをシェーダーで利用するためのリソース ビュー
	ID3D11ShaderResourceView* renderTargetResourceView = nullptr;

	// 深度ステンシルのフォーマット
	const DXGI_FORMAT depthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	// 深度ステンシル
	ID3D11DepthStencilView* depthStencilView = nullptr;
	// 深度ステンシルをシェーダーで利用するためのリソース ビュー
	ID3D11ShaderResourceView* depthStencilResourceView = nullptr;

	// このクラスのインスタンスを作成する際に呼び出されます。
	void OnCreate();
	// このクラスのインスタンスを破棄する際に呼び出されます。
	void OnRelease();
};

