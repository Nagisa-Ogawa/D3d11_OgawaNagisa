//=============================================================================
// SwapChain.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "Graphics.h"
#include "DirectXHelper.h"

using namespace Microsoft::WRL;
using namespace DX;

// このクラスのインスタンスを初期化します。
SwapChain::SwapChain(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	std::shared_ptr<GameWindow> window,
	std::shared_ptr<Output> output)
	: GraphicsResource(graphicsDevice)
{
	// モニター出力の推奨設定を取得
	auto modeDesc = output->FindClosestMatchingMode(
		window->GetWidth(), window->GetHeight(), graphicsDevice);
	// MSAAの使用可能な最大品質を取得
	DXGI_SAMPLE_DESC sampleDesc = { 1, 0 };
	for (int index = 0; index <= D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT; index++) {
		UINT quality;
		if (SUCCEEDED(graphicsDevice->GetDevice()->CheckMultisampleQualityLevels(
			modeDesc.Format, index, &quality))) {
			if (quality > 0) {
				sampleDesc.Count = index;
				sampleDesc.Quality = quality - 1;
			}
		}
	}
	// 作成するスワップチェーンについての記述
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferDesc = modeDesc;
	swapChainDesc.SampleDesc = sampleDesc;
	swapChainDesc.BufferUsage =
		DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.OutputWindow = window->GetHwnd();
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Windowed = TRUE;
	// スワップチェーンを作成
	ThrowIfFailed(
		graphicsDevice->GetDXGIFactory()->CreateSwapChain(
			graphicsDevice->GetDevice().Get(), &swapChainDesc, &nativePointer));
}

// ディスプレイにレンダリングイメージを表示します。
void SwapChain::Present(UINT syncInterval, UINT flags)
{
	ThrowIfFailed(nativePointer->Present(syncInterval, flags));
}

// スワップチェーンを取得します。
Microsoft::WRL::ComPtr<IDXGISwapChain> SwapChain::GetNativePointer() const
{
	return nativePointer;
}
