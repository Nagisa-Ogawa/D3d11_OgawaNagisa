//=============================================================================
// Game.cpp
// 
//=============================================================================
#include <DirectXMath.h>	// DirectX�̎Z�p���C�u�����[
#include <DirectXColors.h>	// DirectX�̃J���[���C�u�����[
#include "Game.h"

using namespace DirectX;

// �֐��̃v���g�^�C�v�錾
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// �A�v���P�[�V���������������܂��B
void Game::Initialize(LPCWSTR windowTitle, int screenWidth, int screenHeight)
{
	// �E�B���h�E�̃^�C�g��
	WindowTitle = windowTitle;
	// �E�B���h�E�̕�
	ScreenWidth = screenWidth;
	// �E�B���h�E�̍���
	ScreenHeight = screenHeight;
}

// �E�B���h�E���쐬���܂��B
bool Game::InitWindow()
{
	HINSTANCE hInstance = GetModuleHandle(NULL);
	// �E�B���h�E �N���X��o�^����
	const wchar_t CLASS_NAME[] = L"GameWindow";
	WNDCLASSEX wndClass = {};
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.lpfnWndProc = WindowProc;	// �E�B���h�E �v���V�[�W���[���w��
	wndClass.hInstance = hInstance;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)COLOR_BACKGROUND;
	wndClass.lpszClassName = CLASS_NAME;
	if (!RegisterClassEx(&wndClass)) {
		return false;
	}

	// �N���C�A���g�̈悪�w�肵���𑜓x�ɂȂ�E�B���h�E�T�C�Y���v�Z
	RECT rect = { 0, 0, ScreenWidth, ScreenHeight };
	AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, FALSE, 0);

	// �E�B���h�E���쐬����
	auto hWnd = CreateWindowEx(
		0, CLASS_NAME, WindowTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		(rect.right - rect.left), (rect.bottom - rect.top),
		NULL, NULL, hInstance, NULL);
	if (hWnd == NULL) {
		return false;
	}

	// �E�B���h�E��\��
	ShowWindow(hWnd, SW_SHOWNORMAL);
	UpdateWindow(hWnd);

	this->hWnd = hWnd;

	return true;
}

// �E�B���h�E ���b�Z�[�W����������v���V�[�W���[
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_CLOSE:
		// �E�B���h�E�����
		if (MessageBox(hWnd, L"�E�B���h�E����܂����H", L"���b�Z�[�W", MB_OKCANCEL) == IDOK) {
			DestroyWindow(hWnd);
		}
		return 0;

	case WM_DESTROY:
		// �A�v���P�[�V�������I��
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


// �O���t�B�b�N�f�o�C�X���쐬���܂�
bool Game::InitGraphicsDevice()
{
	HRESULT hr = S_OK;	// �֐��̎��s���ʂ��󂯎��ϐ�

// �f�o�C�X�쐬���̃I�v�V�����t���O
	UINT creationFlags = 0;
#if defined(_DEBUG)
	// DEBUG�r���h�̍ۂ�Direct3D�̃f�o�b�O�\���@�\����������
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif


	// �쐬����X���b�v�`�F�[���ɂ��Ă̋L�q
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	{
		// �𑜓x�̕���\���B0���w�肷��Əo�̓E�B���h�E���畝���擾��
		// �X���b�v�`�F�[���Ɋ��蓖�Ă�
		swapChainDesc.BufferDesc.Width = ScreenWidth;
		// �𑜓x�̍�����\���B0���w�肷��Əo�̓E�B���h�E���獂�����擾��
		// �X���b�v�`�F�[���Ɋ��蓖�Ă�
		swapChainDesc.BufferDesc.Height = ScreenHeight;
		// ���t���b�V�����[�g��\��
		swapChainDesc.BufferDesc.RefreshRate = { 60,1 };
		// �f�[�^���ǂ̂悤�Ɉ������̌`��
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		// �}���`�T���v�����O�̃p�����[�^
		swapChainDesc.SampleDesc = { 1,0 };
		// �o�b�N�o�b�t�@�[�̕\�ʎg�p�ʂ�CPU�A�N�Z�X�I�v�V������ݒ肷�郁���o�[
		// �V�F�[�_�[���\�[�X�Ƃ��Ďg�p���邱�Ƃ�ݒ�
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT |
			DXGI_USAGE_SHADER_INPUT;
		// �X���b�v�`�F�[�����̃o�b�t�@�̐��B�ʏ�͂��̐���
		// �t�����g�o�b�t�@���܂߂�
		swapChainDesc.BufferCount = 2;
		// �o�̓E�B���h�E�ւ�HWND�n���h���BNULL�ł͂����Ȃ�
		swapChainDesc.OutputWindow = hWnd;
		// �o�b�t�@�̐؂�ւ����̃p�����[�^
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		// �o�͂��E�B���h�E���[�h�ł��邩���w�肷��p�����[�^
		swapChainDesc.Windowed = TRUE;
	}

	// �f�o�C�X�A�f�o�C�X�R���e�L�X�g�A�X���b�v�`�F�[�����쐬
	hr = D3D11CreateDeviceAndSwapChain(
		NULL,						// �f�o�C�X�̍쐬���Ɏg�p����r�f�I�A�_�v�^�[�ւ̃|�C���^�[�BNULL���w�肷��ƃf�t�H���g�A�_�v�^�[
		D3D_DRIVER_TYPE_HARDWARE,	// �쐬����h���C�o�[�̎�ށB��{��HARDWARE
		0,							// �\�t�g�E�F�A���X�^���C�U�[����������DLL�̃n���h���BDriverType��SOFTWARE�̏ꍇ��NULL�ɂ��邱�Ƃ��ł��Ȃ�
		creationFlags,				// �f�o�C�X�쐬���̃I�v�V�����t���O
		featureLevels,				// �쐬����@�\���x���̔z��ւ̃|�C���^�[�BNULL���w�肵�ďȗ��ł���
		6,							// �쐬����@�\���x���̔z��ւ̃|�C���^�[�BNULL���w�肵�ďȗ��ł���
		D3D11_SDK_VERSION,			// ���ł�D3D11_SDK_VERSION
		&swapChainDesc,
		&swapChain,
		&graphicsDevice,			// [�o��]Direct3D 11 �f�o�C�X
		&featureLevel,				// ���ۂɍ쐬���ꂽ�@�\���x��
		&immediateContext			// [�o��]Direct3D 11 �f�o�C�X �R���e�L�X�g
	);
	if (FAILED(hr)) {
		MessageBox(hWnd, L"Direct3D 11�f�o�C�X���쐬�ł��܂���ł����B", L"�G���[", MB_OK);
		return false;
	}

	// �X���b�v�`�F�[������o�b�N�o�b�t�@���擾
	ID3D11Texture2D* backBuffer = nullptr;
	// �X���b�v�`�F�[���̃o�b�N�o�b�t�@�ɃA�N�Z�X
	hr = swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
	if (FAILED(hr))
	{
		MessageBox(hWnd, L"�o�b�N�o�b�t�@���擾�ł��܂���ł����B", L"�G���[", MB_OK);
		return false;
	}

	// �o�b�N�o�b�t�@�ɃA�N�Z�X���邽�߂̃����_�[�^�[�Q�b�g�r���[���쐬
	hr = graphicsDevice->CreateRenderTargetView(backBuffer, NULL, &renderTargetViews[0]);
	if (FAILED(hr))
	{
		MessageBox(hWnd, L"�����_�[�^�[�Q�b�g�r���[���쐬�ł��܂���ł����B", L"�G���[", MB_OK);
		return false;
	}

	// �o�b�N�o�b�t�@�ɃV�F�[�_�[����A�N�Z�X���邽�߂̃��\�[�X�r���[���쐬
	hr = graphicsDevice->CreateShaderResourceView(
		// �V�F�[�_�[�ւ̓��͂Ƃ��ċ@�\���郊�\�[�X�ւ̃|�C���^�[
		// ������ID3D11Resource�^����ID3D11Texture2D��IDID3D11Resource��
		// �p�����Ă��邽�߈����Ɏg����
		backBuffer,
		// �V�F�[�_�[���\�[�X�r���[�����ւ̃|�C���^�[
		// NULL���w�肷��ƃ��\�[�X�S�̂ɃA�N�Z�X����r���[���쐬����
		NULL,
		&renderTargetResourceView);
	if (FAILED(hr))
	{
		MessageBox(hWnd, L"�����_�[�^�[�Q�b�g ���\�[�X �r���[���쐬�ł��܂���ł����B", L"�G���[", MB_OK);
		return false;
	}
	SAFE_RELEASE(backBuffer);


	// �e�N�X�`���̃t�H�[�}�b�g��ݒ�
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
	ID3D11Texture2D* depthStencil = nullptr;
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
	// �V�F�[�_�[���\�[�X�Ƃ��Ďg�p���邱�Ƃ�ݒ�
	depthStencilDesc.BindFlags =
		D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	// �������CPU�A�N�Z�X�̃^�C�v���w�肷��t���O
	// CPU�A�N�Z�X���K�v�Ȃ��ꍇ��0���w��
	depthStencilDesc.CPUAccessFlags = 0;
	// ���\�[�X�I�v�V�������w�肷��t���O
	// �g�p���Ȃ��ꍇ��0���w��
	depthStencilDesc.MiscFlags = 0;
	// �[�x�X�e���V�����쐬
	hr = graphicsDevice->CreateTexture2D(&depthStencilDesc, NULL, &depthStencil);
	if (FAILED(hr))
	{
		MessageBox(hWnd, L"�[�x�X�e���V�����쐬�ł��܂���ł����B", L"�G���[", MB_OK);
		return false;
	}

	// �[�x�X�e���V���ɃA�N�Z�X���邽�߂̃r���[���쐬
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
	// �[�x�X�e���V���Ɠ����t�H�[�}�b�g�ɂ���
	depthStencilViewDesc.Format = depthStencilFormat;
	if (depthStencilDesc.SampleDesc.Count > 0)
	{
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	}
	else
	{
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;
	}
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// �[�x�X�e���V���r���[���쐬
	hr = graphicsDevice->CreateDepthStencilView(depthStencil, &depthStencilViewDesc, &depthStencilView);
	if (FAILED(hr)) {
		MessageBox(hWnd, L"�[�x�X�e���V�� �r���[���쐬�ł��܂���ł����B", L"�G���[", MB_OK);
		return false;
	}

	// �[�x�X�e���V���ɃV�F�[�_�[����A�N�Z�X���邽�߂̃��\�[�X�r���[���쐬
	D3D11_SHADER_RESOURCE_VIEW_DESC depthStencilResourceViewDesc = {};
	depthStencilResourceViewDesc.Format = resourceFormat;
	if (depthStencilDesc.SampleDesc.Count > 0)
	{
		depthStencilResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
	}
	else
	{
		depthStencilResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		depthStencilResourceViewDesc.Texture2D.MostDetailedMip = 0;
		depthStencilResourceViewDesc.Texture2D.MipLevels = 1;
	}
	hr = graphicsDevice->CreateShaderResourceView(
		depthStencil,
		&depthStencilResourceViewDesc,
		&depthStencilResourceView);
	if (FAILED(hr))
	{
		MessageBox(hWnd, L"�[�x�X�e���V���[�x�X�e���V�� ���\�[�X �r���[���쐬�ł��܂���ł����B", L"�G���[", MB_OK);
		return false;
	}
	SAFE_RELEASE(depthStencil);

	// �r���[�|�[�g��ݒ�
	viewports[0].Width = static_cast<FLOAT>(ScreenWidth);
	viewports[0].Height = static_cast<FLOAT>(ScreenHeight);
	viewports[0].MinDepth = 0.0f;
	viewports[0].MaxDepth = 1.0f;
	viewports[0].TopLeftX = 0.0f;
	viewports[0].TopLeftY = 0.0f;

	return true;

}


// �O���t�B�b�N���\�[�X���J�����܂�
void Game::ReleaseGraphicsDevice()
{
	// ���\�[�X�����
	/*
	if (immediateContext != nullptr) {
		immediateContext->Release();
		immediateContext = nullptr;
	}
	if (graphicsDevice != nullptr) {
		graphicsDevice->Release();
		graphicsDevice = nullptr;
	}
	*/
	SAFE_RELEASE(depthStencilResourceView);
	SAFE_RELEASE(depthStencilView);
	SAFE_RELEASE(renderTargetResourceView);
	SAFE_RELEASE(renderTargetViews[0]);
	SAFE_RELEASE(swapChain);
	SAFE_RELEASE(immediateContext);
	SAFE_RELEASE(graphicsDevice);
}

// ���b�Z�[�W���[�v�����s���܂��B
int Game::Run()
{
	// �E�B���h�E���쐬
	if (!InitWindow()) {
		MessageBox(NULL, L"�E�B���h�E�̍쐬�Ɏ��s���܂����B", L"���b�Z�[�W", MB_OK);
		return -1;
	}
	// �O���t�B�b�N�f�o�C�X���쐬
	if (!InitGraphicsDevice())
	{
		MessageBox(NULL, L"�O���t�B�b�N�f�o�C�X���������ł��܂���ł����B", L"���b�Z�[�W", MB_OK);
		return -1;
	}

	// ����
	// �}�C�i�X�ȉ��Ȃ�G���[
	HRESULT hr = S_OK;

	// ���_�f�[�^�̔z��
	// ���̕�����
	const int u_max = 64;
	// �c�̕�����
	const int v_max = 64;
	// ���a
	const float radius = 3.0f;
	const int vertex_num = (u_max + 1) * (v_max + 1);
	// UV���W�̃I�t�Z�b�g
	const float uOffset = 1.0f / u_max;
	const float vOffset = 1.0f / v_max;
	// ���݂�UV���W
	float nowUPos = 0.0f;
	float nowVPos = 0.0f;

	VertexPositionNormalTexture vertices[vertex_num] = {};

	for (int v = 0; v <= v_max; v++) {
		for (int u = 0; u <= u_max; u++) {
			// 0<=theta<=��
			float theta = XM_PI * v / v_max;
			// 0<=phi<2��
			float phi = XM_2PI * u / u_max;
			if (u == u_max)
			{

				float phi = XM_2PI * 0 / u_max;
				vertices[(u_max + 1) * v + u].position.x = radius * XMScalarSin(theta) * XMScalarCos(phi);
				vertices[(u_max + 1) * v + u].position.y = radius * XMScalarCos(theta);
				vertices[(u_max + 1) * v + u].position.z = radius * XMScalarSin(theta) * XMScalarSin(phi);
				// �@��
				vertices[(u_max + 1) * v + u].normal.x = XMScalarSin(theta) * XMScalarCos(phi);
				vertices[(u_max + 1) * v + u].normal.y = XMScalarCos(theta);
				vertices[(u_max + 1) * v + u].normal.z = XMScalarSin(theta) * XMScalarSin(phi);
				// UV���W
				vertices[(u_max + 1) * v + u].texCoord = { nowUPos,nowVPos };
			}
			else
			{
				// �O�����ɍ��W����O�����������W�ւ̕ϊ��������(y��z�����ւ�)
				// x = r * sin�� * cos��
				// y = r * cos��
				// z = r * sin�� * sin��
				vertices[(u_max + 1) * v + u].position.x = radius * XMScalarSin(theta) * XMScalarCos(phi);
				vertices[(u_max + 1) * v + u].position.y = radius * XMScalarCos(theta);
				vertices[(u_max + 1) * v + u].position.z = radius * XMScalarSin(theta) * XMScalarSin(phi);
				// �@��
				vertices[(u_max + 1) * v + u].normal.x = XMScalarSin(theta) * XMScalarCos(phi);
				vertices[(u_max + 1) * v + u].normal.y = XMScalarCos(theta);
				vertices[(u_max + 1) * v + u].normal.z = XMScalarSin(theta) * XMScalarSin(phi);
				// UV���W
				vertices[(u_max + 1) * v + u].texCoord = { nowUPos,nowVPos };

			}
			nowUPos += uOffset;
		}
		nowUPos = 0.0f;
		nowVPos += vOffset;
	}


	//// ���_�f�[�^�̔z��
	//VertexPositionNormalTexture vertices[divideCount + divideCount + 2 * (divideCount + 1)] = {};
	//// Top
	//for (size_t triangle = 0; triangle < divideCount; triangle++)
	//{
	//	// �p�x
	//	const auto angle = XM_2PI / divideCount * triangle;
	//	// �ʒu���W
	//	vertices[triangle].position.x = radius * XMScalarCos(angle);
	//	vertices[triangle].position.y = height / 2;
	//	vertices[triangle].position.z = radius * -XMScalarSin(angle);
	//	// �@���x�N�g��
	//	vertices[triangle].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	//	// UV���W

	//}
	//// Bottom
	//for (size_t triangle = 0; triangle < divideCount; triangle++)
	//{
	//	const auto angle = XM_2PI / divideCount * triangle;
	//	// vertices�z��ɂ�Top�ʂ̒��_����(divideCount)�������炵�ăA�N�Z�X����
	//	// �ʒu���W
	//	vertices[triangle + divideCount].position.x = radius * XMScalarCos(angle);
	//	vertices[triangle + divideCount].position.y = -height / 2;
	//	vertices[triangle + divideCount].position.z = radius * XMScalarSin(angle);
	//	// �@���x�N�g��
	//	vertices[triangle + divideCount].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
	//	// UV���W
	//}
	//// Side
	//for (size_t triangle = 0; triangle <= divideCount; triangle++)
	//{
	//	const auto angle = XM_2PI / divideCount * triangle;
	//	// vertices�z��ɂ�Top�ʂ�Bottom�ʂ̒��_���̕�(2 * divideCount)�������炵�ăA�N�Z�X����
	//	auto& top = vertices[2 * triangle + 0 + 2 * divideCount];
	//	auto& bottom = vertices[2 * triangle + 1 + 2 * divideCount];

	//	// Top�ʂɗאڂ��钸�_
	//	// �@���x�N�g��
	//	top.normal = XMFLOAT3(XMScalarCos(angle), 0.0f, XMScalarSin(angle));
	//	// �ʒu���W
	//	top.position.x = radius * top.normal.x;
	//	top.position.y = height / 2;
	//	top.position.z = radius * top.normal.z;

	//	// Bottom�ʂɗאڂ��钸�_
	//	// �@���x�N�g��
	//	bottom.normal = top.normal;
	//	// �ʒu���W
	//	bottom.position = top.position;
	//	bottom.position.y = -height / 2;
	//	// UV���W
	//	if (triangle%2==0)
	//	{
	//		top.texCoord = { UVPos,0.0f };
	//		bottom.texCoord = { UVPos,1.0f };
	//	}
	//	if (triangle %2==1)
	//	{
	//		top.texCoord = { UVPos,0.0f };
	//		bottom.texCoord = { UVPos,1.0f };
	//	}
	//	UVPos += UVOffset;
	//}

		// ���_�f�[�^�̔z��
	//VertexPositionNormalTexture vertices[] = {
	//	// Front
	//	//{ { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } },
	//	//{ {-1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
	//	//{ { 1.0f,-1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } },
	//	//{ {-1.0f,-1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } },
		//// Back
		//{ {-2.0f, 2.0f,0.0f }, { 0.0f, 0.0f,-1.0f }, { 0.0f, 0.0f } },
		//{ { 2.0f, 2.0f,0.0f }, { 0.0f, 0.0f,-1.0f }, { 1.0f, 0.0f } },
		//{ {-2.0f,-2.0f,0.0f }, { 0.0f, 0.0f,-1.0f }, { 0.0f, 1.0f } },
		//{ { 2.0f,-2.0f,0.0f }, { 0.0f, 0.0f,-1.0f }, { 1.0f, 1.0f } },
	//	//// Right
	//	//{ { 1.0f, 1.0f,-1.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
	//	//{ { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } },
	//	//{ { 1.0f,-1.0f,-1.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f } },
	//	//{ { 1.0f,-1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } },
	//	//// Left
	//	//{ {-1.0f, 1.0f, 1.0f }, {-1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
	//	//{ {-1.0f, 1.0f,-1.0f }, {-1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } },
	//	//{ {-1.0f,-1.0f, 1.0f }, {-1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f } },
	//	//{ {-1.0f,-1.0f,-1.0f }, {-1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } },
	//	//// UP
	//	//{ {-1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f } },
	//	//{ { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f } },
	//	//{ {-1.0f, 1.0f,-1.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f } },
	//	//{ { 1.0f, 1.0f,-1.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 1.0f } },
	//	//// DOWN
	//	//{ { 1.0f,-1.0f, 1.0f }, { 0.0f,-1.0f, 0.0f }, { 0.0f, 0.0f } },
	//	//{ {-1.0f,-1.0f, 1.0f }, { 0.0f,-1.0f, 0.0f }, { 1.0f, 0.0f } },
	//	//{ { 1.0f,-1.0f,-1.0f }, { 0.0f,-1.0f, 0.0f }, { 0.0f, 1.0f } },
	//	//{ {-1.0f,-1.0f,-1.0f }, { 0.0f,-1.0f, 0.0f }, { 1.0f, 1.0f } },
	//};


	// ���_�o�b�t�@�[���쐬
	VertexBuffer* vertexBuffer = nullptr;
	vertexBuffer = VertexBuffer::Create(graphicsDevice, sizeof vertices);
	if (vertexBuffer == nullptr)
	{
		OutputDebugString(L"���_�o�b�t�@�[���쐬�ł��܂���ł����B");
		return -1;
	}
	// ���\�[�X���X�V
	vertexBuffer->SetData(vertices);

	// �C���f�b�N�X�f�[�^�̗v�f��
	const int index_num = (v_max * u_max) * 6;
	//�C���f�b�N�X�f�[�^�̔z��
    UINT32 indices[index_num] = {};

	for (int v = 0; v < v_max; v++) {
		// �c�����̃I�t�Z�b�g
		int v_offset = v * u_max;
		int offset = v * (u_max + 1);
		for (int u = 0; u < u_max; u++) 
		{
			indices[(v_offset + u) * 6 + 0] = offset + u + 0;
			indices[(v_offset + u) * 6 + 1] = offset + u + 1;
			indices[(v_offset + u) * 6 + 2] = offset + u + u_max + 1;
			indices[(v_offset + u) * 6 + 3] = offset + u_max + 1 + (u + 1);
			indices[(v_offset + u) * 6 + 4] = offset + u_max + 1 + u;
			indices[(v_offset + u) * 6 + 5] = offset + u + 1;
		}
	}

	////�C���f�b�N�X�f�[�^�̔z��
	//UINT32 indices[3 * (divideCount - 2) + 3 * (divideCount - 2) + 3 * 2 * divideCount] = {};
	//// Top
	//for (UINT32 triangle = 0; triangle < divideCount - 2; triangle++)
	//{
	//	indices[3 * triangle + 0] = 0;
	//	indices[3 * triangle + 1] = triangle + 1;
	//	indices[3 * triangle + 2] = triangle + 2;
	//}
	//// Buttom
	//const SIZE_T indexOffset = 3 * (divideCount - 2);
	//for (UINT32 triangle = 0; triangle < divideCount - 2; triangle++)
	//{
	//	indices[3 * triangle + 0 + indexOffset] = 0 + divideCount;
	//	indices[3 * triangle + 1 + indexOffset] = triangle + 1 + divideCount;
	//	indices[3 * triangle + 2 + indexOffset] = triangle + 2 + divideCount;
	//}
	//// Side
	//for (UINT triangle = 0; triangle < divideCount; triangle++)
	//{
	//	const UINT32 indexOffset = 3 * (divideCount - 2) + 3 * (divideCount - 2);
	//	//indices[indexOffset + 6 * triangle + 0] = (2 * triangle + 0) % (2 * divideCount) + 2 * divideCount;
	//	//indices[indexOffset + 6 * triangle + 1] = (2 * triangle + 2) % (2 * divideCount) + 2 * divideCount;
	//	//indices[indexOffset + 6 * triangle + 2] = (2 * triangle + 1) % (2 * divideCount) + 2 * divideCount;
	//	//indices[indexOffset + 6 * triangle + 3] = (2 * triangle + 3) % (2 * divideCount) + 2 * divideCount;
	//	//indices[indexOffset + 6 * triangle + 4] = (2 * triangle + 1) % (2 * divideCount) + 2 * divideCount;
	//	//indices[indexOffset + 6 * triangle + 5] = (2 * triangle + 2) % (2 * divideCount) + 2 * divideCount;
	//	indices[indexOffset + 6 * triangle + 0] = (2 * triangle + 0) + 2 * divideCount;
	//	indices[indexOffset + 6 * triangle + 1] = (2 * triangle + 2) + 2 * divideCount;
	//	indices[indexOffset + 6 * triangle + 2] = (2 * triangle + 1) + 2 * divideCount;
	//	indices[indexOffset + 6 * triangle + 3] = (2 * triangle + 3) + 2 * divideCount;
	//	indices[indexOffset + 6 * triangle + 4] = (2 * triangle + 1) + 2 * divideCount;
	//	indices[indexOffset + 6 * triangle + 5] = (2 * triangle + 2) + 2 * divideCount;

	//}

		// �C���f�b�N�X�f�[�^�̔z��
	//UINT32 indices[] = {
	//	 0,  1,  2,  3,  2,  1,	// Front
	//	// 4,  5,  6,  7,  6,  5,	// Back
	//	// 8,  9, 10, 11, 10,  9,	// Right
	//	//12, 13, 14, 15, 14, 13,	// Left
	//	//16, 17, 18, 19, 18, 17,	// Top
	//	//20, 21, 22, 23, 22, 21,	// Bottom
	//};


	// �C���f�b�N�X�̐�
	UINT indexCount = ARRAYSIZE(indices);

	// �C���f�b�N�X�o�b�t�@�[���쐬
	IndexBuffer* indexBuffer = nullptr;
	indexBuffer = IndexBuffer::Create(graphicsDevice, indexCount);
	if (indexBuffer == nullptr) {
		return -1;
	}
	// ���\�[�X���X�V
	indexBuffer->SetData(indices);



	// �萔�o�b�t�@�[����ăV�F�[�_�[�ɖ��t���[������s��f�[�^��\���Ă��܂��B
	//struct ConstantLight
	//{
	//	// ���C�g�̈ʒu���W(x,y,z,w ���s���� w = 0, �_���� w = 1)
	//	DirectX::XMFLOAT4 lightPosition;
	//	// ���C�g�̊g�U���˂̐F(r,g,b)
	//	DirectX::XMFLOAT4 lightDiffuse;
	//	// ���C�g�̋��ʔ��˂̐F(r,g,b)
	//	DirectX::XMFLOAT4 lightSpecular;
	//};

	struct ConstantDirectionalLight {
		XMFLOAT4 direction = XMFLOAT4(-0.5f, 0.5f, -1.0f, 0.0f);
		XMFLOAT4 color = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f);
	};

	struct ConstantPointLight {
		XMFLOAT4 position = XMFLOAT4(10.0f, -10.0f, -2.0f, 0.0f);;
		XMFLOAT4 color = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
		XMFLOAT4 distance_decay = XMFLOAT4(100.0f, 1.64f, 0.0f, 0.0f);
	};

	struct ConstantMaterial
	{
		// ����
		XMFLOAT4 roughness_metallic = XMFLOAT4(0.5f, 1.0f, 0, 0);
		XMFLOAT4 albedo = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f);
	};

	struct ConstantMatricesBuffer
	{
		XMFLOAT4X4 worldMatrix;
		XMFLOAT4X4 viewMatrix;
		XMFLOAT4X4 projectionMatrix;
		XMFLOAT4X4 worldViewProjectionMatrix;
		// ���_���W
		XMFLOAT4 viewPosition = XMFLOAT4(0, 1, -10, 1);
		// ����
		// ����
		ConstantDirectionalLight constantDirectionalLight;
		ConstantPointLight constantPointLight;
		// ���̂̎���
		ConstantMaterial material;
	};

	// �萔�o�b�t�@�[���쐬
	ConstantBuffer* constantBuffer = nullptr;
	constantBuffer = ConstantBuffer::Create(graphicsDevice, sizeof(ConstantMatricesBuffer));
	if (constantBuffer == nullptr)
	{
		OutputDebugString(L"�萔�o�b�t�@�[���쐬�ł��܂���ł����B");
		return -1;
	}

	// �V�F�[�_�\���쐬
	BasicVertexShader* vertexShader = nullptr;
	vertexShader = BasicVertexShader::Create(graphicsDevice);
	if (vertexShader == nullptr)
	{
		OutputDebugString(L"���_�V�F�[�_�[���쐬�ł��܂���ł����B");
		return -1;
	}
	BasicGeometryShader* geometryShader = nullptr;
	geometryShader = BasicGeometryShader::Create(graphicsDevice);
	if (geometryShader == nullptr)
	{
		OutputDebugString(L"�W�I���g���[�V�F�[�_�[���쐬�ł��܂���ł����B");
		return -1;
	}
	BasicPixelShader* pixelShader = nullptr;
	pixelShader = BasicPixelShader::Create(graphicsDevice);
	if (pixelShader == nullptr)
	{
		OutputDebugString(L"�s�N�Z���V�F�[�_�[���쐬�ł��܂���ł����B");
		return -1;
	}

	// ���̓��C�A�E�g���쐬
	InputLayout* inputLayout = InputLayout::Create(
		graphicsDevice,
		VertexPositionNormalTexture::GetInputElementDescs(),
		VertexPositionNormalTexture::GetInputElementDescsLength(),
		vertexShader->GetBytecode(),
		vertexShader->GetBytecodeLength()
	);
	if (inputLayout == nullptr)
	{
		return -1;
	}

	// �摜�̃_�~�[
	//const uint32_t source[] = {
	//	0xFF00FFFF, 0xFF000000, 0xFF00FFFF, 0xFF000000,
	//	0xFF000000, 0xFF00FFFF, 0xFF000000, 0xFF00FFFF,
	//	0xFF00FFFF, 0xFF000000, 0xFF00FFFF, 0xFF000000,
	//	0xFF000000, 0xFF00FFFF, 0xFF000000, 0xFF00FFFF,
	//};
	{
		//FILE* fp;

		//fopen_s(&fp,"earth.data", "rb");

		//if (fp == NULL)
		//{
		//	printf("���s");
		//	return -1;
		//}

		//fread(source, sizeof(uint32_t), 1024*1024, fp);

		//fclose(fp);
	}

	// bitmap�f�[�^�̓ǂݍ���
	ReadBitMap readBitMap;
	// �f�[�^���擾
	uint32_t* source = readBitMap.ReadFromBitMap("../resource/image/gold.bmp");
	if (source == nullptr)
	{
		OutputDebugString(L"�摜�f�[�^��ǂݍ��߂܂���ł����B");
		return -1;
	}
	// �e�N�X�`���[���쐬
	Texture2D* texture =
		Texture2D::Create(graphicsDevice, readBitMap.width, readBitMap.height, DXGI_FORMAT_R8G8B8A8_UNORM, false);
	if (texture == nullptr) {
		OutputDebugString(L"�e�N�X�`���[���쐬�ł��܂���ł����B");
		return -1;
	}
	texture->width = readBitMap.width;
	// �s�N�Z���f�[�^���X�V
	texture->SetData(source);
	// �f�[�^���J��
	readBitMap.Release();

	float time = 0;

	// ���b�Z�[�W���[�v�����s
	MSG msg = {};
	while (true) {
		time += 0.00666f;

		// �萔�o�b�t�@�[�֓]������f�[�^�\�[�X������
		XMMATRIX worldMatrix = XMMatrixIdentity();
		worldMatrix *= XMMatrixScaling(1.0f, 1.0f, 1.0f);
		XMVECTOR axis = XMVectorSet(1, 1, 0, 0);
		worldMatrix *= XMMatrixRotationAxis(axis, time);
		worldMatrix *= XMMatrixTranslation(0.0f, 0.0f, 0.0f);
		// �r���[�s����v�Z
		XMVECTOR viewPosition = XMVectorSet(0, 1, -10, 1);	// �J�����̈ʒu
		XMVECTOR focusPosition = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);	// �œ_�̈ʒu
		XMVECTOR upDirection = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);		// �J�����̏���� (�ʏ�� <0.0f, 1.0f, 0.0f>)
		XMMATRIX viewMatrix =
			// ������W�n�̃r���[�s����쐬
			XMMatrixLookAtLH(viewPosition, focusPosition, upDirection);
		XMMATRIX worldView = worldMatrix * viewMatrix;
		// �v���W�F�N�V�����s����v�Z
		float fovAngleY = 60.0f;			// ����p
		float aspectRatio = 640 / 480.0f;	// �A�X�y�N�g��
		float nearZ = 0.1f;					// �N���b�s���O����(������߂��͕̂`�悵�Ȃ��j
		float farZ = 100.0f;				// �N���b�s���O����(�����艓���͕̂`�悵�Ȃ�)

		// �v���W�F�N�V�����ϊ��s����쐬
		XMMATRIX projectonMatrix =
			XMMatrixPerspectiveFovLH(
				XMConvertToRadians(fovAngleY),
				aspectRatio,
				nearZ,
				farZ
			);

		// �����̈ʒu
		//XMVECTOR lightPosition = XMVectorSet(-2.0f, 2.0f, -1.0f, 0.0f);
		//// �g�U���ː���
		//XMVECTOR lightDiffuse = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);
		//// ���ʔ��ː���
		//XMVECTOR lightSpecular = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);
		//// �����ː���
		//XMVECTOR lightAmbient = XMVectorSet(0.2f, 0.2f, 0.2f, 0.0f);
		//// �g�U���˗�
		//XMVECTOR materialDiffuse = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);
		//// ���ʔ��˗��Ƌ���(����x�W��)
		//XMVECTOR materialSpecular = XMVectorSet(1.0f, 1.0f, 1.0f, 20.0f);
		//// �����˗�
		//XMVECTOR materialAmbient = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);

		ConstantMatricesBuffer constantMatricesBuffer;
		XMStoreFloat4x4(&constantMatricesBuffer.worldMatrix, XMMatrixTranspose(worldMatrix));
		XMStoreFloat4x4(&constantMatricesBuffer.viewMatrix, XMMatrixTranspose(viewMatrix));
		XMStoreFloat4x4(&constantMatricesBuffer.projectionMatrix, XMMatrixTranspose(projectonMatrix));
		XMStoreFloat4x4(&constantMatricesBuffer.worldViewProjectionMatrix, XMMatrixTranspose(worldMatrix* viewMatrix* projectonMatrix));
		XMStoreFloat4(&constantMatricesBuffer.viewPosition, viewPosition);
		// XMStoreFloat4(&constantMatricesBuffer.ambient,lightAmbient);
		//XMStoreFloat4(&constantMatricesBuffer.light.lightPosition,lightPosition);
		//XMStoreFloat4(&constantMatricesBuffer.light.lightDiffuse,lightDiffuse);
		//XMStoreFloat4(&constantMatricesBuffer.light.lightSpecular,lightSpecular);
		//XMStoreFloat4(&constantMatricesBuffer.material.materialAmbient,materialAmbient);
		//XMStoreFloat4(&constantMatricesBuffer.material.materialDiffuse,materialDiffuse);
		//XMStoreFloat4(&constantMatricesBuffer.material.materialSpecular,materialSpecular);

		// �萔�o�b�t�@�[���X�V
		constantBuffer->SetData(&constantMatricesBuffer);

		// �����_�[�^�[�Q�b�g��ݒ�
		immediateContext->OMSetRenderTargets(
			1,					// �o�C���h���郌���_�[�^�[�Q�b�g�̐�
			renderTargetViews,	// �f�o�C�X�Ƀo�C���h���郌���_�[�^�[�Q�b�g
			depthStencilView	// �f�o�C�X�Ƀo�C���h����[�x�X�e���V���r���[
		);
		// ��ʂ��N���A�[
		immediateContext->ClearRenderTargetView(
			renderTargetViews[0],	// �����_�[�^�[�Q�b�g�̃|�C���^�[
			clearColor				// �����_�[�^�[�Q�b�g��h��Ԃ��F
		);

		// �[�x�X�e���V���r���[���N���A
		immediateContext->ClearDepthStencilView(
			depthStencilView,							// �Ώۂ̐[�x�X�e���V���r���[
			D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,	// �N���A����f�[�^�̃^�C�v
			1.0f,										// ���̒l�Ő[�x�o�b�t�@�[��������	
			0											// ���̒l�ŃX�e���V���o�b�t�@�[��������
		);

		// �r���[�|�[�g��ݒ�
		immediateContext->RSSetViewports(
			1,			// �o�C���h����r���[�|�[�g�̐�
			viewports	// �o�C���h����r���[�|�[�g
		);

		// ���_�o�b�t�@�[��ݒ�
		ID3D11Buffer* vertexBuffers[1] = { vertexBuffer->GetNativePointer() };
		UINT strides[1] = { sizeof(VertexPositionNormalTexture) };	// ���_�o�b�t�@��1���_���̗v�f�̃T�C�Y
		UINT offsets[1] = { 0 };	// ���_�o�b�t�@�̍ŏ��̗v�f�Ǝg�p�����ŏ��̒��_�o�b�t�@�̊Ԃ̃o�C�g��
		// ���_�o�b�t�@�̔z���Input-Assembler(IA) stage�փo�C���h(D3D11�̃p�C�v���C���Q��)
		immediateContext->IASetVertexBuffers(
			0,	// �o�C���f�B���O����ŏ��̓��̓X���b�g
			ARRAYSIZE(vertexBuffers),	// �z����̒��_�o�b�t�@�̐�
			vertexBuffers,	// ���_�o�b�t�@�̔z��ւ̃|�C���^
			strides,	// �X�g���C�h�l�ւ̔z��̃|�C���^
			offsets		// �I�t�Z�b�g�l�ւ̔z��̃|�C���^
		);

		// �V�F�[�_�\��ݒ�
		immediateContext->VSSetShader(
			vertexShader->GetNativePointer(),	// ���_�V�F�[�_�\�ւ̃|�C���^�[
			NULL,			// �N���X�C���X�^���X�C���^�[�t�F�C�X�̔z��ւ̃|�C���^�[
			0				// �N���X�C���X�^���X�C���^�[�t�F�C�X�̔z��̗v�f��
		);
		immediateContext->GSSetShader(
			geometryShader->GetNativePointer(),
			NULL,
			0
		);
		immediateContext->PSSetShader(
			pixelShader->GetNativePointer(),	// �s�N�Z���V�F�[�_�\�ւ̃|�C���^�[
			NULL,			// �N���X�C���X�^���X�C���^�[�t�F�C�X�̔z��ւ̃|�C���^�[
			0				// �N���X�C���X�^���X�C���^�[�t�F�C�X�̔z��̗v�f��
		);

		// �e�V�F�[�_�[�ɒ萔�o�b�t�@�[��ݒ�
		ID3D11Buffer* constantBuffers[1] = { constantBuffer->GetNativePointer() };
		immediateContext->VSSetConstantBuffers(
			0,
			1,					// �Z�b�g����o�b�t�@�[�̐�
			constantBuffers		// �萔�o�b�t�@�[�̔z��
		);
		immediateContext->GSSetConstantBuffers(0, 1, constantBuffers);
		immediateContext->PSSetConstantBuffers(0, 1, constantBuffers);

		// �V�F�[�_�\���\�[�X�r���[���쐬
		ID3D11ShaderResourceView* textureViews[1] = { texture->GetShaderResourceView() };
		// �V�F�[�_�[���\�[�X�̔z����s�N�Z���V�F�[�_�[�X�e�[�W�Ƀo�C���h
		immediateContext->PSSetShaderResources(0, 1, textureViews);
		// �T���v���[�X�e�[�g���쐬
		ID3D11SamplerState* samplerStates[1] = { texture->GetSamplerState() };
		// �s�N�Z���V�F�[�_�[�p�C�v���C���X�e�[�W�ɃT���v���[�X�e�[�g�̔z���n���܂�
		immediateContext->PSSetSamplers(0, 1, samplerStates);

		// ���_�o�b�t�@�\�ƒ��_�V�F�[�_�\�̑g�����ɑΉ��������̓��C�A�E�g��ݒ�
		immediateContext->IASetInputLayout(
			inputLayout->GetNativePointer()		// ���̓��C�A�E�g�ɋL�q����I�u�W�F�N�g�̃|�C���^�[
		);

		// �v���~�e�B�u�g�|���W�[�Ƃ��ăg���C�A���O����ݒ�
		immediateContext->IASetPrimitiveTopology(
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST		// ���_�f�[�^���O�p�`�̃��X�g�Ƃ��ĉ��߂���
		);

		// �C���f�b�N�X�o�b�t�@�[��ݒ�
		immediateContext->IASetIndexBuffer(
			indexBuffer->GetNativePointer(),
			DXGI_FORMAT_R32_UINT,
			0
		);

		// �`��
		immediateContext->DrawIndexed(
			ARRAYSIZE(indices),		// �`�悷�钸�_�̐�
			0,		// �ŏ��̒��_�̃C���f�b�N�X
			0
		);

		// �o�b�N�o�b�t�@�ɕ`�悵���C���[�W���f�B�X�v���C�ɕ\��
		HRESULT hr = S_OK;
		hr = swapChain->Present(
			1,	// �t���[���̕\���𐂒��u�����N�Ɠ���������@���w�肷�鐮��
			0	// �X���b�v�`�F�[���\���I�v�V�������܂ސ����l
		);
		if (FAILED(hr))
		{
			MessageBox(hWnd,
				L"�O���t�B�b�N�f�o�C�X�������I�Ɏ��O���ꂽ���A�h���C�o�[���A�b�v�f�[�g����܂����B",
				L"�G���[", MB_OK);
			return -1;
		}

		// ���̃E�B���h�E�̃��b�Z�[�W�����݂��邩���m�F
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
			// ���b�Z�[�W���擾
			if (!GetMessage(&msg, NULL, 0, 0)) {
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}


	// �J������
	texture->Release();
	inputLayout->Release();
	vertexBuffer->Release();
	indexBuffer->Release();
	constantBuffer->Release();
	vertexShader->Release();
	geometryShader->Release();
	pixelShader->Release();

	ReleaseGraphicsDevice();

	return (int)msg.wParam;
}


