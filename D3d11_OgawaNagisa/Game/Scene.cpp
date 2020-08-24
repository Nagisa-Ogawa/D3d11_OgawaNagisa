//=============================================================================
// Scene.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "Game.h"
#include "DirectXHelper.h"
#include "BasicVertexShader.h"
#include "BasicGeometryShader.h"
#include "BasicPixelShader.h"
#include "PhongPixelShader.h"
#include "CustomPixelShader.h"

using namespace std;
using namespace DirectX;
using namespace DX;

// ���̃N���X�̐V�����C���X�^���X�����������܂��B
Scene::Scene(
	std::shared_ptr<GameWindow> window,
	std::shared_ptr<Graphics> graphics,
	std::shared_ptr<Input> input)
{
	this->window = window;
	this->graphics = graphics;
	this->input = input;
}

// �V�[��������������ۂɌĂяo����܂��B
void Scene::Start()
{
	auto graphicsDevice = graphics->GetGraphicsDevice();

	// �萔�o�b�t�@�[���쐬
	constantBufferForCamera.reset(
		new ConstantBuffer(graphicsDevice, sizeof(ConstantBufferDescForCamera)));
	//constantBufferForLighting.reset(
	//	new ConstantBuffer(graphicsDevice, sizeof(ConstantBufferDescForLighting)));
	constantBufferForCustomLighting.reset(
		new ConstantBuffer(graphicsDevice, sizeof(ConstantBufferDescForCustomLighting)));
	constantBufferForPerFrame.reset(
		new ConstantBuffer(graphicsDevice, sizeof(ConstantBufferDescForPerFrame)));

	// �J���� �I�u�W�F�N�g���쐬
	camera.reset(new Camera(window));
	camera->Start();


	gameObject.reset(new GameObject(input));

	// �ʒu���W�݂̂������_�f�[�^�̌^
	struct VertexPosition
	{
		DirectX::XMFLOAT3 position;	// �ʒu���W
		DirectX::XMFLOAT3 normal;	// �@���x�N�g��
		DirectX::XMFLOAT2 texCoord;	// UV���W
	};
	// ���_�f�[�^�ɂ��ē��͗v�f���`
	const D3D11_INPUT_ELEMENT_DESC inputElementDescs[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL" ,  0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"TEXCOORD" , 0, DXGI_FORMAT_R32G32_FLOAT,       0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	// ���_�f�[�^�̔z��
	// ���̕�����
	const int u_max = 64;
	// �c�̕�����
	const int v_max = 64;
	// ���a
	const float radius = 1.0f;
	const int vertex_num = (u_max + 1) * (v_max + 1);
	// UV���W�̃I�t�Z�b�g
	const float uOffset = 1.0f / u_max;
	const float vOffset = 1.0f / v_max;
	// ���݂�UV���W
	float nowUPos = 0.0f;
	float nowVPos = 0.0f;

	// ���_�f�[�^
	VertexPosition vertices[vertex_num];

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

	// ���_�o�b�t�@�[���쐬
	shared_ptr<VertexBuffer> vertexBuffers[] = {
		shared_ptr<VertexBuffer>(new VertexBuffer(graphicsDevice, vertices)),
	};
	UINT strides[] = {
		sizeof(VertexPosition),
	};
	// �C���f�b�N�X�f�[�^�̗v�f��
	const int index_num = (v_max * u_max) * 6;
	//�C���f�b�N�X�f�[�^�̔z��
	UINT32 indices[index_num];
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


	shared_ptr<IndexBuffer> indexBuffer(new IndexBuffer(graphicsDevice, indices));
	gameObject->mesh.reset(new Mesh(vertexBuffers, strides, indexBuffer, inputElementDescs));

	// �}�e���A�����쐬
	shared_ptr<Material> material(
		new Material(
			shared_ptr<VertexShader>(
				new VertexShader(graphicsDevice, g_BasicVertexShader)),
			shared_ptr<GeometryShader>(
				new GeometryShader(graphicsDevice, g_BasicGeometryShader)),
			shared_ptr<PixelShader>(
				new PixelShader(graphicsDevice, g_CustomPixelShader))));


	std::shared_ptr<Texture2D> texture;
	// �e�N�X�`���[��ǂݍ���
	texture.reset(new Texture2D(graphicsDevice, L"../resource/image/earth.png"));


	// �����_���[���쐬
	gameObject->renderer.reset(new MeshRenderer(graphicsDevice, gameObject->mesh, material,texture));
}

// �t���[�����X�V����ۂɌĂяo����܂��B
void Scene::Update(float time, float elapsedTime)
{
	gameObject->Update(time, elapsedTime);
}

// �t���[����`�悷��ۂɌĂяo����܂��B
void Scene::Draw(float time, float elapsedTime)
{
	auto deviceContext = graphics->GetGraphicsDevice()->GetImmediateContext();

	// �����_�[�^�[�Q�b�g��ݒ�
	ID3D11RenderTargetView* renderTargetViews[] = {
		graphics->GetRenderTarget()->GetView().Get(),
	};
	deviceContext->OMSetRenderTargets(
		ARRAYSIZE(renderTargetViews), renderTargetViews,
		graphics->GetDepthStencil()->GetView().Get());

	// ��ʂ��N���A�[
	deviceContext->ClearRenderTargetView(
		graphics->GetRenderTarget()->GetView().Get(), camera->clearColor);
	deviceContext->ClearDepthStencilView(
		graphics->GetDepthStencil()->GetView().Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// �r���[�ƃv���W�F�N�V�����̕ϊ��s����擾
	auto viewMatrix = camera->GetViewMatrix();
	auto projectionMatrix = camera->GetProjectionMatrix();
	{
		// �萔�o�b�t�@�[��ݒ�
		const ConstantBufferDescForCamera srcData = {
			viewMatrix,
			projectionMatrix,
		};
		// �T�u���\�[�X���X�V
		deviceContext->UpdateSubresource(
			constantBufferForCamera->GetNativePointer().Get(), 0, NULL, &srcData, 0, 0);
		ID3D11Buffer* const constantBuffers[] = {
			constantBufferForCamera->GetNativePointer().Get(),
		};
		deviceContext->VSSetConstantBuffers(
			0, ARRAYSIZE(constantBuffers), constantBuffers);
	}

	// ���C�e�B���O�p�̒萔�o�b�t�@�[���X�V
	{
		
		const LightingDesc lightDesc =
		{
			{ -0.5f, 0.5f, -1.0f, 0.0f },
			{ 1.0f, 1.0f, 1.0f, 0.0f },
		};
		const MaterialDesc materialDesc =
		{
			{ 1.0f, 1.0f, 1.0,  },
			0.0f,
			{1.0f,1.0f,1.0f},
			0.5,
		};
		XMFLOAT4 cameraPosition;
		XMStoreFloat4(&cameraPosition, camera->eyePosition);
		// �萔�o�b�t�@�[��ݒ�
		const ConstantBufferDescForCustomLighting srcData =
		{
			lightDesc,
			materialDesc,
			cameraPosition,
		};
		// �T�u���\�[�X���X�V
		deviceContext->UpdateSubresource(
			constantBufferForCustomLighting->GetNativePointer().Get(), 0, NULL, &srcData, 0, 0);
		ID3D11Buffer* const constantBuffers[] = {
			constantBufferForCustomLighting->GetNativePointer().Get(),
		};
		deviceContext->PSSetConstantBuffers(
			0, ARRAYSIZE(constantBuffers), constantBuffers);
	}

	// �r���[�|�[�g��ݒ�
	D3D11_VIEWPORT viewports[] = {
		camera->viewport,
	};
	deviceContext->RSSetViewports(ARRAYSIZE(viewports), viewports);

	// �Q�[���I�u�W�F�N�g��`��
	{
		// ���[���h�ϊ��s����X�V
		const ConstantBufferDescForPerFrame srcData = {
			gameObject->GetTransform()->GetWorldMatrix(),
		};
		// �T�u���\�[�X���X�V
		deviceContext->UpdateSubresource(
			constantBufferForPerFrame->GetNativePointer().Get(), 0, NULL, &srcData, 0, 0);
		// �萔�o�b�t�@�[��ݒ�
		ID3D11Buffer* const constantBuffers[] = {
			constantBufferForPerFrame->GetNativePointer().Get(),
		};
		deviceContext->VSSetConstantBuffers(
			1, ARRAYSIZE(constantBuffers), constantBuffers);
	}

	gameObject->Draw(deviceContext.Get(), time, elapsedTime);
}
