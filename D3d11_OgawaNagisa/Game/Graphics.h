//=============================================================================
// Graphics.h
// 
//=============================================================================
#pragma once

#include <d3d11.h>
#include "GameWindow.h"

// �O���t�B�b�N�X�f�o�C�X��\���܂��B
class GraphicsDevice final
{
public:
	// ���̃N���X�̃C���X�^���X���쐬���܂��B
	static GraphicsDevice* Create();

	// IDXGIFactory1�I�u�W�F�N�g���擾���܂��B
	IDXGIFactory1* GetDXGIFactory();
	// IDXGIAdapter1�I�u�W�F�N�g���擾���܂��B
	IDXGIAdapter1* GetDXGIAdapter();
	// IDXGIDevice1�I�u�W�F�N�g���擾���܂��B
	IDXGIDevice1* GetDXGIDevice();
	// Direct3D 11�̃f�o�C�X���擾���܂��B
	ID3D11Device* GetDevice();
	// Direct3D 11�̋@�\���x�����擾���܂��B
	D3D_FEATURE_LEVEL GetFeatureLevel() const;
	// Direct3D 11�̃f�o�C�X �R���e�L�X�g���擾���܂��B
	ID3D11DeviceContext* GetImmediateContext();

	// �f�X�g���N�^�[
	~GraphicsDevice();

private:
	// �C���X�^���X�������֎~
	GraphicsDevice(
		IDXGIFactory1* dxgiFactory,
		IDXGIAdapter1* dxgiAdapter,
		IDXGIDevice1* dxgiDevice,
		ID3D11Device* device, D3D_FEATURE_LEVEL featureLevel,
		ID3D11DeviceContext* immediateContext);
	// �R�s�[�Ƒ�����Z���֎~
	GraphicsDevice(const GraphicsDevice&) {}
	GraphicsDevice& operator=(const GraphicsDevice&) { return *this; }

	// IDXGIFactory1�I�u�W�F�N�g
	IDXGIFactory1* dxgiFactory = nullptr;
	// IDXGIAdapter1�I�u�W�F�N�g
	IDXGIAdapter1* dxgiAdapter = nullptr;
	// IDXGIDevice1�I�u�W�F�N�g
	IDXGIDevice1* dxgiDevice = nullptr;
	// ID3D11Device�I�u�W�F�N�g
	ID3D11Device* device = nullptr;
	// Direct3D 11�̋@�\���x��
	D3D_FEATURE_LEVEL featureLevel = {};
	// Direct3D 11�̃f�o�C�X �R���e�L�X�g
	ID3D11DeviceContext* immediateContext = nullptr;
};

// �O���t�B�b�N�X ���\�[�X�̊�{�N���X��\���܂��B
class GraphicsResource
{
public:
	// Direct3D 11�̃f�o�C�X���擾���܂��B
	virtual GraphicsDevice* GetGraphicsDevice() final;

	// �f�X�g���N�^�[
	virtual ~GraphicsResource();

protected:
	// �C���X�^���X�������֎~
	GraphicsResource(GraphicsDevice* graphicsDevice);
	// �R�s�[���֎~
	GraphicsResource(const GraphicsResource&) {}
private:
	// ������Z���֎~
	GraphicsResource& operator=(const GraphicsResource&) { return *this; }

	// ���̃��\�[�X���쐬����ID3D11Device�I�u�W�F�N�g
	GraphicsDevice* graphicsDevice = nullptr;
};

// �X���b�v�`�F�[����\���܂��B
class SwapChain : public GraphicsResource
{
public:
	// ���̃N���X�̃C���X�^���X���쐬���܂��B
	static SwapChain* Create(GameWindow* window, GraphicsDevice* graphicsDevice);

	// �f�B�X�v���C�Ƀ����_�����O�C���[�W��\�����܂��B
	void Present(UINT syncInterval, UINT flags);
	// �X���b�v�`�F�[�����擾���܂��B
	IDXGISwapChain* GetNativePointer();

	// �f�X�g���N�^�[
	~SwapChain();

private:
	// �C���X�^���X�������֎~
	SwapChain(GraphicsDevice* graphicsDevice, IDXGISwapChain* swapChain);
	// �R�s�[���֎~
	SwapChain(const SwapChain& other) : GraphicsResource(other) {}
	// ������Z���֎~
	SwapChain& operator=(const SwapChain&) { return *this; }

	// �X���b�v�`�F�[��
	IDXGISwapChain* nativePointer = nullptr;
};

// �����_�[�^�[�Q�b�g��\���܂��B
class RenderTarget final : public GraphicsResource
{
public:
	// ���̃N���X�̃C���X�^���X���쐬���܂��B
	static RenderTarget* Create(SwapChain* swapChain);

	// �f�X�g���N�^�[
	~RenderTarget();

	// �o�b�N�o�b�t�@�[���擾���܂��B
	ID3D11Texture2D* GetBuffer();
	// �����_�[�^�[�Q�b�g���擾���܂��B
	ID3D11RenderTargetView* GetView();
	// �o�b�N�o�b�t�@�[���V�F�[�_�[�ŗ��p���邽�߂̃��\�[�X �r���[���擾���܂��B
	ID3D11ShaderResourceView* GetResourceView();

private:
	// �o�b�N�o�b�t�@�[
	ID3D11Texture2D* buffer = nullptr;
	// �����_�[�^�[�Q�b�g
	ID3D11RenderTargetView* view = nullptr;
	// �o�b�N�o�b�t�@�[���V�F�[�_�[�ŗ��p���邽�߂̃��\�[�X �r���[
	ID3D11ShaderResourceView* resourceView = nullptr;

	// �C���X�^���X�������֎~
	RenderTarget(
		GraphicsDevice* graphicsDevice,
		ID3D11Texture2D* backBuffer,
		ID3D11RenderTargetView* view,
		ID3D11ShaderResourceView* resourceView);
	// �R�s�[�Ƒ�����Z���֎~
	RenderTarget(const RenderTarget& other) : GraphicsResource(other) {}
	RenderTarget& operator=(const RenderTarget&) { return *this; }
};

// �[�x�X�e���V����\���܂��B
class DepthStencil final : public GraphicsResource
{
public:
	// ���̃N���X�̃C���X�^���X���쐬���܂��B
	static DepthStencil* Create(RenderTarget* renderTarget, DXGI_FORMAT format);

	// �f�X�g���N�^�[
	~DepthStencil();

	// �[�x�X�e���V���o�b�t�@�[���擾���܂��B
	ID3D11Texture2D* GetBuffer();
	// �����_�[�^�[�Q�b�g���擾���܂��B
	ID3D11DepthStencilView* GetView();
	// �o�b�N�o�b�t�@�[���V�F�[�_�[�ŗ��p���邽�߂̃��\�[�X �r���[���擾���܂��B
	ID3D11ShaderResourceView* GetResourceView();

private:
	// �o�b�t�@�[
	ID3D11Texture2D* buffer = nullptr;
	// �[�x�X�e���V��
	ID3D11DepthStencilView* view = nullptr;
	// �[�x�X�e���V�����V�F�[�_�[�ŗ��p���邽�߂̃��\�[�X �r���[
	ID3D11ShaderResourceView* resourceView = nullptr;

	// �C���X�^���X�������֎~
	DepthStencil(
		GraphicsDevice* graphicsDevice,
		ID3D11Texture2D* buffer,
		ID3D11DepthStencilView* view,
		ID3D11ShaderResourceView* resourceView);
	// �R�s�[�Ƒ�����Z���֎~
	DepthStencil(const DepthStencil& other) : GraphicsResource(other) {}
	DepthStencil& operator=(const DepthStencil&) { return *this; }
};

// �O���t�B�b�N�X�@�\��\���܂��B
class Graphics final
{
public:
	// �O���t�B�b�N�X�@�\�����������܂��B
	static Graphics* Create(GameWindow* gameWindow);

	// �O���t�B�b�N�X �f�o�C�X���擾���܂��B
	GraphicsDevice* GetGraphicsDevice();
	// �X���b�v�`�F�[�����擾���܂��B
	SwapChain* GetSwapChain();
	// �����_�[�^�[�Q�b�g���擾���܂��B
	RenderTarget* GetRenderTarget();
	// �[�x�X�e���V�����擾���܂��B
	DepthStencil* GetDepthStencil();

	// �f�X�g���N�^�[
	~Graphics();

private:
	// �O���t�B�b�N�X �f�o�C�X
	GraphicsDevice* graphicsDevice = nullptr;
	// �X���b�v�`�F�[��
	SwapChain* swapChain = nullptr;
	// �����_�[�^�[�Q�b�g
	RenderTarget* renderTarget = nullptr;
	// �[�x�X�e���V��
	DepthStencil* depthStencil = nullptr;

	// �C���X�^���X�������֎~
	Graphics(
		GraphicsDevice* graphicsDevice, SwapChain* swapChain,
		RenderTarget* renderTarget, DepthStencil* depthStencil);
	// �R�s�[�Ƒ�����Z���֎~
	Graphics(const Graphics&) {}
	Graphics& operator=(const Graphics&) { return *this; }
};

