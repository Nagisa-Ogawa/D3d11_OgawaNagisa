//=============================================================================
// DepthStencil.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "Graphics.h"
#include "DirectXHelper.h"

using namespace DX;

// ���̃N���X�̃C���X�^���X�����������܂��B
DepthStencil::DepthStencil(
	std::shared_ptr<RenderTarget> renderTarget, DXGI_FORMAT format)
	: GraphicsResource(renderTarget->GetGraphicsDevice())
{
	auto graphicsDevice = renderTarget->GetGraphicsDevice();

	// �e�N�X�`���[�ƃV�F�[�_�[���\�[�X�r���[�̃t�H�[�}�b�g��ݒ�
	DXGI_FORMAT textureFormat = DXGI_FORMAT_UNKNOWN;
	DXGI_FORMAT resourceFormat = DXGI_FORMAT_UNKNOWN;
	switch (format)
	{
	case DXGI_FORMAT_D16_UNORM:
		textureFormat = DXGI_FORMAT_R16_TYPELESS;
		resourceFormat = DXGI_FORMAT_R16_UNORM;
		break;
	case DXGI_FORMAT_D24_UNORM_S8_UINT:
		textureFormat = DXGI_FORMAT_R24G8_TYPELESS;
		resourceFormat = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		break;
	case DXGI_FORMAT_D32_FLOAT:
		textureFormat = DXGI_FORMAT_R32_TYPELESS;
		resourceFormat = DXGI_FORMAT_R32_FLOAT;
		break;
	case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
		textureFormat = DXGI_FORMAT_R32G8X24_TYPELESS;
		resourceFormat = DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;
		break;
	}
	// 
	D3D11_TEXTURE2D_DESC backBufferDesc = {};
	renderTarget->GetBuffer()->GetDesc(&backBufferDesc);
	// �[�x�X�e���V�����쐬
	D3D11_TEXTURE2D_DESC depthStencilDesc = {};
	depthStencilDesc.Width = backBufferDesc.Width;
	depthStencilDesc.Height = backBufferDesc.Height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = textureFormat;
	depthStencilDesc.SampleDesc = backBufferDesc.SampleDesc;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags =
		D3D11_BIND_DEPTH_STENCIL |
		D3D11_BIND_SHADER_RESOURCE;	// �V�F�[�_�[���\�[�X�Ƃ��Ďg�p���邱�Ƃ�ݒ�
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;
	ThrowIfFailed(graphicsDevice->GetDevice()->CreateTexture2D(&depthStencilDesc, NULL, &buffer));
	// �[�x�X�e���V���ɃA�N�Z�X���邽�߂̃r���[���쐬
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
	depthStencilViewDesc.Format = format;
	if (depthStencilDesc.SampleDesc.Count > 0) {
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	}
	else {
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;
	}
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	ThrowIfFailed(graphicsDevice->GetDevice()->CreateDepthStencilView(
		buffer.Get(), &depthStencilViewDesc, &view));
	// �[�x�X�e���V���ɃV�F�[�_�[����A�N�Z�X���邽�߂̃��\�[�X �r���[���쐬
	D3D11_SHADER_RESOURCE_VIEW_DESC depthStencilResourceViewDesc = {};
	depthStencilResourceViewDesc.Format = resourceFormat;
	if (depthStencilDesc.SampleDesc.Count > 0) {
		depthStencilResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
	}
	else {
		depthStencilResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		depthStencilResourceViewDesc.Texture2D.MostDetailedMip = 0;
		depthStencilResourceViewDesc.Texture2D.MipLevels = 1;
	}
	ThrowIfFailed(graphicsDevice->GetDevice()->CreateShaderResourceView(
		buffer.Get(), &depthStencilResourceViewDesc, &resourceView));
}

// �[�x�X�e���V���o�b�t�@�[���擾���܂��B
Microsoft::WRL::ComPtr<ID3D11Texture2D> DepthStencil::GetBuffer() const
{
	return buffer;
}

// �[�x�X�e���V�� �r���[���擾���܂��B
Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DepthStencil::GetView() const
{
	return view;
}

// �[�x�X�e���V�����V�F�[�_�[�ŗ��p���邽�߂̃��\�[�X �r���[���擾���܂��B
Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> DepthStencil::GetResourceView() const
{
	return resourceView;
}
