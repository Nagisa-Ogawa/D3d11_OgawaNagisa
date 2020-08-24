//=============================================================================
// GraphicsDevice.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "Graphics.h"
#include "DirectXHelper.h"

using namespace Microsoft::WRL;
using namespace DX;

// このクラスの新しいインスタンスを初期化します。
GraphicsDevice::GraphicsDevice(
	std::shared_ptr<Adapter> adapter, UINT creationFlags)
{
	try {
		// デバイス、デバイスコンテキストを作成
		const D3D_FEATURE_LEVEL featureLevels[] = {
			D3D_FEATURE_LEVEL_11_0,
			//D3D_FEATURE_LEVEL_10_1,
			//D3D_FEATURE_LEVEL_10_0,
			//D3D_FEATURE_LEVEL_9_3,
			//D3D_FEATURE_LEVEL_9_2,
			//D3D_FEATURE_LEVEL_9_1,
		};
		if (adapter == nullptr) {
			ThrowIfFailed(D3D11CreateDevice(
				NULL, D3D_DRIVER_TYPE_HARDWARE,
				0, creationFlags,
				featureLevels, ARRAYSIZE(featureLevels),
				D3D11_SDK_VERSION,
				device.GetAddressOf(), &featureLevel, immediateContext.GetAddressOf()));
		}
		else {
			ThrowIfFailed(D3D11CreateDevice(
				adapter->GetNativePointer().Get(), D3D_DRIVER_TYPE_UNKNOWN,
				0, creationFlags,
				featureLevels, ARRAYSIZE(featureLevels),
				D3D11_SDK_VERSION,
				device.GetAddressOf(), &featureLevel, immediateContext.GetAddressOf()));
		}
		// DXGIインターフェイスを取得
		ThrowIfFailed(device.As(&dxgiDevice));
		ThrowIfFailed(dxgiDevice->GetParent(IID_PPV_ARGS(&dxgiAdapter)));
		ThrowIfFailed(dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory)));
	}
	catch (...) {
		device.Reset();
		immediateContext.Reset();
		dxgiDevice.Reset();
		dxgiAdapter.Reset();
		dxgiFactory.Reset();
		throw;
	}
}

// IDXGIFactory1オブジェクトを取得します。
Microsoft::WRL::ComPtr<IDXGIFactory1> GraphicsDevice::GetDXGIFactory() const
{
	return dxgiFactory;
}

// IDXGIAdapter1オブジェクトを取得します。
Microsoft::WRL::ComPtr<IDXGIAdapter1> GraphicsDevice::GetDXGIAdapter() const
{
	return dxgiAdapter;
}

// IDXGIDevice1オブジェクトを取得します。
Microsoft::WRL::ComPtr<IDXGIDevice1> GraphicsDevice::GetDXGIDevice() const
{
	return dxgiDevice;
}

// Direct3D 11のデバイスを取得します。
Microsoft::WRL::ComPtr<ID3D11Device> GraphicsDevice::GetDevice() const
{
	return device;
}

// Direct3D 11の機能レベルを取得します。
D3D_FEATURE_LEVEL GraphicsDevice::GetFeatureLevel() const
{
	return featureLevel;
}

// Direct3D 11のデバイス コンテキストを取得します。
Microsoft::WRL::ComPtr<ID3D11DeviceContext> GraphicsDevice::GetImmediateContext() const
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

// Direct3D 11のデバイスを取得します。
std::shared_ptr<const GraphicsDevice> GraphicsResource::GetGraphicsDevice() const
{
	return graphicsDevice;
}
