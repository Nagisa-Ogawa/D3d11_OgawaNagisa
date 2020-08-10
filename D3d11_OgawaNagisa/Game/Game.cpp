//=============================================================================
// Game.cpp
// 
//=============================================================================
#include <DirectXMath.h>	// DirectX�̎Z�p���C�u�����[
#include <d3dcompiler.h>
#include <DirectXColors.h>	// DirectX�̃J���[���C�u�����[
#include "DirectXHelper.h"
#include "Game.h"
#include "GameWindow.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "GeometryShader.h"
#include "BasicVertexShader.h"
#include "BasicGeometryShader.h"
#include "BasicPixelShader.h"
#include "VertexBuffer.h"
#include "InputLayout.h"

using namespace DirectX;
using namespace DX;

//// �֐��̃v���g�^�C�v�錾
//LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// ���̃N���X�̃V���O���g���C���X�^���X���擾���܂��B
Game& Game::GetInstance()
{
	static Game instance;
	return instance;
}


// �A�v���P�[�V���������������܂��B
void Game::Initialize(LPCWSTR windowTitle, int screenWidth, int screenHeight)
{
	GetInstance().OnInitialize(windowTitle, screenWidth, screenHeight);
}

// �������̍ۂɌĂяo����܂��B
void Game::OnInitialize(LPCWSTR windowTitle, int screenWidth, int screenHeight)
{
	if (isInitialized) {
		return;
	}

	// �E�B���h�E���쐬
	GameWindow::Initialize(windowTitle, screenWidth, screenHeight);
	// �O���t�B�b�N�f�o�C�X���쐬
	Graphics::Initialize();

	isInitialized = true;
}

// ���b�Z�[�W���[�v�����s���܂��B
int Game::Run()
{
	{
		//// ���_�f�[�^�̔z��
		//// ���̕�����
		//const int u_max = 64;
		//// �c�̕�����
		//const int v_max = 64;
		//// ���a
		//const float radius = 3.0f;
		//const int vertex_num = (u_max + 1) * (v_max + 1);
		//// UV���W�̃I�t�Z�b�g
		//const float uOffset = 1.0f / u_max;
		//const float vOffset = 1.0f / v_max;
		//// ���݂�UV���W
		//float nowUPos = 0.0f;
		//float nowVPos = 0.0f;

		//VertexPositionNormalTexture* vertices = new VertexPositionNormalTexture[vertex_num];

		//for (int v = 0; v <= v_max; v++) {
		//	for (int u = 0; u <= u_max; u++) {
		//		// 0<=theta<=��
		//		float theta = XM_PI * v / v_max;
		//		// 0<=phi<2��
		//		float phi = XM_2PI * u / u_max;
		//		if (u == u_max)
		//		{

		//			float phi = XM_2PI * 0 / u_max;
		//			vertices[(u_max + 1) * v + u].position.x = radius * XMScalarSin(theta) * XMScalarCos(phi);
		//			vertices[(u_max + 1) * v + u].position.y = radius * XMScalarCos(theta);
		//			vertices[(u_max + 1) * v + u].position.z = radius * XMScalarSin(theta) * XMScalarSin(phi);
		//			// �@��
		//			vertices[(u_max + 1) * v + u].normal.x = XMScalarSin(theta) * XMScalarCos(phi);
		//			vertices[(u_max + 1) * v + u].normal.y = XMScalarCos(theta);
		//			vertices[(u_max + 1) * v + u].normal.z = XMScalarSin(theta) * XMScalarSin(phi);
		//			// UV���W
		//			vertices[(u_max + 1) * v + u].texCoord = { nowUPos,nowVPos };
		//		}
		//		else
		//		{
		//			// �O�����ɍ��W����O�����������W�ւ̕ϊ��������(y��z�����ւ�)
		//			// x = r * sin�� * cos��
		//			// y = r * cos��
		//			// z = r * sin�� * sin��
		//			vertices[(u_max + 1) * v + u].position.x = radius * XMScalarSin(theta) * XMScalarCos(phi);
		//			vertices[(u_max + 1) * v + u].position.y = radius * XMScalarCos(theta);
		//			vertices[(u_max + 1) * v + u].position.z = radius * XMScalarSin(theta) * XMScalarSin(phi);
		//			// �@��
		//			vertices[(u_max + 1) * v + u].normal.x = XMScalarSin(theta) * XMScalarCos(phi);
		//			vertices[(u_max + 1) * v + u].normal.y = XMScalarCos(theta);
		//			vertices[(u_max + 1) * v + u].normal.z = XMScalarSin(theta) * XMScalarSin(phi);
		//			// UV���W
		//			vertices[(u_max + 1) * v + u].texCoord = { nowUPos,nowVPos };

		//		}
		//		nowUPos += uOffset;
		//	}
		//	nowUPos = 0.0f;
		//	nowVPos += vOffset;
		//}

		//	// ���_�f�[�^�̔z��
		////VertexPositionNormalTexture vertices[] = {
		//	//{ {-2.0f, 2.0f,0.0f }, { 0.0f, 0.0f,-1.0f }, { 0.0f, 0.0f } },
		//	//{ { 2.0f, 2.0f,0.0f }, { 0.0f, 0.0f,-1.0f }, { 1.0f, 0.0f } },
		//	//{ {-2.0f,-2.0f,0.0f }, { 0.0f, 0.0f,-1.0f }, { 0.0f, 1.0f } },
		//	//{ { 2.0f,-2.0f,0.0f }, { 0.0f, 0.0f,-1.0f }, { 1.0f, 1.0f } },
		////};


		//// ���_�o�b�t�@�[���쐬
		//VertexBuffer* vertexBuffer = nullptr;
		//vertexBuffer = VertexBuffer::Create(graphicsDevice, sizeof(VertexPositionNormalTexture) * vertex_num);
		//if (vertexBuffer == nullptr)
		//{
		//	OutputDebugString(L"���_�o�b�t�@�[���쐬�ł��܂���ł����B");
		//	return -1;
		//}
		//// ���\�[�X���X�V
		//vertexBuffer->SetData(vertices);

		//delete[] vertices;

		//// �C���f�b�N�X�f�[�^�̗v�f��
		//const int index_num = (v_max * u_max) * 6;
		////�C���f�b�N�X�f�[�^�̔z��
		//UINT32* indices = new UINT32[index_num];

		//for (int v = 0; v < v_max; v++) {
		//	// �c�����̃I�t�Z�b�g
		//	int v_offset = v * u_max;
		//	int offset = v * (u_max + 1);
		//	for (int u = 0; u < u_max; u++) 
		//	{
		//		indices[(v_offset + u) * 6 + 0] = offset + u + 0;
		//		indices[(v_offset + u) * 6 + 1] = offset + u + 1;
		//		indices[(v_offset + u) * 6 + 2] = offset + u + u_max + 1;
		//		indices[(v_offset + u) * 6 + 3] = offset + u_max + 1 + (u + 1);
		//		indices[(v_offset + u) * 6 + 4] = offset + u_max + 1 + u;
		//		indices[(v_offset + u) * 6 + 5] = offset + u + 1;
		//	}
		//}

		//	// �C���f�b�N�X�f�[�^�̔z��
		////UINT32 indices[] = {
		////	 0,  1,  2,  3,  2,  1,	// Front
		////	// 4,  5,  6,  7,  6,  5,	// Back
		////	// 8,  9, 10, 11, 10,  9,	// Right
		////	//12, 13, 14, 15, 14, 13,	// Left
		////	//16, 17, 18, 19, 18, 17,	// Top
		////	//20, 21, 22, 23, 22, 21,	// Bottom
		////};


		//// �C���f�b�N�X�̐�
		//UINT indexCount = index_num;

		//// �C���f�b�N�X�o�b�t�@�[���쐬
		//IndexBuffer* indexBuffer = nullptr;
		//indexBuffer = IndexBuffer::Create(graphicsDevice, indexCount);
		//if (indexBuffer == nullptr) {
		//	return -1;
		//}
		//// ���\�[�X���X�V
		//indexBuffer->SetData(indices);

		//delete[] indices;

		//// ���s�����̏��
		//struct ConstantDirectionalLight {
		//	XMFLOAT4 direction = XMFLOAT4(-0.5f, 0.5f, -1.0f, 0.0f);
		//	XMFLOAT4 color = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f);
		//};

		//// ���̕\�ʂ̎���
		//struct ConstantMaterial
		//{
		//	XMFLOAT3 baseColor = XMFLOAT3(1.0f, 1.0f, 1.0f);
		//	float metallic = 1.0f;
		//	XMFLOAT3 specular = XMFLOAT3(0.5f, 0.5f, 0.5f);
		//	float roughness = 0.5f;
		//};
		//
		//struct ConstantMatricesBuffer
		//{
		//	XMFLOAT4X4 worldMatrix;
		//	XMFLOAT4X4 viewMatrix;
		//	XMFLOAT4X4 projectionMatrix;
		//	XMFLOAT4X4 worldViewProjectionMatrix;
		//	// ���_���W
		//	XMFLOAT4 viewPosition = XMFLOAT4(0, 1, -10, 1);
		//	// ����
		//	ConstantDirectionalLight constantDirectionalLight;
		//	// ���̂̎���
		//	ConstantMaterial material;
		//};

		//// �萔�o�b�t�@�[���쐬
		//ConstantBuffer* constantBuffer = nullptr;
		//constantBuffer = ConstantBuffer::Create(graphicsDevice, sizeof(ConstantMatricesBuffer));
		//if (constantBuffer == nullptr)
		//{
		//	OutputDebugString(L"�萔�o�b�t�@�[���쐬�ł��܂���ł����B");
		//	return -1;
		//}



		//// �摜�̃_�~�[
		////const uint32_t source[] = {
		////	0xFF00FFFF, 0xFF000000, 0xFF00FFFF, 0xFF000000,
		////	0xFF000000, 0xFF00FFFF, 0xFF000000, 0xFF00FFFF,
		////	0xFF00FFFF, 0xFF000000, 0xFF00FFFF, 0xFF000000,
		////	0xFF000000, 0xFF00FFFF, 0xFF000000, 0xFF00FFFF,
		////};
		//// raw�f�[�^�̓ǂݍ���
		//{
		//	//FILE* fp;

		//	//fopen_s(&fp,"earth.data", "rb");

		//	//if (fp == NULL)
		//	//{
		//	//	printf("���s");
		//	//	return -1;
		//	//}

		//	//fread(source, sizeof(uint32_t), 1024*1024, fp);

		//	//fclose(fp);
		//}

		//// bitmap�f�[�^�̓ǂݍ���
		//ReadBitMap readBitMap;
		//// �f�[�^���擾
		//uint32_t* source = readBitMap.ReadFromBitMap("../resource/image/earth.bmp");
		//if (source == nullptr)
		//{
		//	OutputDebugString(L"�摜�f�[�^��ǂݍ��߂܂���ł����B");
		//	return -1;
		//}
		//// �e�N�X�`���[���쐬
		//Texture2D* texture =
		//	Texture2D::Create(graphicsDevice, readBitMap.width, readBitMap.height, DXGI_FORMAT_R8G8B8A8_UNORM, false);
		//if (texture == nullptr) {
		//	OutputDebugString(L"�e�N�X�`���[���쐬�ł��܂���ł����B");
		//	return -1;
		//}
		//texture->width = readBitMap.width;
		//// �s�N�Z���f�[�^���X�V
		//texture->SetData(source);
		//// �f�[�^���J��
		//readBitMap.Release();

		//float time = 0;

		//// ���b�Z�[�W���[�v�����s
		//MSG msg = {};
		//while (true) {
		//	time += 0.00666f;

		//	// �萔�o�b�t�@�[�֓]������f�[�^�\�[�X������
		//	XMMATRIX worldMatrix = XMMatrixIdentity();
		//	worldMatrix *= XMMatrixScaling(1.0f, 1.0f, 1.0f);
		//	XMVECTOR axis = XMVectorSet(1, 1, 0, 0);
		//	worldMatrix *= XMMatrixRotationAxis(axis, time);
		//	worldMatrix *= XMMatrixTranslation(0.0f, 0.0f, 0.0f);
		//	// �r���[�s����v�Z
		//	XMVECTOR viewPosition = XMVectorSet(0, 1, -10, 1);	// �J�����̈ʒu
		//	XMVECTOR focusPosition = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);	// �œ_�̈ʒu
		//	XMVECTOR upDirection = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);		// �J�����̏���� (�ʏ�� <0.0f, 1.0f, 0.0f>)
		//	XMMATRIX viewMatrix =
		//		// ������W�n�̃r���[�s����쐬
		//		XMMatrixLookAtLH(viewPosition, focusPosition, upDirection);
		//	XMMATRIX worldView = worldMatrix * viewMatrix;
		//	// �v���W�F�N�V�����s����v�Z
		//	float fovAngleY = 60.0f;			// ����p
		//	float aspectRatio = 640 / 480.0f;	// �A�X�y�N�g��
		//	float nearZ = 0.1f;					// �N���b�s���O����(������߂��͕̂`�悵�Ȃ��j
		//	float farZ = 100.0f;				// �N���b�s���O����(�����艓���͕̂`�悵�Ȃ�)

		//	// �v���W�F�N�V�����ϊ��s����쐬
		//	XMMATRIX projectonMatrix =
		//		XMMatrixPerspectiveFovLH(
		//			XMConvertToRadians(fovAngleY),
		//			aspectRatio,
		//			nearZ,
		//			farZ
		//		);

		//	ConstantMatricesBuffer constantMatricesBuffer;
		//	XMStoreFloat4x4(&constantMatricesBuffer.worldMatrix, XMMatrixTranspose(worldMatrix));
		//	XMStoreFloat4x4(&constantMatricesBuffer.viewMatrix, XMMatrixTranspose(viewMatrix));
		//	XMStoreFloat4x4(&constantMatricesBuffer.projectionMatrix, XMMatrixTranspose(projectonMatrix));
		//	XMStoreFloat4x4(&constantMatricesBuffer.worldViewProjectionMatrix, XMMatrixTranspose(worldMatrix* viewMatrix* projectonMatrix));
		//	// XMStoreFloat4(&constantMatricesBuffer.viewPosition, viewPosition);

		//	// �萔�o�b�t�@�[���X�V
		//	constantBuffer->SetData(&constantMatricesBuffer);

		//	// �e�V�F�[�_�[�ɒ萔�o�b�t�@�[��ݒ�
		//	ID3D11Buffer* constantBuffers[1] = { constantBuffer->GetNativePointer() };
		//	immediateContext->VSSetConstantBuffers(
		//		0,
		//		1,					// �Z�b�g����o�b�t�@�[�̐�
		//		constantBuffers		// �萔�o�b�t�@�[�̔z��
		//	);
		//	immediateContext->GSSetConstantBuffers(0, 1, constantBuffers);
		//	immediateContext->PSSetConstantBuffers(0, 1, constantBuffers);

		//	// �V�F�[�_�\���\�[�X�r���[���쐬
		//	ID3D11ShaderResourceView* textureViews[1] = { texture->GetShaderResourceView() };
		//	// �V�F�[�_�[���\�[�X�̔z����s�N�Z���V�F�[�_�[�X�e�[�W�Ƀo�C���h
		//	immediateContext->PSSetShaderResources(0, 1, textureViews);
		//	// �T���v���[�X�e�[�g���쐬
		//	ID3D11SamplerState* samplerStates[1] = { texture->GetSamplerState() };
		//	// �s�N�Z���V�F�[�_�[�p�C�v���C���X�e�[�W�ɃT���v���[�X�e�[�g�̔z���n���܂�
		//	immediateContext->PSSetSamplers(0, 1, samplerStates);

		//	// ���_�o�b�t�@�\�ƒ��_�V�F�[�_�\�̑g�����ɑΉ��������̓��C�A�E�g��ݒ�
		//	immediateContext->IASetInputLayout(
		//		inputLayout->GetNativePointer()		// ���̓��C�A�E�g�ɋL�q����I�u�W�F�N�g�̃|�C���^�[
		//	);

		//	// �v���~�e�B�u�g�|���W�[�Ƃ��ăg���C�A���O����ݒ�
		//	immediateContext->IASetPrimitiveTopology(
		//		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST		// ���_�f�[�^���O�p�`�̃��X�g�Ƃ��ĉ��߂���
		//	);

		//	// �C���f�b�N�X�o�b�t�@�[��ݒ�
		//	immediateContext->IASetIndexBuffer(
		//		indexBuffer->GetNativePointer(),
		//		DXGI_FORMAT_R32_UINT,
		//		0
		//	);

		//	// �`��
		//	immediateContext->DrawIndexed(
		//		indexCount,		// �`�悷�钸�_�̐�
		//		0,		// �ŏ��̒��_�̃C���f�b�N�X
		//		0
		//	);

		//	// �o�b�N�o�b�t�@�ɕ`�悵���C���[�W���f�B�X�v���C�ɕ\��
		//	HRESULT hr = S_OK;
		//	hr = swapChain->Present(
		//		1,	// �t���[���̕\���𐂒��u�����N�Ɠ���������@���w�肷�鐮��
		//		0	// �X���b�v�`�F�[���\���I�v�V�������܂ސ����l
		//	);
		//	if (FAILED(hr))
		//	{
		//		MessageBox(hWnd,
		//			L"�O���t�B�b�N�f�o�C�X�������I�Ɏ��O���ꂽ���A�h���C�o�[���A�b�v�f�[�g����܂����B",
		//			L"�G���[", MB_OK);
		//		return -1;
		//	}

		//	// ���̃E�B���h�E�̃��b�Z�[�W�����݂��邩���m�F
		//	if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
		//		// ���b�Z�[�W���擾
		//		if (!GetMessage(&msg, NULL, 0, 0)) {
		//			break;
		//		}
		//		TranslateMessage(&msg);
		//		DispatchMessage(&msg);
		//	}
		//}


		//// �J������
		//texture->Release();
		//inputLayout->Release();
		//vertexBuffer->Release();
		//indexBuffer->Release();
		//constantBuffer->Release();
		//vertexShader->Release();
		//geometryShader->Release();
		//pixelShader->Release();

		//ReleaseGraphicsDevice();

		//return (int)msg.wParam;
	}
	return GetInstance().OnRun();

}

// ���b�Z�[�W���[�v�����s���܂��B
int Game::OnRun()
{
	// ���������̏ꍇ�̂݊���l�ŏ���������
	if (!isInitialized) {
		Initialize(L"�^�C�g��", 640, 480);
	}

	// �r���[�|�[�g
	viewport.Width = static_cast<FLOAT>(GameWindow::GetWidth());
	viewport.Height = static_cast<FLOAT>(GameWindow::GetHeight());
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	// ���_�V�F�[�_�[
	VertexShader* vertexShader = nullptr;
	// �W�I���g���[ �V�F�[�_�[
	GeometryShader* geometryShader = nullptr;
	// �s�N�Z�� �V�F�[�_�[
	PixelShader* pixelShader = nullptr;
	// �V�F�[�_�[���쐬
	vertexShader = VertexShader::Create(BasicVertexShader);
	geometryShader = GeometryShader::Create(BasicGeometryShader);
	pixelShader = PixelShader::Create(BasicPixelShader);

	// �ʒu���W�݂̂������_�f�[�^�̌^
	struct VertexPosition
	{
		XMFLOAT3 position;	// �ʒu���W
	};
	// ���_�f�[�^
	const VertexPosition vertices[] = {
		{ { -1.0f, 0.0f, 0.0f }, },	// ���_0
		{ { 0.0f, 1.0f, 0.0f }, },	// ���_1
		{ { 1.0f, 0.0f, 0.0f }, },	// ���_2
	};
	// ���_�f�[�^�ɂ��ē��͗v�f���`
	const D3D11_INPUT_ELEMENT_DESC inputElementDescs[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	// ���_�o�b�t�@�[���쐬
	VertexBuffer* vertexBuffer = nullptr;
	vertexBuffer = VertexBuffer::Create(vertices);

	// ���̓��C�A�E�g���쐬
	InputLayout* inputLayout = nullptr;
	inputLayout = InputLayout::Create(
		inputElementDescs, vertexShader->GetShaderBytecode());




	DirectX::XMVECTOR scale;
	DirectX::XMVECTOR rotation;
	DirectX::XMVECTOR position;
	XMMATRIX worldMatrix;

	scale = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	rotation = XMQuaternionIdentity();
	position = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

	worldMatrix = XMMatrixTransformation(
		XMVectorZero(),
		XMQuaternionIdentity(),
		scale,
		XMVectorZero(),
		rotation,
		position);


	// ���b�Z�[�W���[�v�����s
	MSG msg = {};
	while (true) {
		auto immediateContext = Graphics::GetImmediateContext();
		// ��ʂ��N���A�[
		immediateContext->ClearRenderTargetView(
			Graphics::GetRenderTargetView(), GetInstance().clearColor);
		immediateContext->ClearDepthStencilView(
			Graphics::GetDepthStencilView(),	// �Ώۂ̐[�x�X�e���V���r���[
			D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, // �N���A����f�[�^�̃^�C�v
			1.0f,	// ���̒l�Ő[�x�o�b�t�@�[��������	
			0	// ���̒l�ŃX�e���V���o�b�t�@�[��������
		);
		// �����_�[�^�[�Q�b�g��ݒ�
		ID3D11RenderTargetView* renderTargetViews[] = {
			Graphics::GetRenderTargetView(),
		};
		immediateContext->OMSetRenderTargets(
			ARRAYSIZE(renderTargetViews),	// �o�C���h���郌���_�[�^�[�Q�b�g�̐�
			renderTargetViews,		// �f�o�C�X�Ƀo�C���h���郌���_�[�^�[�Q�b�g
			Graphics::GetDepthStencilView()	// �f�o�C�X�Ƀo�C���h����[�x�X�e���V���r���[
		);
		// �r���[�|�[�g��ݒ�
		D3D11_VIEWPORT viewports[] = {
			viewport,
		};
		immediateContext->RSSetViewports(
			ARRAYSIZE(viewports), // �o�C���h����r���[�|�[�g�̐�
			viewports	// �o�C���h����r���[�|�[�g
		);

		// �V�F�[�_�[��ݒ�
		immediateContext->VSSetShader(
			vertexShader->GetNativePointer(), // ���_�V�F�[�_�\�ւ̃|�C���^�[
			NULL,	// �N���X�C���X�^���X�C���^�[�t�F�C�X�̔z��ւ̃|�C���^�[
			0	// �N���X�C���X�^���X�C���^�[�t�F�C�X�̔z��̗v�f��
		);
		immediateContext->GSSetShader(geometryShader->GetNativePointer(), NULL, 0);
		immediateContext->PSSetShader(pixelShader->GetNativePointer(), NULL, 0);
		// ���̓A�Z���u���[�ɒ��_�o�b�t�@�[��ݒ�
		UINT stride = sizeof(VertexPosition);	// ���_�o�b�t�@��1���_���̗v�f�̃T�C�Y
		UINT offset = 0;	// ���_�o�b�t�@�̍ŏ��̗v�f�Ǝg�p�����ŏ��̒��_�o�b�t�@�̊Ԃ̃o�C�g��
		ID3D11Buffer* vertexBuffers[1];
		vertexBuffers[0] = vertexBuffer->GetNativePointer();
		// ���_�o�b�t�@�̔z���Input-Assembler(IA) stage�փo�C���h(D3D11�̃p�C�v���C���Q��)
		immediateContext->IASetVertexBuffers(
			0,	// �o�C���f�B���O����ŏ��̓��̓X���b�g
			ARRAYSIZE(vertexBuffers),	// �z����̒��_�o�b�t�@�̐�
			vertexBuffers,	// ���_�o�b�t�@�̔z��ւ̃|�C���^
			&stride,	// �X�g���C�h�l�ւ̔z��̃|�C���^
			&offset		// �I�t�Z�b�g�l�ւ̔z��̃|�C���^
		);
		// ���̓��C�A�E�g��ݒ�
		immediateContext->IASetInputLayout(inputLayout->GetNativePointer());
		// �v���~�e�B�u�̎�ނ�ݒ�
		immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		// �`��
		immediateContext->Draw(3, 0);


		// �o�b�N�o�b�t�@�[�ɕ`�悵���C���[�W���f�B�X�v���C�ɕ\��
		HRESULT hr = S_OK;
		hr = Graphics::GetSwapChain()->Present(1, 0);
		if (FAILED(hr))
		{
			MessageBox(GameWindow::GetHwnd(),
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
	SAFE_DELETE(inputLayout);
	SAFE_DELETE(vertexBuffer);
	SAFE_DELETE(vertexShader);
	SAFE_DELETE(geometryShader);
	SAFE_DELETE(pixelShader);

	Graphics::Release();

	return (int)msg.wParam;
}

// �A�v���P�[�V�������I�����܂��B
void Game::Quit()
{
	GameWindow::Close();
}



