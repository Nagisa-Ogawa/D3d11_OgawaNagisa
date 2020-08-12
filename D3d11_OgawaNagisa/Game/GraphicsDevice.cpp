//=============================================================================
// GraphicsDevice.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "Graphics.h"
#include "DirectXHelper.h"

using namespace Microsoft::WRL;
using namespace DX;

// このクラスのインスタンスを作成します。
std::shared_ptr<GraphicsDevice> GraphicsDevice::Create()
{
	// デバイス作成時のオプションフラグ
	UINT creationFlags = 0;
#if defined(_DEBUG)
	// DEBUGビルドの際にDirect3Dのデバッグ表示機能を持たせる
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	// デバイス、デバイスコンテキストを作成
	ComPtr<ID3D11Device> device;
	D3D_FEATURE_LEVEL featureLevel = {};
	ComPtr<ID3D11DeviceContext> immediateContext;
	ThrowIfFailed(D3D11CreateDevice(
		NULL, D3D_DRIVER_TYPE_HARDWARE, 0, creationFlags, NULL, 0, D3D11_SDK_VERSION,
		&device, &featureLevel, &immediateContext));

	// DXGIインターフェイスを取得
	ComPtr<IDXGIFactory1> dxgiFactory;
	ComPtr<IDXGIAdapter1> dxgiAdapter;
	ComPtr<IDXGIDevice1> dxgiDevice;
	ThrowIfFailed(device.As(&dxgiDevice));
	ThrowIfFailed(dxgiDevice->GetParent(IID_PPV_ARGS(&dxgiAdapter)));
	ThrowIfFailed(dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory)));

	return std::shared_ptr<GraphicsDevice>(
		new GraphicsDevice(
			dxgiFactory, dxgiAdapter, dxgiDevice,
			device, featureLevel, immediateContext));
}

// このクラスの新しいインスタンスを初期化します。
GraphicsDevice::GraphicsDevice(
	Microsoft::WRL::ComPtr<IDXGIFactory1> dxgiFactory,
	Microsoft::WRL::ComPtr<IDXGIAdapter1> dxgiAdapter,
	Microsoft::WRL::ComPtr<IDXGIDevice1> dxgiDevice,
	Microsoft::WRL::ComPtr<ID3D11Device> device,
	D3D_FEATURE_LEVEL featureLevel,
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediateContext)
{
	this->dxgiFactory = dxgiFactory;
	this->dxgiAdapter = dxgiAdapter;
	this->dxgiDevice = dxgiDevice;
	this->device = device;
	this->featureLevel = featureLevel;
	this->immediateContext = immediateContext;
}

// IDXGIFactory1オブジェクトを取得します。
Microsoft::WRL::ComPtr<IDXGIFactory1> GraphicsDevice::GetDXGIFactory()
{
	return dxgiFactory;
}

// IDXGIAdapter1オブジェクトを取得します。
Microsoft::WRL::ComPtr<IDXGIAdapter1> GraphicsDevice::GetDXGIAdapter()
{
	return dxgiAdapter;
}

// IDXGIDevice1オブジェクトを取得します。
Microsoft::WRL::ComPtr<IDXGIDevice1> GraphicsDevice::GetDXGIDevice()
{
	return dxgiDevice;
}

// Direct3D 11のデバイスを取得します。
Microsoft::WRL::ComPtr<ID3D11Device> GraphicsDevice::GetDevice()
{
	return device;
}

// Direct3D 11の機能レベルを取得します。
D3D_FEATURE_LEVEL GraphicsDevice::GetFeatureLevel() const
{
	return featureLevel;
}

// Direct3D 11のデバイス コンテキストを取得します。
Microsoft::WRL::ComPtr<ID3D11DeviceContext> GraphicsDevice::GetImmediateContext()
{
	return immediateContext;
}

// このクラスのインスタンスを初期化します。
GraphicsResource::GraphicsResource(
	std::shared_ptr<GraphicsDevice> graphicsDevice)
{
	this->graphicsDevice = graphicsDevice;
}

// Direct3D 11のデバイスを取得します。
std::shared_ptr<GraphicsDevice> GraphicsResource::GetGraphicsDevice()
{
	return graphicsDevice;
}
