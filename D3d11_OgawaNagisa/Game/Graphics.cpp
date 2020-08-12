//=============================================================================
// Graphics.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "Graphics.h"
#include "DirectXHelper.h"
#include <vector>

using namespace DX;

// �O���t�B�b�N�X�@�\�����������܂��B
std::shared_ptr<Graphics> Graphics::Create(
	std::shared_ptr<GameWindow> gameWindow,
	DXGI_FORMAT backBufferFormt, DXGI_FORMAT depthStencilFormt)
{
	// �f�o�C�X�쐬���̃I�v�V�����t���O
	UINT creationFlags = 0;
#if defined(_DEBUG)
	// DEBUG�r���h�̍ۂ�Direct3D�̃f�o�b�O�\���@�\����������
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	// �f�o�C�X�A�f�o�C�X�R���e�L�X�g���쐬
	auto graphicsDevice = GraphicsDevice::Create();

	// �g�p�\��MSAA���擾
	auto device = graphicsDevice->GetDevice();
	DXGI_SAMPLE_DESC sampleDesc = { 1, 0 };
	for (int index = 0; index <= D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT; index++) {
		UINT quality;
		// �}���`�T���v�����O���ɗ��p�\�ȕi�����x�����擾
		if (SUCCEEDED(device->CheckMultisampleQualityLevels(
			depthStencilFormt, index, &quality))) {
			if (quality > 0) {
				sampleDesc.Count = index;
				sampleDesc.Quality = quality - 1;
			}
		}
	}

	// �X���b�v�`�F�[�����쐬
	auto swapChain = SwapChain::Create(
		gameWindow, graphicsDevice, backBufferFormt, sampleDesc);
	// �����_�[�^�[�Q�b�g���쐬
	auto renderTarget = RenderTarget::Create(swapChain);
	// �[�x�X�e���V�����쐬
	auto depthStencil = DepthStencil::Create(
		renderTarget, depthStencilFormt);

	return std::shared_ptr<Graphics>(
		new Graphics(graphicsDevice, swapChain, renderTarget, depthStencil));
}

// ���̃N���X�̐V�����C���X�^���X�����������܂��B
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

// �O���t�B�b�N�X �f�o�C�X���擾���܂��B
std::shared_ptr<GraphicsDevice> Graphics::GetGraphicsDevice()
{
	return graphicsDevice;
}

// �����_�[�^�[�Q�b�g���擾���܂��B
std::shared_ptr<RenderTarget> Graphics::GetRenderTarget()
{
	return renderTarget;
}

// �[�x�X�e���V�����擾���܂��B
std::shared_ptr<DepthStencil> Graphics::GetDepthStencil()
{
	return depthStencil;
}

// �X���b�v�`�F�[�����擾���܂��B
std::shared_ptr<SwapChain> Graphics::GetSwapChain()
{
	return swapChain;
}
