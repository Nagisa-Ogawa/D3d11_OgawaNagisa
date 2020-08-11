//=============================================================================
// SwapChain.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "Graphics.h"
#include "DirectXHelper.h"

using namespace DX;

// ���̃N���X�̃C���X�^���X���쐬���܂��B
SwapChain* SwapChain::Create(GameWindow* window, GraphicsDevice* graphicsDevice)
{
	SwapChain* retVal = nullptr;
	IDXGISwapChain* swapChain = nullptr;
	try {
		// �쐬����X���b�v�`�F�[���ɂ��Ă̋L�q
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
		// �X���b�v�`�F�[�����쐬
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

// �C���X�^���X�������֎~
SwapChain::SwapChain(GraphicsDevice* graphicsDevice, IDXGISwapChain* swapChain)
	: GraphicsResource(graphicsDevice)
{
	swapChain->AddRef();
	this->nativePointer = swapChain;
}

// �f�X�g���N�^�[
SwapChain::~SwapChain()
{
	SafeRelease(nativePointer);
}

// �f�B�X�v���C�Ƀ����_�����O�C���[�W��\�����܂��B
void SwapChain::Present(UINT syncInterval, UINT flags)
{
	ThrowIfFailed(nativePointer->Present(syncInterval, flags));
}

// �X���b�v�`�F�[�����擾���܂��B
IDXGISwapChain* SwapChain::GetNativePointer()
{
	return nativePointer;
}
