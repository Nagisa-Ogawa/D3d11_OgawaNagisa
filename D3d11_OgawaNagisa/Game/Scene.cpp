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

using namespace std;
using namespace DX;
using namespace DirectX;

// ���̃N���X�̐V�����C���X�^���X���쐬���܂��B
Scene* Scene::Create(
	std::shared_ptr<GameWindow> window, std::shared_ptr<Graphics> graphics)
{
	return new Scene(window, graphics);
}

// ���̃N���X�̐V�����C���X�^���X�����������܂��B
Scene::Scene(
	std::shared_ptr<GameWindow> window, std::shared_ptr<Graphics> graphics)
{
	this->window = window;
	this->graphics = graphics;
}

// �V�[��������������ۂɌĂяo����܂��B
void Scene::Start()
{
	auto graphicsDevice = graphics->GetGraphicsDevice();

	// �萔�o�b�t�@�[���쐬
	constantBufferForCamera = ConstantBuffer::Create(
		graphicsDevice, sizeof(ConstantBufferDescForCamera));
	constantBufferForPerFrame = ConstantBuffer::Create(
		graphicsDevice, sizeof(ConstantBufferDescForPerFrame));

	// �J���� �I�u�W�F�N�g���쐬
	camera = Camera::Create(window);
	camera->Start();


	transform = Transform::Create();

	// �ʒu���W�݂̂������_�f�[�^�̌^
	struct VertexPosition
	{
		DirectX::XMFLOAT3 position;	// �ʒu���W
	};
	// ���_�f�[�^�ɂ��ē��͗v�f���`
	const D3D11_INPUT_ELEMENT_DESC inputElementDescs[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	// ���_�f�[�^
	const VertexPosition vertices[] = {
		{ {-0.5f, 0.5f, 0.0f }, },	// ���_0
		{ { 0.5f, 0.5f, 0.0f }, },	// ���_1
		{ { -0.5f,-0.5f, 0.0f }, },	// ���_2
		{ { 0.5f,-0.5f, 0.0f }, },	// ���_3
	};
	// ���_�o�b�t�@�[���쐬
	shared_ptr<VertexBuffer> vertexBuffers[] = {
		VertexBuffer::Create(graphicsDevice, vertices),
	};
	UINT strides[] = {
		sizeof(VertexPosition),
	};
	// �C���f�b�N�X �f�[�^
	uint32_t indices[] = {
		0, 1, 2,
		3, 2, 1,
	};
	auto indexBuffer = IndexBuffer::Create(graphicsDevice, indices);
	mesh = Mesh::Create(vertexBuffers, strides, indexBuffer, inputElementDescs);

	// �}�e���A�����쐬
	auto material = Material::Create(
		VertexShader::Create(graphicsDevice, BasicVertexShader),
		GeometryShader::Create(graphicsDevice, BasicGeometryShader),
		PixelShader::Create(graphicsDevice, BasicPixelShader));
	// �����_���[���쐬
	renderer = MeshRenderer::Create(graphicsDevice, mesh, material);
}

// �t���[�����X�V����ۂɌĂяo����܂��B
void Scene::Update(float time, float elapsedTime)
{
	// �g�����X�t�H�[�����X�V
	transform->scale = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	//transform->rotation = XMQuaternionIdentity();
	transform->rotation = XMQuaternionRotationRollPitchYaw(0.0f, 0.0f, time);
	transform->position = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
}

// �t���[����`�悷��ۂɌĂяo����܂��B
void Scene::Draw(float time, float elapsedTime)
{
	auto immediateContext = graphics->GetGraphicsDevice()->GetImmediateContext();

	// �����_�[�^�[�Q�b�g��ݒ�
	ID3D11RenderTargetView* renderTargetViews[] = {
		graphics->GetRenderTarget()->GetView().Get(),
	};
	immediateContext->OMSetRenderTargets(
		ARRAYSIZE(renderTargetViews), renderTargetViews,
		graphics->GetDepthStencil()->GetView().Get());

	// ��ʂ��N���A�[
	immediateContext->ClearRenderTargetView(
		graphics->GetRenderTarget()->GetView().Get(), camera->clearColor);
	immediateContext->ClearDepthStencilView(
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
		immediateContext->UpdateSubresource(
			constantBufferForCamera->GetNativePointer().Get(), 0, NULL, &srcData, 0, 0);
		ID3D11Buffer* const constantBuffers[] = {
			constantBufferForCamera->GetNativePointer().Get(),
		};
		immediateContext->VSSetConstantBuffers(
			0, ARRAYSIZE(constantBuffers), constantBuffers);
	}

	// �r���[�|�[�g��ݒ�
	D3D11_VIEWPORT viewports[] = {
		camera->viewport,
	};
	immediateContext->RSSetViewports(ARRAYSIZE(viewports), viewports);


	// ���[���h�ϊ��s����X�V
	auto worldMatrix = transform->GetWorldMatrix();
	{
		// �萔�o�b�t�@�[��ݒ�
		const ConstantBufferDescForPerFrame srcData = {
			worldMatrix,
		};
		// �T�u���\�[�X���X�V
		immediateContext->UpdateSubresource(
			constantBufferForPerFrame->GetNativePointer().Get(), 0, NULL, &srcData, 0, 0);
		ID3D11Buffer* const constantBuffers[] = {
			constantBufferForPerFrame->GetNativePointer().Get(),
		};
		immediateContext->VSSetConstantBuffers(
			1, ARRAYSIZE(constantBuffers), constantBuffers);
	}

	renderer->Draw(immediateContext);
}
