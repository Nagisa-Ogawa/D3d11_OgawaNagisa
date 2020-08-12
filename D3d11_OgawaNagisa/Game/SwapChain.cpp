//=============================================================================
// SwapChain.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "Graphics.h"
#include "DirectXHelper.h"

using namespace Microsoft::WRL;
using namespace DX;

// このクラスのインスタンスを作成します。
std::shared_ptr<SwapChain> SwapChain::Create(
	std::shared_ptr<GameWindow> window,
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	DXGI_FORMAT format, const DXGI_SAMPLE_DESC& sampleDesc)
{
	// 作成するスワップチェーンについての記述
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferDesc.Width = window->GetWidth();
	swapChainDesc.BufferDesc.Height = window->GetHeight();
	swapChainDesc.BufferDesc.RefreshRate = { 60, 1 };
	swapChainDesc.BufferDesc.Format = format;
	swapChainDesc.SampleDesc = sampleDesc;
	swapChainDesc.BufferUsage =
		DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.OutputWindow = window->GetHwnd();
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Windowed = TRUE;
	// スワップチェーンを作成
	ComPtr<IDXGISwapChain> swapChain;
	ThrowIfFailed(
		graphicsDevice->GetDXGIFactory()->CreateSwapChain(
			graphicsDevice->GetDevice().Get(), &swapChainDesc, &swapChain));

	return std::shared_ptr<SwapChain>(
		new SwapChain(graphicsDevice, swapChain));
}

// インスタンス生成を禁止
SwapChain::SwapChain(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain)
	: GraphicsResource(graphicsDevice)
{
	this->nativePointer = swapChain;
}

// ディスプレイにレンダリングイメージを表示します。
void SwapChain::Present(UINT syncInterval, UINT flags)
{
	ThrowIfFailed(nativePointer->Present(syncInterval, flags));
}

// スワップチェーンを取得します。
Microsoft::WRL::ComPtr<IDXGISwapChain> SwapChain::GetNativePointer()
{
	return nativePointer;
}
