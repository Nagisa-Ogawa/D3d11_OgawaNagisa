//=============================================================================
// Graphics.h
// 
//=============================================================================
#pragma once

#include <d3d11.h>
#include "GameWindow.h"

// グラフィックスデバイスを表します。
class GraphicsDevice final
{
public:
	// このクラスのインスタンスを作成します。
	static GraphicsDevice* Create();

	// IDXGIFactory1オブジェクトを取得します。
	IDXGIFactory1* GetDXGIFactory();
	// IDXGIAdapter1オブジェクトを取得します。
	IDXGIAdapter1* GetDXGIAdapter();
	// IDXGIDevice1オブジェクトを取得します。
	IDXGIDevice1* GetDXGIDevice();
	// Direct3D 11のデバイスを取得します。
	ID3D11Device* GetDevice();
	// Direct3D 11の機能レベルを取得します。
	D3D_FEATURE_LEVEL GetFeatureLevel() const;
	// Direct3D 11のデバイス コンテキストを取得します。
	ID3D11DeviceContext* GetImmediateContext();

	// デストラクター
	~GraphicsDevice();

private:
	// インスタンス生成を禁止
	GraphicsDevice(
		IDXGIFactory1* dxgiFactory,
		IDXGIAdapter1* dxgiAdapter,
		IDXGIDevice1* dxgiDevice,
		ID3D11Device* device, D3D_FEATURE_LEVEL featureLevel,
		ID3D11DeviceContext* immediateContext);
	// コピーと代入演算を禁止
	GraphicsDevice(const GraphicsDevice&) {}
	GraphicsDevice& operator=(const GraphicsDevice&) { return *this; }

	// IDXGIFactory1オブジェクト
	IDXGIFactory1* dxgiFactory = nullptr;
	// IDXGIAdapter1オブジェクト
	IDXGIAdapter1* dxgiAdapter = nullptr;
	// IDXGIDevice1オブジェクト
	IDXGIDevice1* dxgiDevice = nullptr;
	// ID3D11Deviceオブジェクト
	ID3D11Device* device = nullptr;
	// Direct3D 11の機能レベル
	D3D_FEATURE_LEVEL featureLevel = {};
	// Direct3D 11のデバイス コンテキスト
	ID3D11DeviceContext* immediateContext = nullptr;
};

// グラフィックス リソースの基本クラスを表します。
class GraphicsResource
{
public:
	// Direct3D 11のデバイスを取得します。
	virtual GraphicsDevice* GetGraphicsDevice() final;

	// デストラクター
	virtual ~GraphicsResource();

protected:
	// インスタンス生成を禁止
	GraphicsResource(GraphicsDevice* graphicsDevice);
	// コピーを禁止
	GraphicsResource(const GraphicsResource&) {}
private:
	// 代入演算を禁止
	GraphicsResource& operator=(const GraphicsResource&) { return *this; }

	// このリソースを作成したID3D11Deviceオブジェクト
	GraphicsDevice* graphicsDevice = nullptr;
};

// スワップチェーンを表します。
class SwapChain : public GraphicsResource
{
public:
	// このクラスのインスタンスを作成します。
	static SwapChain* Create(GameWindow* window, GraphicsDevice* graphicsDevice);

	// ディスプレイにレンダリングイメージを表示します。
	void Present(UINT syncInterval, UINT flags);
	// スワップチェーンを取得します。
	IDXGISwapChain* GetNativePointer();

	// デストラクター
	~SwapChain();

private:
	// インスタンス生成を禁止
	SwapChain(GraphicsDevice* graphicsDevice, IDXGISwapChain* swapChain);
	// コピーを禁止
	SwapChain(const SwapChain& other) : GraphicsResource(other) {}
	// 代入演算を禁止
	SwapChain& operator=(const SwapChain&) { return *this; }

	// スワップチェーン
	IDXGISwapChain* nativePointer = nullptr;
};

// レンダーターゲットを表します。
class RenderTarget final : public GraphicsResource
{
public:
	// このクラスのインスタンスを作成します。
	static RenderTarget* Create(SwapChain* swapChain);

	// デストラクター
	~RenderTarget();

	// バックバッファーを取得します。
	ID3D11Texture2D* GetBuffer();
	// レンダーターゲットを取得します。
	ID3D11RenderTargetView* GetView();
	// バックバッファーをシェーダーで利用するためのリソース ビューを取得します。
	ID3D11ShaderResourceView* GetResourceView();

private:
	// バックバッファー
	ID3D11Texture2D* buffer = nullptr;
	// レンダーターゲット
	ID3D11RenderTargetView* view = nullptr;
	// バックバッファーをシェーダーで利用するためのリソース ビュー
	ID3D11ShaderResourceView* resourceView = nullptr;

	// インスタンス生成を禁止
	RenderTarget(
		GraphicsDevice* graphicsDevice,
		ID3D11Texture2D* backBuffer,
		ID3D11RenderTargetView* view,
		ID3D11ShaderResourceView* resourceView);
	// コピーと代入演算を禁止
	RenderTarget(const RenderTarget& other) : GraphicsResource(other) {}
	RenderTarget& operator=(const RenderTarget&) { return *this; }
};

// 深度ステンシルを表します。
class DepthStencil final : public GraphicsResource
{
public:
	// このクラスのインスタンスを作成します。
	static DepthStencil* Create(RenderTarget* renderTarget, DXGI_FORMAT format);

	// デストラクター
	~DepthStencil();

	// 深度ステンシルバッファーを取得します。
	ID3D11Texture2D* GetBuffer();
	// レンダーターゲットを取得します。
	ID3D11DepthStencilView* GetView();
	// バックバッファーをシェーダーで利用するためのリソース ビューを取得します。
	ID3D11ShaderResourceView* GetResourceView();

private:
	// バッファー
	ID3D11Texture2D* buffer = nullptr;
	// 深度ステンシル
	ID3D11DepthStencilView* view = nullptr;
	// 深度ステンシルをシェーダーで利用するためのリソース ビュー
	ID3D11ShaderResourceView* resourceView = nullptr;

	// インスタンス生成を禁止
	DepthStencil(
		GraphicsDevice* graphicsDevice,
		ID3D11Texture2D* buffer,
		ID3D11DepthStencilView* view,
		ID3D11ShaderResourceView* resourceView);
	// コピーと代入演算を禁止
	DepthStencil(const DepthStencil& other) : GraphicsResource(other) {}
	DepthStencil& operator=(const DepthStencil&) { return *this; }
};

// グラフィックス機能を表します。
class Graphics final
{
public:
	// グラフィックス機能を初期化します。
	static Graphics* Create(GameWindow* gameWindow);

	// グラフィックス デバイスを取得します。
	GraphicsDevice* GetGraphicsDevice();
	// スワップチェーンを取得します。
	SwapChain* GetSwapChain();
	// レンダーターゲットを取得します。
	RenderTarget* GetRenderTarget();
	// 深度ステンシルを取得します。
	DepthStencil* GetDepthStencil();

	// デストラクター
	~Graphics();

private:
	// グラフィックス デバイス
	GraphicsDevice* graphicsDevice = nullptr;
	// スワップチェーン
	SwapChain* swapChain = nullptr;
	// レンダーターゲット
	RenderTarget* renderTarget = nullptr;
	// 深度ステンシル
	DepthStencil* depthStencil = nullptr;

	// インスタンス生成を禁止
	Graphics(
		GraphicsDevice* graphicsDevice, SwapChain* swapChain,
		RenderTarget* renderTarget, DepthStencil* depthStencil);
	// コピーと代入演算を禁止
	Graphics(const Graphics&) {}
	Graphics& operator=(const Graphics&) { return *this; }
};

