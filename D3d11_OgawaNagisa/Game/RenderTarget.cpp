//=============================================================================
// RenderTarget.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "Graphics.h"
#include "DirectXHelper.h"

using namespace Microsoft::WRL;
using namespace DX;

// このクラスのインスタンスを作成します。
std::shared_ptr<RenderTarget> RenderTarget::Create(
	std::shared_ptr<SwapChain> swapChain)
{
	auto graphicsDevice = swapChain->GetGraphicsDevice()->GetDevice();
	// バックバッファーを取得
	ComPtr<ID3D11Texture2D> backBuffer;
	ThrowIfFailed(swapChain->GetNativePointer()->GetBuffer(
		0, IID_PPV_ARGS(&backBuffer)));
	// バックバッファーにアクセスするためのレンダーターゲット ビューを作成
	ComPtr<ID3D11RenderTargetView> view;
	ThrowIfFailed(graphicsDevice->CreateRenderTargetView(
		backBuffer.Get(), NULL, &view));
	// バックバッファーにシェーダーからアクセスするためのリソース ビューを作成
	ComPtr<ID3D11ShaderResourceView> resourceView;
	ThrowIfFailed(graphicsDevice->CreateShaderResourceView(
		backBuffer.Get(), NULL, &resourceView));

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc = {};
	view->GetDesc(&renderTargetViewDesc);

	return std::shared_ptr<RenderTarget>(
		new RenderTarget(
			swapChain->GetGraphicsDevice(), backBuffer, view, resourceView));
}

// インスタンス生成を禁止
RenderTarget::RenderTarget(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer,
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> view,
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> resourceView)
	: GraphicsResource(graphicsDevice)
{
	this->buffer = backBuffer;
	this->view = view;
	this->resourceView = resourceView;
}

// バックバッファーを取得します。
Microsoft::WRL::ComPtr<ID3D11Texture2D> RenderTarget::GetBuffer()
{
	return buffer;
}

// レンダーターゲットを取得します。
Microsoft::WRL::ComPtr<ID3D11RenderTargetView> RenderTarget::GetView()
{
	return view;
}

// バックバッファーをシェーダーで利用するためのリソース ビューを取得します。
Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> RenderTarget::GetResourceView()
{
	return resourceView;
}
