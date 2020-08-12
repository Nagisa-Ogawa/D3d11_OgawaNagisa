//=============================================================================
// GraphicsDevice.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "Graphics.h"
#include "DirectXHelper.h"

using namespace Microsoft::WRL;
using namespace DX;

// ���̃N���X�̃C���X�^���X���쐬���܂��B
std::shared_ptr<GraphicsDevice> GraphicsDevice::Create()
{
	// �f�o�C�X�쐬���̃I�v�V�����t���O
	UINT creationFlags = 0;
#if defined(_DEBUG)
	// DEBUG�r���h�̍ۂ�Direct3D�̃f�o�b�O�\���@�\����������
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	// �f�o�C�X�A�f�o�C�X�R���e�L�X�g���쐬
	ComPtr<ID3D11Device> device;
	D3D_FEATURE_LEVEL featureLevel = {};
	ComPtr<ID3D11DeviceContext> immediateContext;
	ThrowIfFailed(D3D11CreateDevice(
		NULL, D3D_DRIVER_TYPE_HARDWARE, 0, creationFlags, NULL, 0, D3D11_SDK_VERSION,
		&device, &featureLevel, &immediateContext));

	// DXGI�C���^�[�t�F�C�X���擾
	ComPtr<IDXGIFactory1> dxgiFactory;
	ComPtr<IDXGIAdapter1> dxgiAdapter;
	ComPtr<IDXGIDevice1> dxgiDevice;
	ThrowIfFailed(device.As(&dxgiDevice));
	ThrowIfFailed(dxgiDevice->GetParent(IID_PPV_ARGS(&dxgiAdapter)));
	ThrowIfFailed(dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory)));

	return std::shared_ptr<GraphicsDevice>(
		new GraphicsDevice(
			dxgiFactory, dxgiAdapter, dxgiDevice,
			device, featureLevel, immediateContext));
}

// ���̃N���X�̐V�����C���X�^���X�����������܂��B
GraphicsDevice::GraphicsDevice(
	Microsoft::WRL::ComPtr<IDXGIFactory1> dxgiFactory,
	Microsoft::WRL::ComPtr<IDXGIAdapter1> dxgiAdapter,
	Microsoft::WRL::ComPtr<IDXGIDevice1> dxgiDevice,
	Microsoft::WRL::ComPtr<ID3D11Device> device,
	D3D_FEATURE_LEVEL featureLevel,
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediateContext)
{
	this->dxgiFactory = dxgiFactory;
	this->dxgiAdapter = dxgiAdapter;
	this->dxgiDevice = dxgiDevice;
	this->device = device;
	this->featureLevel = featureLevel;
	this->immediateContext = immediateContext;
}

// IDXGIFactory1�I�u�W�F�N�g���擾���܂��B
Microsoft::WRL::ComPtr<IDXGIFactory1> GraphicsDevice::GetDXGIFactory()
{
	return dxgiFactory;
}

// IDXGIAdapter1�I�u�W�F�N�g���擾���܂��B
Microsoft::WRL::ComPtr<IDXGIAdapter1> GraphicsDevice::GetDXGIAdapter()
{
	return dxgiAdapter;
}

// IDXGIDevice1�I�u�W�F�N�g���擾���܂��B
Microsoft::WRL::ComPtr<IDXGIDevice1> GraphicsDevice::GetDXGIDevice()
{
	return dxgiDevice;
}

// Direct3D 11�̃f�o�C�X���擾���܂��B
Microsoft::WRL::ComPtr<ID3D11Device> GraphicsDevice::GetDevice()
{
	return device;
}

// Direct3D 11�̋@�\���x�����擾���܂��B
D3D_FEATURE_LEVEL GraphicsDevice::GetFeatureLevel() const
{
	return featureLevel;
}

// Direct3D 11�̃f�o�C�X �R���e�L�X�g���擾���܂��B
Microsoft::WRL::ComPtr<ID3D11DeviceContext> GraphicsDevice::GetImmediateContext()
{
	return immediateContext;
}

// ���̃N���X�̃C���X�^���X�����������܂��B
GraphicsResource::GraphicsResource(
	std::shared_ptr<GraphicsDevice> graphicsDevice)
{
	this->graphicsDevice = graphicsDevice;
}

// Direct3D 11�̃f�o�C�X���擾���܂��B
std::shared_ptr<GraphicsDevice> GraphicsResource::GetGraphicsDevice()
{
	return graphicsDevice;
}
