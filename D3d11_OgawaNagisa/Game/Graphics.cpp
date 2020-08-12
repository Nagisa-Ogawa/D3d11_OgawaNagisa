//=============================================================================
// Graphics.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "Graphics.h"
#include "DirectXHelper.h"
#include <vector>

using namespace DX;

// グラフィックス機能を初期化します。
std::shared_ptr<Graphics> Graphics::Create(
	std::shared_ptr<GameWindow> gameWindow,
	DXGI_FORMAT backBufferFormt, DXGI_FORMAT depthStencilFormt)
{
	// デバイス作成時のオプションフラグ
	UINT creationFlags = 0;
#if defined(_DEBUG)
	// DEBUGビルドの際にDirect3Dのデバッグ表示機能を持たせる
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	// デバイス、デバイスコンテキストを作成
	auto graphicsDevice = GraphicsDevice::Create();

	// 使用可能なMSAAを取得
	auto device = graphicsDevice->GetDevice();
	DXGI_SAMPLE_DESC sampleDesc = { 1, 0 };
	for (int index = 0; index <= D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT; index++) {
		UINT quality;
		// マルチサンプリング中に利用可能な品質レベルを取得
		if (SUCCEEDED(device->CheckMultisampleQualityLevels(
			depthStencilFormt, index, &quality))) {
			if (quality > 0) {
				sampleDesc.Count = index;
				sampleDesc.Quality = quality - 1;
			}
		}
	}

	// スワップチェーンを作成
	auto swapChain = SwapChain::Create(
		gameWindow, graphicsDevice, backBufferFormt, sampleDesc);
	// レンダーターゲットを作成
	auto renderTarget = RenderTarget::Create(swapChain);
	// 深度ステンシルを作成
	auto depthStencil = DepthStencil::Create(
		renderTarget, depthStencilFormt);

	return std::shared_ptr<Graphics>(
		new Graphics(graphicsDevice, swapChain, renderTarget, depthStencil));
}

// このクラスの新しいインスタンスを初期化します。
Graphics::Graphics(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	std::shared_ptr<SwapChain> swapChain,
	std::shared_ptr<RenderTarget> renderTarget,
	std::shared_ptr<DepthStencil> depthStencil)
{
	this->graphicsDevice = graphicsDevice;
	this->swapChain = swapChain;
	this->renderTarget = renderTarget;
	this->depthStencil = depthStencil;
}

// グラフィックス デバイスを取得します。
std::shared_ptr<GraphicsDevice> Graphics::GetGraphicsDevice()
{
	return graphicsDevice;
}

// レンダーターゲットを取得します。
std::shared_ptr<RenderTarget> Graphics::GetRenderTarget()
{
	return renderTarget;
}

// 深度ステンシルを取得します。
std::shared_ptr<DepthStencil> Graphics::GetDepthStencil()
{
	return depthStencil;
}

// スワップチェーンを取得します。
std::shared_ptr<SwapChain> Graphics::GetSwapChain()
{
	return swapChain;
}
