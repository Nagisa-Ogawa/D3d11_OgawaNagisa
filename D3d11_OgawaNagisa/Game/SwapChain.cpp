//=============================================================================
// SwapChain.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "Graphics.h"
#include "DirectXHelper.h"

using namespace Microsoft::WRL;
using namespace DX;

// ���̃N���X�̃C���X�^���X���쐬���܂��B
std::shared_ptr<SwapChain> SwapChain::Create(
	std::shared_ptr<GameWindow> window,
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	DXGI_FORMAT format, const DXGI_SAMPLE_DESC& sampleDesc)
{
	// �쐬����X���b�v�`�F�[���ɂ��Ă̋L�q
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
	// �X���b�v�`�F�[�����쐬
	ComPtr<IDXGISwapChain> swapChain;
	ThrowIfFailed(
		graphicsDevice->GetDXGIFactory()->CreateSwapChain(
			graphicsDevice->GetDevice().Get(), &swapChainDesc, &swapChain));

	return std::shared_ptr<SwapChain>(
		new SwapChain(graphicsDevice, swapChain));
}

// �C���X�^���X�������֎~
SwapChain::SwapChain(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain)
	: GraphicsResource(graphicsDevice)
{
	this->nativePointer = swapChain;
}

// �f�B�X�v���C�Ƀ����_�����O�C���[�W��\�����܂��B
void SwapChain::Present(UINT syncInterval, UINT flags)
{
	ThrowIfFailed(nativePointer->Present(syncInterval, flags));
}

// �X���b�v�`�F�[�����擾���܂��B
Microsoft::WRL::ComPtr<IDXGISwapChain> SwapChain::GetNativePointer()
{
	return nativePointer;
}
