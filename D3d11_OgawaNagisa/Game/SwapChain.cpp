//=============================================================================
// SwapChain.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "Graphics.h"
#include "DirectXHelper.h"

using namespace DX;

// このクラスのインスタンスを作成します。
SwapChain* SwapChain::Create(GameWindow* window, GraphicsDevice* graphicsDevice)
{
	SwapChain* retVal = nullptr;
	IDXGISwapChain* swapChain = nullptr;
	try {
		// 作成するスワップチェーンについての記述
		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
		swapChainDesc.BufferDesc.Width = window->GetWidth();
		swapChainDesc.BufferDesc.Height = window->GetHeight();
		swapChainDesc.BufferDesc.RefreshRate = { 60, 1 };
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.SampleDesc = { 1, 0 };
		swapChainDesc.BufferUsage =
			DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
		swapChainDesc.BufferCount = 2;
		swapChainDesc.OutputWindow = window->GetHwnd();
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapChainDesc.Windowed = TRUE;
		// スワップチェーンを作成
		ThrowIfFailed(
			graphicsDevice->GetDXGIFactory()->CreateSwapChain(
				graphicsDevice->GetDevice(), &swapChainDesc, &swapChain));

		retVal = new SwapChain(graphicsDevice, swapChain);
		SafeRelease(swapChain);
		return retVal;
	}
	catch (...) {
		SafeDelete(retVal);
		SafeRelease(swapChain);
	}
	return nullptr;
}

// インスタンス生成を禁止
SwapChain::SwapChain(GraphicsDevice* graphicsDevice, IDXGISwapChain* swapChain)
	: GraphicsResource(graphicsDevice)
{
	swapChain->AddRef();
	this->nativePointer = swapChain;
}

// デストラクター
SwapChain::~SwapChain()
{
	SafeRelease(nativePointer);
}

// ディスプレイにレンダリングイメージを表示します。
void SwapChain::Present(UINT syncInterval, UINT flags)
{
	ThrowIfFailed(nativePointer->Present(syncInterval, flags));
}

// スワップチェーンを取得します。
IDXGISwapChain* SwapChain::GetNativePointer()
{
	return nativePointer;
}
