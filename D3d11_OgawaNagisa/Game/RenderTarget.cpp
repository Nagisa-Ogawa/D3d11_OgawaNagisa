//=============================================================================
// RenderTarget.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "Graphics.h"
#include "DirectXHelper.h"

using namespace DX;

// ���̃N���X�̃C���X�^���X�����������܂��B
RenderTarget::RenderTarget(std::shared_ptr<SwapChain> swapChain)
	: GraphicsResource(swapChain->GetGraphicsDevice())
{
	auto graphicsDevice = swapChain->GetGraphicsDevice()->GetDevice();
	// �o�b�N�o�b�t�@�[���擾
	ThrowIfFailed(swapChain->GetNativePointer()->GetBuffer(
		0, IID_PPV_ARGS(&buffer)));
	// �o�b�N�o�b�t�@�[�ɃA�N�Z�X���邽�߂̃����_�[�^�[�Q�b�g �r���[���쐬
	ThrowIfFailed(graphicsDevice->CreateRenderTargetView(
		buffer.Get(), NULL, &view));
	// �o�b�N�o�b�t�@�[�ɃV�F�[�_�[����A�N�Z�X���邽�߂̃��\�[�X �r���[���쐬
	ThrowIfFailed(graphicsDevice->CreateShaderResourceView(
		buffer.Get(), NULL, &resourceView));

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc = {};
	view->GetDesc(&renderTargetViewDesc);
}

// �o�b�N�o�b�t�@�[���擾���܂��B
Microsoft::WRL::ComPtr<ID3D11Texture2D> RenderTarget::GetBuffer() const
{
	return buffer;
}

// �����_�[�^�[�Q�b�g���擾���܂��B
Microsoft::WRL::ComPtr<ID3D11RenderTargetView> RenderTarget::GetView() const
{
	return view;
}

// �o�b�N�o�b�t�@�[���V�F�[�_�[�ŗ��p���邽�߂̃��\�[�X �r���[���擾���܂��B
Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> RenderTarget::GetResourceView() const
{
	return resourceView;
}
