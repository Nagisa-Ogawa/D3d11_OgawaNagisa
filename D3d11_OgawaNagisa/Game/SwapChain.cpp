//=============================================================================
// SwapChain.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "Graphics.h"
#include "DirectXHelper.h"

using namespace Microsoft::WRL;
using namespace DX;

// ���̃N���X�̃C���X�^���X�����������܂��B
SwapChain::SwapChain(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	std::shared_ptr<GameWindow> window,
	std::shared_ptr<Output> output)
	: GraphicsResource(graphicsDevice)
{
	// ���j�^�[�o�͂̐����ݒ���擾
	auto modeDesc = output->FindClosestMatchingMode(
		window->GetWidth(), window->GetHeight(), graphicsDevice);
	// MSAA�̎g�p�\�ȍő�i�����擾
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
	// �쐬����X���b�v�`�F�[���ɂ��Ă̋L�q
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferDesc = modeDesc;
	swapChainDesc.SampleDesc = sampleDesc;
	swapChainDesc.BufferUsage =
		DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.OutputWindow = window->GetHwnd();
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Windowed = TRUE;
	// �X���b�v�`�F�[�����쐬
	ThrowIfFailed(
		graphicsDevice->GetDXGIFactory()->CreateSwapChain(
			graphicsDevice->GetDevice().Get(), &swapChainDesc, &nativePointer));
}

// �f�B�X�v���C�Ƀ����_�����O�C���[�W��\�����܂��B
void SwapChain::Present(UINT syncInterval, UINT flags)
{
	ThrowIfFailed(nativePointer->Present(syncInterval, flags));
}

// �X���b�v�`�F�[�����擾���܂��B
Microsoft::WRL::ComPtr<IDXGISwapChain> SwapChain::GetNativePointer() const
{
	return nativePointer;
}
