//=============================================================================
// Graphics.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "Graphics.h"
#include "DirectXHelper.h"

using namespace DX;

// �O���t�B�b�N�X�@�\�����������܂��B
Graphics* Graphics::Create(GameWindow* gameWindow)
{
	Graphics* retVal = nullptr;
	GraphicsDevice* graphicsDevice = nullptr;
	SwapChain* swapChain = nullptr;
	RenderTarget* renderTarget = nullptr;
	DepthStencil* depthStencil = nullptr;
	try {
		// �f�o�C�X�쐬���̃I�v�V�����t���O
		UINT creationFlags = 0;
#if defined(_DEBUG)
		// DEBUG�r���h�̍ۂ�Direct3D�̃f�o�b�O�\���@�\����������
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		// �f�o�C�X�A�f�o�C�X�R���e�L�X�g���쐬
		graphicsDevice = GraphicsDevice::Create();
		// �X���b�v�`�F�[�����쐬
		swapChain = SwapChain::Create(gameWindow, graphicsDevice);
		// �����_�[�^�[�Q�b�g���쐬
		renderTarget = RenderTarget::Create(swapChain);
		// �[�x�X�e���V�����쐬
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

// ���̃N���X�̐V�����C���X�^���X�����������܂��B
Graphics::Graphics(
	GraphicsDevice* graphicsDevice, SwapChain* swapChain,
	RenderTarget* renderTarget, DepthStencil* depthStencil)
{
	this->graphicsDevice = graphicsDevice;
	this->swapChain = swapChain;
	this->renderTarget = renderTarget;
	this->depthStencil = depthStencil;
}

// �f�X�g���N�^�[
Graphics::~Graphics()
{
	SafeDelete(depthStencil);
	SafeDelete(renderTarget);
	SafeDelete(swapChain);
	SafeDelete(graphicsDevice);
}

// �O���t�B�b�N�X �f�o�C�X���擾���܂��B
GraphicsDevice* Graphics::GetGraphicsDevice()
{
	return graphicsDevice;
}

// �����_�[�^�[�Q�b�g���擾���܂��B
RenderTarget* Graphics::GetRenderTarget()
{
	return renderTarget;
}

// �[�x�X�e���V�����擾���܂��B
DepthStencil* Graphics::GetDepthStencil()
{
	return depthStencil;
}

// �X���b�v�`�F�[�����擾���܂��B
SwapChain* Graphics::GetSwapChain()
{
	return swapChain;
}
