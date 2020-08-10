//=============================================================================
// Graphics.h
// 
//=============================================================================
#pragma once

#include <vector>
#include <d3d11.h>
#include <DirectXMath.h>

class DXGIOutput
{
public:
	// ���̃N���X�̃C���X�^���X���쐬���܂��B
	static DXGIOutput* Create(IDXGIOutput* output);
	// �f�X�g���N�^�[
	~DXGIOutput();

private:
	IDXGIOutput* dxgiOutput = nullptr;
	DXGI_OUTPUT_DESC dxgiOutputDesc = {};
	UINT numModes = 0;
	const DXGI_MODE_DESC* dxgiModeDescs = nullptr;

	// ���̃N���X�̃C���X�^���X�����������܂��B
	DXGIOutput(IDXGIOutput* output, const DXGI_OUTPUT_DESC& outputDesc,
		UINT numModes, const DXGI_MODE_DESC* modeDescs);
};

class GraphicsPart
{
public:
	// ���̃N���X�̃C���X�^���X���쐬���܂��B
	static GraphicsPart* Create(IDXGIAdapter1* adapter);

	// �f�X�g���N�^�[
	~GraphicsPart();

	// Direct3D 11�̃f�o�C�X���擾���܂��B
	ID3D11Device* GetGraphicsDevice();
	// Direct3D 11�̋@�\���x�����擾���܂��B
	D3D_FEATURE_LEVEL GetFeatureLevel() const;
	// Direct3D 11�̃f�o�C�X �R���e�L�X�g���擾���܂��B
	ID3D11DeviceContext* GetImmediateContext();

private:
	// 
	IDXGIAdapter1* dxgiAdapter = nullptr;
	// 
	DXGI_ADAPTER_DESC1 adapterDesc = {};
	// Direct3D 11�̃f�o�C�X
	ID3D11Device* graphicsDevice = nullptr;
	// Direct3D 11�̋@�\���x��
	D3D_FEATURE_LEVEL featureLevel = {};
	// Direct3D 11�̃f�o�C�X �R���e�L�X�g
	ID3D11DeviceContext* immediateContext = nullptr;
	// 
	std::vector<DXGIOutput*> dxgiOutputs;

	// ���̃N���X�̃C���X�^���X�����������܂��B
	GraphicsPart(
		IDXGIAdapter1* adapter, const DXGI_ADAPTER_DESC1& adapterDesc,
		ID3D11Device* graphicsDevice, const D3D_FEATURE_LEVEL& featureLevel,
		ID3D11DeviceContext* immediateContext);
};

class Graphics
{
public:
	// �O���t�B�b�N�X�@�\�����������܂��B
	static void Initialize();
	// �O���t�B�b�N�X���\�[�X��������܂��B
	static void Release();

	// Direct3D 11�̃f�o�C�X���擾���܂��B
	static ID3D11Device* GetGraphicsDevice();
	// Direct3D 11�̃f�o�C�X �R���e�L�X�g���擾���܂��B
	static ID3D11DeviceContext* GetImmediateContext();
	// �����_�[�^�[�Q�b�g���擾���܂��B
	static ID3D11RenderTargetView* GetRenderTargetView();
	// �[�x�X�e���V�����擾���܂��B
	static ID3D11DepthStencilView* GetDepthStencilView();
	// �X���b�v�`�F�[�����擾���܂��B
	static IDXGISwapChain* GetSwapChain();

private:
	// ���̃N���X�̃V���O���g���C���X�^���X���擾���܂��B
	static Graphics& GetInstance();

	std::vector<GraphicsPart*> graphicsParts;

	// Direct3D 11�̃f�o�C�X
	ID3D11Device* graphicsDevice = nullptr;
	// Direct3D 11�̃f�o�C�X �R���e�L�X�g
	ID3D11DeviceContext* immediateContext = nullptr;
	// Direct3D 11�̋@�\���x��
	D3D_FEATURE_LEVEL featureLevel = {};

	// �X���b�v�`�F�[��
	IDXGISwapChain* swapChain = nullptr;

	// �����_�[�^�[�Q�b�g
	ID3D11RenderTargetView* renderTargetView = nullptr;
	// �o�b�N�o�b�t�@�[���V�F�[�_�[�ŗ��p���邽�߂̃��\�[�X �r���[
	ID3D11ShaderResourceView* renderTargetResourceView = nullptr;

	// �[�x�X�e���V���̃t�H�[�}�b�g
	const DXGI_FORMAT depthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	// �[�x�X�e���V��
	ID3D11DepthStencilView* depthStencilView = nullptr;
	// �[�x�X�e���V�����V�F�[�_�[�ŗ��p���邽�߂̃��\�[�X �r���[
	ID3D11ShaderResourceView* depthStencilResourceView = nullptr;

	// ���̃N���X�̃C���X�^���X���쐬����ۂɌĂяo����܂��B
	void OnCreate();
	// ���̃N���X�̃C���X�^���X��j������ۂɌĂяo����܂��B
	void OnRelease();
};

