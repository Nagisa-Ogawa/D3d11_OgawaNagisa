//=============================================================================
// GraphicsDevice.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "Graphics.h"
#include "DirectXHelper.h"

using namespace DX;

// このクラスのインスタンスを作成します。
GraphicsDevice* GraphicsDevice::Create()
{
	GraphicsDevice* retVal = nullptr;
	IDXGIFactory1* dxgiFactory = nullptr;
	IDXGIAdapter1* dxgiAdapter = nullptr;
	IDXGIDevice1* dxgiDevice = nullptr;
	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* immediateContext = nullptr;
	try {
		// デバイス作成時のオプションフラグ
		UINT creationFlags = 0;
#if defined(_DEBUG)
		// DEBUGビルドの際にDirect3Dのデバッグ表示機能を持たせる
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		// デバイス、デバイスコンテキストを作成
		D3D_FEATURE_LEVEL featureLevel = {};
		ThrowIfFailed(D3D11CreateDevice(
			NULL, D3D_DRIVER_TYPE_HARDWARE, 0, creationFlags, NULL, 0, D3D11_SDK_VERSION,
			&device, &featureLevel, &immediateContext));

		// DXGIインターフェイスを取得
		ThrowIfFailed(
			device->QueryInterface(IID_PPV_ARGS(&dxgiDevice)));
		ThrowIfFailed(dxgiDevice->GetParent(IID_PPV_ARGS(&dxgiAdapter)));
		ThrowIfFailed(dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory)));

		retVal = new GraphicsDevice(
			dxgiFactory, dxgiAdapter, dxgiDevice,
			device, featureLevel, immediateContext);
		SafeRelease(dxgiFactory);
		SafeRelease(dxgiAdapter);
		SafeRelease(dxgiDevice);
		SafeRelease(device);
		SafeRelease(immediateContext);
		return retVal;
	}
	catch (...) {
		SafeDelete(retVal);
		SafeRelease(dxgiFactory);
		SafeRelease(dxgiAdapter);
		SafeRelease(dxgiDevice);
		SafeRelease(device);
		SafeRelease(immediateContext);
		throw;
	}
	return nullptr;
}

// このクラスの新しいインスタンスを初期化します。
GraphicsDevice::GraphicsDevice(
	IDXGIFactory1* dxgiFactory,
	IDXGIAdapter1* dxgiAdapter,
	IDXGIDevice1* dxgiDevice,
	ID3D11Device* device, D3D_FEATURE_LEVEL featureLevel,
	ID3D11DeviceContext* immediateContext)
{
	dxgiFactory->AddRef();
	this->dxgiFactory = dxgiFactory;
	dxgiAdapter->AddRef();
	this->dxgiAdapter = dxgiAdapter;
	dxgiDevice->AddRef();
	this->dxgiDevice = dxgiDevice;

	device->AddRef();
	this->device = device;
	this->featureLevel = featureLevel;
	immediateContext->AddRef();
	this->immediateContext = immediateContext;
}

// デストラクター
GraphicsDevice::~GraphicsDevice()
{
	SafeRelease(dxgiFactory);
	SafeRelease(dxgiAdapter);
	SafeRelease(dxgiDevice);
	SafeRelease(immediateContext);
	SafeRelease(device);
}

// IDXGIFactory1オブジェクトを取得します。
IDXGIFactory1* GraphicsDevice::GetDXGIFactory()
{
	return dxgiFactory;
}

// IDXGIAdapter1オブジェクトを取得します。
IDXGIAdapter1* GraphicsDevice::GetDXGIAdapter()
{
	return dxgiAdapter;
}

// IDXGIDevice1オブジェクトを取得します。
IDXGIDevice1* GraphicsDevice::GetDXGIDevice()
{
	return dxgiDevice;
}

// Direct3D 11のデバイスを取得します。
ID3D11Device* GraphicsDevice::GetDevice()
{
	return device;
}

// Direct3D 11の機能レベルを取得します。
D3D_FEATURE_LEVEL GraphicsDevice::GetFeatureLevel() const
{
	return featureLevel;
}

// Direct3D 11のデバイス コンテキストを取得します。
ID3D11DeviceContext* GraphicsDevice::GetImmediateContext()
{
	return immediateContext;
}

// このクラスのインスタンスを初期化します。
GraphicsResource::GraphicsResource(GraphicsDevice* graphicsDevice)
{
	this->graphicsDevice = graphicsDevice;
}

// デストラクター
GraphicsResource::~GraphicsResource()
{
}

// Direct3D 11のデバイスを取得します。
GraphicsDevice* GraphicsResource::GetGraphicsDevice()
{
	return graphicsDevice;
}
