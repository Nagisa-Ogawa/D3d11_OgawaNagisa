//=============================================================================
// RenderTarget.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "Graphics.h"
#include "DirectXHelper.h"

using namespace DX;

// ���̃N���X�̃C���X�^���X���쐬���܂��B
RenderTarget* RenderTarget::Create(SwapChain* swapChain)
{
	RenderTarget* retVal = nullptr;
	ID3D11Texture2D* backBuffer = nullptr;
	ID3D11RenderTargetView* view = nullptr;
	ID3D11ShaderResourceView* resourceView = nullptr;
	try {
		auto graphicsDevice = swapChain->GetGraphicsDevice()->GetDevice();
		// �o�b�N�o�b�t�@�[���擾
		ThrowIfFailed(swapChain->GetNativePointer()->GetBuffer(
			0, IID_PPV_ARGS(&backBuffer)));
		// �o�b�N�o�b�t�@�[�ɃA�N�Z�X���邽�߂̃����_�[�^�[�Q�b�g �r���[���쐬
		ThrowIfFailed(graphicsDevice->CreateRenderTargetView(
			backBuffer, NULL, &view));
		// �o�b�N�o�b�t�@�[�ɃV�F�[�_�[����A�N�Z�X���邽�߂̃��\�[�X �r���[���쐬
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

// �C���X�^���X�������֎~
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

// �f�X�g���N�^�[
RenderTarget::~RenderTarget()
{
	SafeRelease(buffer);
	SafeRelease(view);
	SafeRelease(resourceView);
}

// �o�b�N�o�b�t�@�[���擾���܂��B
ID3D11Texture2D* RenderTarget::GetBuffer()
{
	return buffer;
}

// �����_�[�^�[�Q�b�g���擾���܂��B
ID3D11RenderTargetView* RenderTarget::GetView()
{
	return view;
}

// �o�b�N�o�b�t�@�[���V�F�[�_�[�ŗ��p���邽�߂̃��\�[�X �r���[���擾���܂��B
ID3D11ShaderResourceView* RenderTarget::GetResourceView()
{
	return resourceView;
}
