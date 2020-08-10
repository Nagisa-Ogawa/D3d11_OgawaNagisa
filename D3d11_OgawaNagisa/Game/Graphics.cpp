//=============================================================================
// Graphics.cpp
// 
//=============================================================================
#include "Graphics.h"
#include "GameWindow.h"
#include "DirectXHelper.h"

using namespace DirectX;
using namespace DX;

// ���̃N���X�̃C���X�^���X���쐬���܂��B
DXGIOutput* DXGIOutput::Create(IDXGIOutput* output)
{
	DXGIOutput* retVal = nullptr;
	// �\�����[�h�̋L�q
	DXGI_MODE_DESC* modeDescs = nullptr;
	try {
		DXGI_OUTPUT_DESC outputDesc = {};
		// �o�͂̐������擾
		ThrowIfFailed(output->GetDesc(&outputDesc));

		UINT numModes = 0;
		// �v�����ꂽ�`���Ƒ��̓��̓I�v�V�����Ɉ�v����\�����[�h���擾
		ThrowIfFailed(output->GetDisplayModeList(
			DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_SCALING,
			&numModes, NULL));
		modeDescs = new DXGI_MODE_DESC[numModes];
		ThrowIfFailed(output->GetDisplayModeList(
			DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_SCALING,
			&numModes, modeDescs));

		retVal = new DXGIOutput(output, outputDesc, numModes, modeDescs);
		return retVal;
	}
	catch (...) {
		SAFE_DELETE_ARRAY(modeDescs);
		SAFE_DELETE(retVal);
		throw;
	}
	return nullptr;
}

// ���̃N���X�̃C���X�^���X�����������܂��B
DXGIOutput::DXGIOutput(
	IDXGIOutput* output, const DXGI_OUTPUT_DESC& outputDesc,
	UINT numModes, const DXGI_MODE_DESC* modeDescs)
{
	output->AddRef();
	this->dxgiOutput = output;
	this->dxgiOutputDesc = outputDesc;
	this->numModes = numModes;
	this->dxgiModeDescs = modeDescs;
}

// �f�X�g���N�^�[
DXGIOutput::~DXGIOutput()
{
	SAFE_RELEASE(dxgiOutput);
	SAFE_DELETE_ARRAY(dxgiModeDescs);
}

// ���̃N���X�̃C���X�^���X���쐬���܂��B
GraphicsPart* GraphicsPart::Create(IDXGIAdapter1* adapter)
{
	GraphicsPart* retVal = nullptr;
	ID3D11Device* graphicsDevice = nullptr;
	ID3D11DeviceContext* immediateContext = nullptr;
	DXGIOutput* output = nullptr;
	try {
		DXGI_ADAPTER_DESC1 adapterDesc = {};
		ThrowIfFailed(adapter->GetDesc1(&adapterDesc));

		// �f�o�C�X�쐬���̃I�v�V�����t���O
		UINT creationFlags = 0;
#if defined(_DEBUG)
		// DEBUG�r���h�̍ۂ�Direct3D�̃f�o�b�O�\���@�\����������
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		// �f�o�C�X�A�f�o�C�X�R���e�L�X�g���쐬
		D3D_FEATURE_LEVEL featureLevel = {};
		ThrowIfFailed(D3D11CreateDevice(
			adapter, D3D_DRIVER_TYPE_UNKNOWN, 0, creationFlags, NULL, 0, D3D11_SDK_VERSION,
			&graphicsDevice,
			&featureLevel,
			&immediateContext));
		// ���̃N���X�̃C���X�^���X���쐬
		retVal = new GraphicsPart(adapter,
			adapterDesc, graphicsDevice, featureLevel, immediateContext);
		SAFE_RELEASE(graphicsDevice);
		SAFE_RELEASE(immediateContext);
		// 
		IDXGIOutput* dxgiOutput = nullptr;
		for (UINT outputIndex = 0;
			adapter->EnumOutputs(outputIndex, &dxgiOutput) == S_OK;
			++outputIndex) {
			output = DXGIOutput::Create(dxgiOutput);
			retVal->dxgiOutputs.push_back(output);
			SAFE_RELEASE(dxgiOutput);
		}

		return retVal;
	}
	catch (...) {
		SAFE_RELEASE(graphicsDevice);
		SAFE_RELEASE(immediateContext);
		SAFE_DELETE(output);
		SAFE_DELETE(retVal);
		throw;
	}
	return nullptr;
}

// ���̃N���X�̃C���X�^���X�����������܂��B
GraphicsPart::GraphicsPart(
	IDXGIAdapter1* adapter, const DXGI_ADAPTER_DESC1& adapterDesc,
	ID3D11Device* graphicsDevice, const D3D_FEATURE_LEVEL& featureLevel,
	ID3D11DeviceContext* immediateContext)
{
	adapter->AddRef();
	this->dxgiAdapter = adapter;
	this->adapterDesc = adapterDesc;
	graphicsDevice->AddRef();
	this->graphicsDevice = graphicsDevice;
	this->featureLevel = featureLevel;
	immediateContext->AddRef();
	this->immediateContext = immediateContext;
}

// �f�X�g���N�^�[
GraphicsPart::~GraphicsPart()
{
	SAFE_RELEASE(dxgiAdapter);
	SAFE_RELEASE(graphicsDevice);
	SAFE_RELEASE(immediateContext);
	for (auto&& value : dxgiOutputs) {
		SAFE_DELETE(value);
	}
	dxgiOutputs.clear();
}

// Direct3D 11�̃f�o�C�X���擾���܂��B
ID3D11Device* GraphicsPart::GetGraphicsDevice()
{
	return graphicsDevice;
}

// Direct3D 11�̋@�\���x�����擾���܂��B
D3D_FEATURE_LEVEL GraphicsPart::GetFeatureLevel() const
{
	return featureLevel;
}

// Direct3D 11�̃f�o�C�X �R���e�L�X�g���擾���܂��B
ID3D11DeviceContext* GraphicsPart::GetImmediateContext()
{
	return immediateContext;
}

// ���̃N���X�̃V���O���g���C���X�^���X���擾���܂��B
Graphics& Graphics::GetInstance()
{
	static Graphics instance;
	return instance;
}

// Direct3D 11�̃f�o�C�X���擾���܂��B
ID3D11Device* Graphics::GetGraphicsDevice()
{
	return GetInstance().graphicsParts[0]->GetGraphicsDevice();
}

// Direct3D 11�̃f�o�C�X �R���e�L�X�g���擾���܂��B
ID3D11DeviceContext* Graphics::GetImmediateContext()
{
	return GetInstance().graphicsParts[0]->GetImmediateContext();
}

// �����_�[�^�[�Q�b�g���擾���܂��B
ID3D11RenderTargetView* Graphics::GetRenderTargetView()
{
	return GetInstance().renderTargetView;
}

// �[�x�X�e���V�����擾���܂��B
ID3D11DepthStencilView* Graphics::GetDepthStencilView()
{
	return GetInstance().depthStencilView;
}

// �X���b�v�`�F�[�����擾���܂��B
IDXGISwapChain* Graphics::GetSwapChain()
{
	return GetInstance().swapChain;
}

// �O���t�B�b�N�X�@�\�����������܂��B
void Graphics::Initialize()
{
	GetInstance().OnCreate();
}

// ���̃N���X�̃C���X�^���X���쐬����ۂɌĂяo����܂��B
void Graphics::OnCreate()
{
	IDXGIFactory1* dxgiFactory = nullptr;
	IDXGIAdapter1* dxgiAdapter = nullptr;
	GraphicsPart* graphicsPart = nullptr;

	ID3D11Texture2D* backBuffer = nullptr;
	ID3D11Texture2D* depthStencil = nullptr;
	try {
		// DXGI�t�@�N�g���[���쐬
		ThrowIfFailed(CreateDXGIFactory1(
			IID_PPV_ARGS(&dxgiFactory)
		));
		UINT adapter = 0;
		// �r�f�I�J�[�h��񋓂���
		while (dxgiFactory->EnumAdapters1(
			adapter,	// �A�_�v�^�[�̃C���f�b�N�X
			&dxgiAdapter	// IDXGIAdapter1�̃|�C���^�[�̃A�h���X
		) == S_OK) {
			// �O���t�B�b�N�f�o�C�X�ƃC�~�f�B�G�C�g�R���e�L�X�g���쐬
			graphicsPart = GraphicsPart::Create(dxgiAdapter);
			// graphicsPart��ǉ�
			graphicsParts.push_back(graphicsPart);
			SAFE_RELEASE(dxgiAdapter);
			++adapter;
		}

		// �f�o�C�X�쐬���̃I�v�V�����t���O
		UINT creationFlags = 0;
#if defined(_DEBUG)
		// DEBUG�r���h�̍ۂ�Direct3D�̃f�o�b�O�\���@�\����������
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		// �쐬����X���b�v�`�F�[���ɂ��Ă̋L�q
		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
		// �𑜓x�̕���\���B0���w�肷��Əo�̓E�B���h�E���畝���擾��
		// �X���b�v�`�F�[���Ɋ��蓖�Ă�
		swapChainDesc.BufferDesc.Width = GameWindow::GetWidth();
		// �𑜓x�̍�����\���B0���w�肷��Əo�̓E�B���h�E���獂�����擾��
		// �X���b�v�`�F�[���Ɋ��蓖�Ă�
		swapChainDesc.BufferDesc.Height = GameWindow::GetHeight();
		// ���t���b�V�����[�g��\��
		swapChainDesc.BufferDesc.RefreshRate = { 60, 1 };
		// �f�[�^���ǂ̂悤�Ɉ������̌`��
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		// �}���`�T���v�����O�̃p�����[�^
		swapChainDesc.SampleDesc = { 1, 0 };
		// �o�b�N�o�b�t�@�[�̕\�ʎg�p�ʂ�CPU�A�N�Z�X�I�v�V������ݒ肷�郁���o�[
		swapChainDesc.BufferUsage =
			DXGI_USAGE_RENDER_TARGET_OUTPUT |
			DXGI_USAGE_SHADER_INPUT;	// �V�F�[�_�[���\�[�X�Ƃ��Ďg�p���邱�Ƃ�ݒ�
		// �X���b�v�`�F�[�����̃o�b�t�@�̐��B�ʏ�͂��̐���
		// �t�����g�o�b�t�@���܂߂�
		swapChainDesc.BufferCount = 2;
		// �o�̓E�B���h�E�ւ�HWND�n���h���BNULL�ł͂����Ȃ�
		swapChainDesc.OutputWindow = GameWindow::GetHwnd();
		// �o�b�t�@�̐؂�ւ����̃p�����[�^
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		// �o�͂��E�B���h�E���[�h�ł��邩���w�肷��p�����[�^
		swapChainDesc.Windowed = TRUE;
		//// �f�o�C�X�A�f�o�C�X�R���e�L�X�g�A�X���b�v�`�F�[�����쐬
		//ThrowIfFailed(D3D11CreateDeviceAndSwapChain(
		//	NULL, D3D_DRIVER_TYPE_HARDWARE, 0, creationFlags, NULL, 0, D3D11_SDK_VERSION, &swapChainDesc,
		//	&swapChain, &graphicsDevice, &featureLevel, &immediateContext));

		auto graphicsPart = graphicsParts[0];
		auto graphicsDevice = graphicsPart->GetGraphicsDevice();
		auto featureLevel = graphicsPart->GetFeatureLevel();
		auto immediateContext = graphicsPart->GetImmediateContext();
		// �X���b�v�`�F�[�����쐬
		ThrowIfFailed(dxgiFactory->CreateSwapChain(graphicsDevice, &swapChainDesc, &swapChain));
		ThrowIfFailed(swapChain->GetDesc(&swapChainDesc));

		// �o�b�N�o�b�t�@�[���擾
		ThrowIfFailed(swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer)));
		// �o�b�N�o�b�t�@�[�ɃA�N�Z�X���邽�߂̃����_�[�^�[�Q�b�g �r���[���쐬
		ThrowIfFailed(graphicsDevice->CreateRenderTargetView(
			backBuffer, NULL, &renderTargetView));
		// �o�b�N�o�b�t�@�[�ɃV�F�[�_�[����A�N�Z�X���邽�߂̃��\�[�X �r���[���쐬
		ThrowIfFailed(graphicsDevice->CreateShaderResourceView(
			backBuffer, NULL, &renderTargetResourceView));
		SAFE_RELEASE(backBuffer);

		// �e�N�X�`���[�ƃV�F�[�_�[���\�[�X�r���[�̃t�H�[�}�b�g��ݒ�
		DXGI_FORMAT textureFormat = depthStencilFormat;
		DXGI_FORMAT resourceFormat = depthStencilFormat;
		// �[�x�X�e���V���̃t�H�[�}�b�g�̌`����
		// �e�N�X�`���ƃV�F�[�_�[���\�[�X�̃t�H�[�}�b�g�����킹��
		switch (depthStencilFormat)
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
		// �[�x�X�e���V�����쐬
		D3D11_TEXTURE2D_DESC depthStencilDesc = {};
		depthStencilDesc.Width = swapChainDesc.BufferDesc.Width;
		depthStencilDesc.Height = swapChainDesc.BufferDesc.Height;
		// �e�N�X�`�����̃~�b�v�}�b�v���x���̍ő吔�B
		// �}���`�T���v���e�N�X�`���ɂ�1���g�p����
		depthStencilDesc.MipLevels = 1;
		// �e�N�X�`���z����̃e�N�X�`��
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = textureFormat;
		depthStencilDesc.SampleDesc = swapChainDesc.SampleDesc;
		// �e�N�X�`���̓ǂݏ������@�����ʂ���l
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		// �p�C�v���C���X�e�[�W�Ƀo�C���h���邽�߂̃t���O
		depthStencilDesc.BindFlags =
			D3D11_BIND_DEPTH_STENCIL |
			D3D11_BIND_SHADER_RESOURCE;	// �V�F�[�_�[���\�[�X�Ƃ��Ďg�p���邱�Ƃ�ݒ�
		// �������CPU�A�N�Z�X�̃^�C�v���w�肷��t���O
		// CPU�A�N�Z�X���K�v�Ȃ��ꍇ��0���w��
		depthStencilDesc.CPUAccessFlags = 0;
		// ���\�[�X�I�v�V�������w�肷��t���O
		// �g�p���Ȃ��ꍇ��0���w��
		depthStencilDesc.MiscFlags = 0;

		ThrowIfFailed(graphicsDevice->CreateTexture2D(&depthStencilDesc, NULL, &depthStencil));
		// �[�x�X�e���V���ɃA�N�Z�X���邽�߂̃r���[���쐬
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
		depthStencilViewDesc.Format = depthStencilFormat;
		if (depthStencilDesc.SampleDesc.Count > 0) {
			depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
		}
		else {
			depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			depthStencilViewDesc.Texture2D.MipSlice = 0;
		}
		depthStencilViewDesc.Texture2D.MipSlice = 0;
		ThrowIfFailed(graphicsDevice->CreateDepthStencilView(depthStencil, &depthStencilViewDesc, &depthStencilView));
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
		ThrowIfFailed(graphicsDevice->CreateShaderResourceView(
			depthStencil, &depthStencilResourceViewDesc, &depthStencilResourceView));
		SAFE_RELEASE(depthStencil);


		SAFE_RELEASE(dxgiFactory);


	}
	catch (...) {
		SAFE_RELEASE(dxgiFactory);
		SAFE_RELEASE(dxgiAdapter);
		SAFE_DELETE(graphicsPart);

		SAFE_RELEASE(swapChain);
		SAFE_RELEASE(graphicsDevice);
		SAFE_RELEASE(immediateContext);

		SAFE_RELEASE(backBuffer);
		SAFE_RELEASE(renderTargetView);
		SAFE_RELEASE(renderTargetResourceView);

		SAFE_RELEASE(depthStencil);
		SAFE_RELEASE(depthStencilView);
		SAFE_RELEASE(depthStencilResourceView);

		throw;
	}
}

// ���̃N���X�̃C���X�^���X��j������ۂɌĂяo����܂��B
void Graphics::Release()
{
	GetInstance().OnRelease();
}

// �O���t�B�b�N���\�[�X��������܂��B
void Graphics::OnRelease()
{
	SAFE_RELEASE(depthStencilResourceView);
	SAFE_RELEASE(depthStencilView);
	SAFE_RELEASE(renderTargetResourceView);
	SAFE_RELEASE(renderTargetView);
	SAFE_RELEASE(swapChain);
	SAFE_RELEASE(immediateContext);
	SAFE_RELEASE(graphicsDevice);

	for (auto&& graphicsPart : graphicsParts) {
		SAFE_DELETE(graphicsPart);
	}
	graphicsParts.clear();
}
