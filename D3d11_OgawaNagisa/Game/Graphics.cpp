//=============================================================================
// Graphics.cpp
// 
//=============================================================================
#include "Graphics.h"
#include "GameWindow.h"
#include "DirectXHelper.h"

using namespace DirectX;
using namespace DX;

// このクラスのインスタンスを作成します。
DXGIOutput* DXGIOutput::Create(IDXGIOutput* output)
{
	DXGIOutput* retVal = nullptr;
	// 表示モードの記述
	DXGI_MODE_DESC* modeDescs = nullptr;
	try {
		DXGI_OUTPUT_DESC outputDesc = {};
		// 出力の説明を取得
		ThrowIfFailed(output->GetDesc(&outputDesc));

		UINT numModes = 0;
		// 要求された形式と他の入力オプションに一致する表示モードを取得
		ThrowIfFailed(output->GetDisplayModeList(
			DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_SCALING,
			&numModes, NULL));
		modeDescs = new DXGI_MODE_DESC[numModes];
		ThrowIfFailed(output->GetDisplayModeList(
			DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_SCALING,
			&numModes, modeDescs));

		retVal = new DXGIOutput(output, outputDesc, numModes, modeDescs);
		return retVal;
	}
	catch (...) {
		SAFE_DELETE_ARRAY(modeDescs);
		SAFE_DELETE(retVal);
		throw;
	}
	return nullptr;
}

// このクラスのインスタンスを初期化します。
DXGIOutput::DXGIOutput(
	IDXGIOutput* output, const DXGI_OUTPUT_DESC& outputDesc,
	UINT numModes, const DXGI_MODE_DESC* modeDescs)
{
	output->AddRef();
	this->dxgiOutput = output;
	this->dxgiOutputDesc = outputDesc;
	this->numModes = numModes;
	this->dxgiModeDescs = modeDescs;
}

// デストラクター
DXGIOutput::~DXGIOutput()
{
	SAFE_RELEASE(dxgiOutput);
	SAFE_DELETE_ARRAY(dxgiModeDescs);
}

// このクラスのインスタンスを作成します。
GraphicsPart* GraphicsPart::Create(IDXGIAdapter1* adapter)
{
	GraphicsPart* retVal = nullptr;
	ID3D11Device* graphicsDevice = nullptr;
	ID3D11DeviceContext* immediateContext = nullptr;
	DXGIOutput* output = nullptr;
	try {
		DXGI_ADAPTER_DESC1 adapterDesc = {};
		ThrowIfFailed(adapter->GetDesc1(&adapterDesc));

		// デバイス作成時のオプションフラグ
		UINT creationFlags = 0;
#if defined(_DEBUG)
		// DEBUGビルドの際にDirect3Dのデバッグ表示機能を持たせる
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		// デバイス、デバイスコンテキストを作成
		D3D_FEATURE_LEVEL featureLevel = {};
		ThrowIfFailed(D3D11CreateDevice(
			adapter, D3D_DRIVER_TYPE_UNKNOWN, 0, creationFlags, NULL, 0, D3D11_SDK_VERSION,
			&graphicsDevice,
			&featureLevel,
			&immediateContext));
		// このクラスのインスタンスを作成
		retVal = new GraphicsPart(adapter,
			adapterDesc, graphicsDevice, featureLevel, immediateContext);
		SAFE_RELEASE(graphicsDevice);
		SAFE_RELEASE(immediateContext);
		// 
		IDXGIOutput* dxgiOutput = nullptr;
		for (UINT outputIndex = 0;
			adapter->EnumOutputs(outputIndex, &dxgiOutput) == S_OK;
			++outputIndex) {
			output = DXGIOutput::Create(dxgiOutput);
			retVal->dxgiOutputs.push_back(output);
			SAFE_RELEASE(dxgiOutput);
		}

		return retVal;
	}
	catch (...) {
		SAFE_RELEASE(graphicsDevice);
		SAFE_RELEASE(immediateContext);
		SAFE_DELETE(output);
		SAFE_DELETE(retVal);
		throw;
	}
	return nullptr;
}

// このクラスのインスタンスを初期化します。
GraphicsPart::GraphicsPart(
	IDXGIAdapter1* adapter, const DXGI_ADAPTER_DESC1& adapterDesc,
	ID3D11Device* graphicsDevice, const D3D_FEATURE_LEVEL& featureLevel,
	ID3D11DeviceContext* immediateContext)
{
	adapter->AddRef();
	this->dxgiAdapter = adapter;
	this->adapterDesc = adapterDesc;
	graphicsDevice->AddRef();
	this->graphicsDevice = graphicsDevice;
	this->featureLevel = featureLevel;
	immediateContext->AddRef();
	this->immediateContext = immediateContext;
}

// デストラクター
GraphicsPart::~GraphicsPart()
{
	SAFE_RELEASE(dxgiAdapter);
	SAFE_RELEASE(graphicsDevice);
	SAFE_RELEASE(immediateContext);
	for (auto&& value : dxgiOutputs) {
		SAFE_DELETE(value);
	}
	dxgiOutputs.clear();
}

// Direct3D 11のデバイスを取得します。
ID3D11Device* GraphicsPart::GetGraphicsDevice()
{
	return graphicsDevice;
}

// Direct3D 11の機能レベルを取得します。
D3D_FEATURE_LEVEL GraphicsPart::GetFeatureLevel() const
{
	return featureLevel;
}

// Direct3D 11のデバイス コンテキストを取得します。
ID3D11DeviceContext* GraphicsPart::GetImmediateContext()
{
	return immediateContext;
}

// このクラスのシングルトンインスタンスを取得します。
Graphics& Graphics::GetInstance()
{
	static Graphics instance;
	return instance;
}

// Direct3D 11のデバイスを取得します。
ID3D11Device* Graphics::GetGraphicsDevice()
{
	return GetInstance().graphicsParts[0]->GetGraphicsDevice();
}

// Direct3D 11のデバイス コンテキストを取得します。
ID3D11DeviceContext* Graphics::GetImmediateContext()
{
	return GetInstance().graphicsParts[0]->GetImmediateContext();
}

// レンダーターゲットを取得します。
ID3D11RenderTargetView* Graphics::GetRenderTargetView()
{
	return GetInstance().renderTargetView;
}

// 深度ステンシルを取得します。
ID3D11DepthStencilView* Graphics::GetDepthStencilView()
{
	return GetInstance().depthStencilView;
}

// スワップチェーンを取得します。
IDXGISwapChain* Graphics::GetSwapChain()
{
	return GetInstance().swapChain;
}

// グラフィックス機能を初期化します。
void Graphics::Initialize()
{
	GetInstance().OnCreate();
}

// このクラスのインスタンスを作成する際に呼び出されます。
void Graphics::OnCreate()
{
	IDXGIFactory1* dxgiFactory = nullptr;
	IDXGIAdapter1* dxgiAdapter = nullptr;
	GraphicsPart* graphicsPart = nullptr;

	ID3D11Texture2D* backBuffer = nullptr;
	ID3D11Texture2D* depthStencil = nullptr;
	try {
		// DXGIファクトリーを作成
		ThrowIfFailed(CreateDXGIFactory1(
			IID_PPV_ARGS(&dxgiFactory)
		));
		UINT adapter = 0;
		// ビデオカードを列挙する
		while (dxgiFactory->EnumAdapters1(
			adapter,	// アダプターのインデックス
			&dxgiAdapter	// IDXGIAdapter1のポインターのアドレス
		) == S_OK) {
			// グラフィックデバイスとイミディエイトコンテキストを作成
			graphicsPart = GraphicsPart::Create(dxgiAdapter);
			// graphicsPartを追加
			graphicsParts.push_back(graphicsPart);
			SAFE_RELEASE(dxgiAdapter);
			++adapter;
		}

		// デバイス作成時のオプションフラグ
		UINT creationFlags = 0;
#if defined(_DEBUG)
		// DEBUGビルドの際にDirect3Dのデバッグ表示機能を持たせる
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		// 作成するスワップチェーンについての記述
		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
		// 解像度の幅を表す。0を指定すると出力ウィンドウから幅を取得し
		// スワップチェーンに割り当てる
		swapChainDesc.BufferDesc.Width = GameWindow::GetWidth();
		// 解像度の高さを表す。0を指定すると出力ウィンドウから高さを取得し
		// スワップチェーンに割り当てる
		swapChainDesc.BufferDesc.Height = GameWindow::GetHeight();
		// リフレッシュレートを表す
		swapChainDesc.BufferDesc.RefreshRate = { 60, 1 };
		// データをどのように扱うかの形式
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		// マルチサンプリングのパラメータ
		swapChainDesc.SampleDesc = { 1, 0 };
		// バックバッファーの表面使用量とCPUアクセスオプションを設定するメンバー
		swapChainDesc.BufferUsage =
			DXGI_USAGE_RENDER_TARGET_OUTPUT |
			DXGI_USAGE_SHADER_INPUT;	// シェーダーリソースとして使用することを設定
		// スワップチェーン内のバッファの数。通常はこの数に
		// フロントバッファも含める
		swapChainDesc.BufferCount = 2;
		// 出力ウィンドウへのHWNDハンドル。NULLではいけない
		swapChainDesc.OutputWindow = GameWindow::GetHwnd();
		// バッファの切り替え方のパラメータ
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		// 出力がウィンドウモードであるかを指定するパラメータ
		swapChainDesc.Windowed = TRUE;
		//// デバイス、デバイスコンテキスト、スワップチェーンを作成
		//ThrowIfFailed(D3D11CreateDeviceAndSwapChain(
		//	NULL, D3D_DRIVER_TYPE_HARDWARE, 0, creationFlags, NULL, 0, D3D11_SDK_VERSION, &swapChainDesc,
		//	&swapChain, &graphicsDevice, &featureLevel, &immediateContext));

		auto graphicsPart = graphicsParts[0];
		auto graphicsDevice = graphicsPart->GetGraphicsDevice();
		auto featureLevel = graphicsPart->GetFeatureLevel();
		auto immediateContext = graphicsPart->GetImmediateContext();
		// スワップチェーンを作成
		ThrowIfFailed(dxgiFactory->CreateSwapChain(graphicsDevice, &swapChainDesc, &swapChain));
		ThrowIfFailed(swapChain->GetDesc(&swapChainDesc));

		// バックバッファーを取得
		ThrowIfFailed(swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer)));
		// バックバッファーにアクセスするためのレンダーターゲット ビューを作成
		ThrowIfFailed(graphicsDevice->CreateRenderTargetView(
			backBuffer, NULL, &renderTargetView));
		// バックバッファーにシェーダーからアクセスするためのリソース ビューを作成
		ThrowIfFailed(graphicsDevice->CreateShaderResourceView(
			backBuffer, NULL, &renderTargetResourceView));
		SAFE_RELEASE(backBuffer);

		// テクスチャーとシェーダーリソースビューのフォーマットを設定
		DXGI_FORMAT textureFormat = depthStencilFormat;
		DXGI_FORMAT resourceFormat = depthStencilFormat;
		// 深度ステンシルのフォーマットの形式に
		// テクスチャとシェーダーリソースのフォーマットも合わせる
		switch (depthStencilFormat)
		{
		case DXGI_FORMAT_D16_UNORM:
			textureFormat = DXGI_FORMAT_R16_TYPELESS;
			resourceFormat = DXGI_FORMAT_R16_UNORM;
			break;
		case DXGI_FORMAT_D24_UNORM_S8_UINT:
			textureFormat = DXGI_FORMAT_R24G8_TYPELESS;
			resourceFormat = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
			break;
		case DXGI_FORMAT_D32_FLOAT:
			textureFormat = DXGI_FORMAT_R32_TYPELESS;
			resourceFormat = DXGI_FORMAT_R32_FLOAT;
			break;
		case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
			textureFormat = DXGI_FORMAT_R32G8X24_TYPELESS;
			resourceFormat = DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;
			break;
		}
		// 深度ステンシルを作成
		D3D11_TEXTURE2D_DESC depthStencilDesc = {};
		depthStencilDesc.Width = swapChainDesc.BufferDesc.Width;
		depthStencilDesc.Height = swapChainDesc.BufferDesc.Height;
		// テクスチャ内のミップマップレベルの最大数。
		// マルチサンプルテクスチャには1を使用する
		depthStencilDesc.MipLevels = 1;
		// テクスチャ配列内のテクスチャ
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = textureFormat;
		depthStencilDesc.SampleDesc = swapChainDesc.SampleDesc;
		// テクスチャの読み書き方法を識別する値
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		// パイプラインステージにバインドするためのフラグ
		depthStencilDesc.BindFlags =
			D3D11_BIND_DEPTH_STENCIL |
			D3D11_BIND_SHADER_RESOURCE;	// シェーダーリソースとして使用することを設定
		// 許可されるCPUアクセスのタイプを指定するフラグ
		// CPUアクセスが必要ない場合は0を指定
		depthStencilDesc.CPUAccessFlags = 0;
		// リソースオプションを指定するフラグ
		// 使用しない場合は0を指定
		depthStencilDesc.MiscFlags = 0;

		ThrowIfFailed(graphicsDevice->CreateTexture2D(&depthStencilDesc, NULL, &depthStencil));
		// 深度ステンシルにアクセスするためのビューを作成
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
		depthStencilViewDesc.Format = depthStencilFormat;
		if (depthStencilDesc.SampleDesc.Count > 0) {
			depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
		}
		else {
			depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			depthStencilViewDesc.Texture2D.MipSlice = 0;
		}
		depthStencilViewDesc.Texture2D.MipSlice = 0;
		ThrowIfFailed(graphicsDevice->CreateDepthStencilView(depthStencil, &depthStencilViewDesc, &depthStencilView));
		// 深度ステンシルにシェーダーからアクセスするためのリソース ビューを作成
		D3D11_SHADER_RESOURCE_VIEW_DESC depthStencilResourceViewDesc = {};
		depthStencilResourceViewDesc.Format = resourceFormat;
		if (depthStencilDesc.SampleDesc.Count > 0) {
			depthStencilResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
		}
		else {
			depthStencilResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			depthStencilResourceViewDesc.Texture2D.MostDetailedMip = 0;
			depthStencilResourceViewDesc.Texture2D.MipLevels = 1;
		}
		ThrowIfFailed(graphicsDevice->CreateShaderResourceView(
			depthStencil, &depthStencilResourceViewDesc, &depthStencilResourceView));
		SAFE_RELEASE(depthStencil);


		SAFE_RELEASE(dxgiFactory);


	}
	catch (...) {
		SAFE_RELEASE(dxgiFactory);
		SAFE_RELEASE(dxgiAdapter);
		SAFE_DELETE(graphicsPart);

		SAFE_RELEASE(swapChain);
		SAFE_RELEASE(graphicsDevice);
		SAFE_RELEASE(immediateContext);

		SAFE_RELEASE(backBuffer);
		SAFE_RELEASE(renderTargetView);
		SAFE_RELEASE(renderTargetResourceView);

		SAFE_RELEASE(depthStencil);
		SAFE_RELEASE(depthStencilView);
		SAFE_RELEASE(depthStencilResourceView);

		throw;
	}
}

// このクラスのインスタンスを破棄する際に呼び出されます。
void Graphics::Release()
{
	GetInstance().OnRelease();
}

// グラフィックリソースを解放します。
void Graphics::OnRelease()
{
	SAFE_RELEASE(depthStencilResourceView);
	SAFE_RELEASE(depthStencilView);
	SAFE_RELEASE(renderTargetResourceView);
	SAFE_RELEASE(renderTargetView);
	SAFE_RELEASE(swapChain);
	SAFE_RELEASE(immediateContext);
	SAFE_RELEASE(graphicsDevice);

	for (auto&& graphicsPart : graphicsParts) {
		SAFE_DELETE(graphicsPart);
	}
	graphicsParts.clear();
}
