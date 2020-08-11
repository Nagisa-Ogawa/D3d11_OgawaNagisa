//=============================================================================
// GraphicsDevice.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "Graphics.h"
#include "DirectXHelper.h"

using namespace DX;

// ���̃N���X�̃C���X�^���X���쐬���܂��B
GraphicsDevice* GraphicsDevice::Create()
{
	GraphicsDevice* retVal = nullptr;
	IDXGIFactory1* dxgiFactory = nullptr;
	IDXGIAdapter1* dxgiAdapter = nullptr;
	IDXGIDevice1* dxgiDevice = nullptr;
	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* immediateContext = nullptr;
	try {
		// �f�o�C�X�쐬���̃I�v�V�����t���O
		UINT creationFlags = 0;
#if defined(_DEBUG)
		// DEBUG�r���h�̍ۂ�Direct3D�̃f�o�b�O�\���@�\����������
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		// �f�o�C�X�A�f�o�C�X�R���e�L�X�g���쐬
		D3D_FEATURE_LEVEL featureLevel = {};
		ThrowIfFailed(D3D11CreateDevice(
			NULL, D3D_DRIVER_TYPE_HARDWARE, 0, creationFlags, NULL, 0, D3D11_SDK_VERSION,
			&device, &featureLevel, &immediateContext));

		// DXGI�C���^�[�t�F�C�X���擾
		ThrowIfFailed(
			device->QueryInterface(IID_PPV_ARGS(&dxgiDevice)));
		ThrowIfFailed(dxgiDevice->GetParent(IID_PPV_ARGS(&dxgiAdapter)));
		ThrowIfFailed(dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory)));

		retVal = new GraphicsDevice(
			dxgiFactory, dxgiAdapter, dxgiDevice,
			device, featureLevel, immediateContext);
		SafeRelease(dxgiFactory);
		SafeRelease(dxgiAdapter);
		SafeRelease(dxgiDevice);
		SafeRelease(device);
		SafeRelease(immediateContext);
		return retVal;
	}
	catch (...) {
		SafeDelete(retVal);
		SafeRelease(dxgiFactory);
		SafeRelease(dxgiAdapter);
		SafeRelease(dxgiDevice);
		SafeRelease(device);
		SafeRelease(immediateContext);
		throw;
	}
	return nullptr;
}

// ���̃N���X�̐V�����C���X�^���X�����������܂��B
GraphicsDevice::GraphicsDevice(
	IDXGIFactory1* dxgiFactory,
	IDXGIAdapter1* dxgiAdapter,
	IDXGIDevice1* dxgiDevice,
	ID3D11Device* device, D3D_FEATURE_LEVEL featureLevel,
	ID3D11DeviceContext* immediateContext)
{
	dxgiFactory->AddRef();
	this->dxgiFactory = dxgiFactory;
	dxgiAdapter->AddRef();
	this->dxgiAdapter = dxgiAdapter;
	dxgiDevice->AddRef();
	this->dxgiDevice = dxgiDevice;

	device->AddRef();
	this->device = device;
	this->featureLevel = featureLevel;
	immediateContext->AddRef();
	this->immediateContext = immediateContext;
}

// �f�X�g���N�^�[
GraphicsDevice::~GraphicsDevice()
{
	SafeRelease(dxgiFactory);
	SafeRelease(dxgiAdapter);
	SafeRelease(dxgiDevice);
	SafeRelease(immediateContext);
	SafeRelease(device);
}

// IDXGIFactory1�I�u�W�F�N�g���擾���܂��B
IDXGIFactory1* GraphicsDevice::GetDXGIFactory()
{
	return dxgiFactory;
}

// IDXGIAdapter1�I�u�W�F�N�g���擾���܂��B
IDXGIAdapter1* GraphicsDevice::GetDXGIAdapter()
{
	return dxgiAdapter;
}

// IDXGIDevice1�I�u�W�F�N�g���擾���܂��B
IDXGIDevice1* GraphicsDevice::GetDXGIDevice()
{
	return dxgiDevice;
}

// Direct3D 11�̃f�o�C�X���擾���܂��B
ID3D11Device* GraphicsDevice::GetDevice()
{
	return device;
}

// Direct3D 11�̋@�\���x�����擾���܂��B
D3D_FEATURE_LEVEL GraphicsDevice::GetFeatureLevel() const
{
	return featureLevel;
}

// Direct3D 11�̃f�o�C�X �R���e�L�X�g���擾���܂��B
ID3D11DeviceContext* GraphicsDevice::GetImmediateContext()
{
	return immediateContext;
}

// ���̃N���X�̃C���X�^���X�����������܂��B
GraphicsResource::GraphicsResource(GraphicsDevice* graphicsDevice)
{
	this->graphicsDevice = graphicsDevice;
}

// �f�X�g���N�^�[
GraphicsResource::~GraphicsResource()
{
}

// Direct3D 11�̃f�o�C�X���擾���܂��B
GraphicsDevice* GraphicsResource::GetGraphicsDevice()
{
	return graphicsDevice;
}
