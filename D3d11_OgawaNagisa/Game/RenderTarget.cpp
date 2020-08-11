//=============================================================================
// RenderTarget.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "Graphics.h"
#include "DirectXHelper.h"

using namespace DX;

// このクラスのインスタンスを作成します。
RenderTarget* RenderTarget::Create(SwapChain* swapChain)
{
	RenderTarget* retVal = nullptr;
	ID3D11Texture2D* backBuffer = nullptr;
	ID3D11RenderTargetView* view = nullptr;
	ID3D11ShaderResourceView* resourceView = nullptr;
	try {
		auto graphicsDevice = swapChain->GetGraphicsDevice()->GetDevice();
		// バックバッファーを取得
		ThrowIfFailed(swapChain->GetNativePointer()->GetBuffer(
			0, IID_PPV_ARGS(&backBuffer)));
		// バックバッファーにアクセスするためのレンダーターゲット ビューを作成
		ThrowIfFailed(graphicsDevice->CreateRenderTargetView(
			backBuffer, NULL, &view));
		// バックバッファーにシェーダーからアクセスするためのリソース ビューを作成
		ThrowIfFailed(graphicsDevice->CreateShaderResourceView(
			backBuffer, NULL, &resourceView));

		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc = {};
		view->GetDesc(&renderTargetViewDesc);

		retVal = new RenderTarget(
			swapChain->GetGraphicsDevice(), backBuffer, view, resourceView);
		SafeRelease(backBuffer);
		SafeRelease(view);
		SafeRelease(resourceView);

		return retVal;
	}
	catch (...) {
		SafeRelease(backBuffer);
		SafeRelease(view);
		SafeRelease(resourceView);
		SafeDelete(retVal);
		throw;
	}
	return nullptr;
}

// インスタンス生成を禁止
RenderTarget::RenderTarget(
	GraphicsDevice* graphicsDevice,
	ID3D11Texture2D* backBuffer,
	ID3D11RenderTargetView* view,
	ID3D11ShaderResourceView* resourceView)
	: GraphicsResource(graphicsDevice)
{
	backBuffer->AddRef();
	this->buffer = backBuffer;
	view->AddRef();
	this->view = view;
	resourceView->AddRef();
	this->resourceView = resourceView;
}

// デストラクター
RenderTarget::~RenderTarget()
{
	SafeRelease(buffer);
	SafeRelease(view);
	SafeRelease(resourceView);
}

// バックバッファーを取得します。
ID3D11Texture2D* RenderTarget::GetBuffer()
{
	return buffer;
}

// レンダーターゲットを取得します。
ID3D11RenderTargetView* RenderTarget::GetView()
{
	return view;
}

// バックバッファーをシェーダーで利用するためのリソース ビューを取得します。
ID3D11ShaderResourceView* RenderTarget::GetResourceView()
{
	return resourceView;
}
