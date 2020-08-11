//=============================================================================
// Graphics.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "Graphics.h"
#include "DirectXHelper.h"

using namespace DX;

// グラフィックス機能を初期化します。
Graphics* Graphics::Create(GameWindow* gameWindow)
{
	Graphics* retVal = nullptr;
	GraphicsDevice* graphicsDevice = nullptr;
	SwapChain* swapChain = nullptr;
	RenderTarget* renderTarget = nullptr;
	DepthStencil* depthStencil = nullptr;
	try {
		// デバイス作成時のオプションフラグ
		UINT creationFlags = 0;
#if defined(_DEBUG)
		// DEBUGビルドの際にDirect3Dのデバッグ表示機能を持たせる
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		// デバイス、デバイスコンテキストを作成
		graphicsDevice = GraphicsDevice::Create();
		// スワップチェーンを作成
		swapChain = SwapChain::Create(gameWindow, graphicsDevice);
		// レンダーターゲットを作成
		renderTarget = RenderTarget::Create(swapChain);
		// 深度ステンシルを作成
		depthStencil = DepthStencil::Create(renderTarget, DXGI_FORMAT_D32_FLOAT_S8X24_UINT);

		retVal = new Graphics(graphicsDevice, swapChain, renderTarget, depthStencil);
		return retVal;
	}
	catch (...) {
		SafeDelete(retVal);
		SafeDelete(graphicsDevice);
		SafeDelete(swapChain);
		SafeDelete(renderTarget);
		SafeDelete(depthStencil);
		throw;
	}
	return nullptr;
}

// このクラスの新しいインスタンスを初期化します。
Graphics::Graphics(
	GraphicsDevice* graphicsDevice, SwapChain* swapChain,
	RenderTarget* renderTarget, DepthStencil* depthStencil)
{
	this->graphicsDevice = graphicsDevice;
	this->swapChain = swapChain;
	this->renderTarget = renderTarget;
	this->depthStencil = depthStencil;
}

// デストラクター
Graphics::~Graphics()
{
	SafeDelete(depthStencil);
	SafeDelete(renderTarget);
	SafeDelete(swapChain);
	SafeDelete(graphicsDevice);
}

// グラフィックス デバイスを取得します。
GraphicsDevice* Graphics::GetGraphicsDevice()
{
	return graphicsDevice;
}

// レンダーターゲットを取得します。
RenderTarget* Graphics::GetRenderTarget()
{
	return renderTarget;
}

// 深度ステンシルを取得します。
DepthStencil* Graphics::GetDepthStencil()
{
	return depthStencil;
}

// スワップチェーンを取得します。
SwapChain* Graphics::GetSwapChain()
{
	return swapChain;
}
