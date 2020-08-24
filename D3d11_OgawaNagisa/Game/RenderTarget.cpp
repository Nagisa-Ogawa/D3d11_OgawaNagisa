//=============================================================================
// RenderTarget.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "Graphics.h"
#include "DirectXHelper.h"

using namespace DX;

// このクラスのインスタンスを初期化します。
RenderTarget::RenderTarget(std::shared_ptr<SwapChain> swapChain)
	: GraphicsResource(swapChain->GetGraphicsDevice())
{
	auto graphicsDevice = swapChain->GetGraphicsDevice()->GetDevice();
	// バックバッファーを取得
	ThrowIfFailed(swapChain->GetNativePointer()->GetBuffer(
		0, IID_PPV_ARGS(&buffer)));
	// バックバッファーにアクセスするためのレンダーターゲット ビューを作成
	ThrowIfFailed(graphicsDevice->CreateRenderTargetView(
		buffer.Get(), NULL, &view));
	// バックバッファーにシェーダーからアクセスするためのリソース ビューを作成
	ThrowIfFailed(graphicsDevice->CreateShaderResourceView(
		buffer.Get(), NULL, &resourceView));

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc = {};
	view->GetDesc(&renderTargetViewDesc);
}

// バックバッファーを取得します。
Microsoft::WRL::ComPtr<ID3D11Texture2D> RenderTarget::GetBuffer() const
{
	return buffer;
}

// レンダーターゲットを取得します。
Microsoft::WRL::ComPtr<ID3D11RenderTargetView> RenderTarget::GetView() const
{
	return view;
}

// バックバッファーをシェーダーで利用するためのリソース ビューを取得します。
Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> RenderTarget::GetResourceView() const
{
	return resourceView;
}
