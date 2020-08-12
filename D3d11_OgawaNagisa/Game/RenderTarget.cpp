//=============================================================================
// RenderTarget.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "Graphics.h"
#include "DirectXHelper.h"

using namespace Microsoft::WRL;
using namespace DX;

// ���̃N���X�̃C���X�^���X���쐬���܂��B
std::shared_ptr<RenderTarget> RenderTarget::Create(
	std::shared_ptr<SwapChain> swapChain)
{
	auto graphicsDevice = swapChain->GetGraphicsDevice()->GetDevice();
	// �o�b�N�o�b�t�@�[���擾
	ComPtr<ID3D11Texture2D> backBuffer;
	ThrowIfFailed(swapChain->GetNativePointer()->GetBuffer(
		0, IID_PPV_ARGS(&backBuffer)));
	// �o�b�N�o�b�t�@�[�ɃA�N�Z�X���邽�߂̃����_�[�^�[�Q�b�g �r���[���쐬
	ComPtr<ID3D11RenderTargetView> view;
	ThrowIfFailed(graphicsDevice->CreateRenderTargetView(
		backBuffer.Get(), NULL, &view));
	// �o�b�N�o�b�t�@�[�ɃV�F�[�_�[����A�N�Z�X���邽�߂̃��\�[�X �r���[���쐬
	ComPtr<ID3D11ShaderResourceView> resourceView;
	ThrowIfFailed(graphicsDevice->CreateShaderResourceView(
		backBuffer.Get(), NULL, &resourceView));

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc = {};
	view->GetDesc(&renderTargetViewDesc);

	return std::shared_ptr<RenderTarget>(
		new RenderTarget(
			swapChain->GetGraphicsDevice(), backBuffer, view, resourceView));
}

// �C���X�^���X�������֎~
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

// �o�b�N�o�b�t�@�[���擾���܂��B
Microsoft::WRL::ComPtr<ID3D11Texture2D> RenderTarget::GetBuffer()
{
	return buffer;
}

// �����_�[�^�[�Q�b�g���擾���܂��B
Microsoft::WRL::ComPtr<ID3D11RenderTargetView> RenderTarget::GetView()
{
	return view;
}

// �o�b�N�o�b�t�@�[���V�F�[�_�[�ŗ��p���邽�߂̃��\�[�X �r���[���擾���܂��B
Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> RenderTarget::GetResourceView()
{
	return resourceView;
}
