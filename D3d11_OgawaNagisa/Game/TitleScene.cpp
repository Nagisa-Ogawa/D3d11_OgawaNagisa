//=============================================================================
// TitleScene.cpp
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
#include "ReadModel.h"

using namespace std;
using namespace DirectX;
using namespace DX;


TitleScene::TitleScene(
	std::shared_ptr<GameWindow> window,
	std::shared_ptr<Graphics> graphics,
	std::shared_ptr<Input> input) : Scene(window, graphics, input)
{

}

// �V�[��������������ۂɌĂяo����܂��B
void TitleScene::Start()
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


	titleObj.reset(new GameObject(input));

	// ���_�f�[�^�ɂ��ē��͗v�f���`
	const D3D11_INPUT_ELEMENT_DESC inputElementDescs[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL" ,  0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"TEXCOORD" , 0, DXGI_FORMAT_R32G32_FLOAT,       0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	// ���_�f�[�^
	ReadModel::VertexPosition vertices[] =
	{
		{ {-3.0f, 3.0f,0.0f},{0.0f,0.0f,-1.0f},{0.0f,0.0f} },
		{ { 3.0f, 3.0f,0.0f},{0.0f,0.0f,-1.0f},{1.0f,0.0f} },
		{ {-3.0f,-3.0f,0.0f},{0.0f,0.0f,-1.0f},{0.0f,1.0f} },
		{ { 3.0f,-3.0f,0.0f},{0.0f,0.0f,-1.0f},{1.0f,1.0f} },
	};

	// ���_�o�b�t�@�[���쐬
	shared_ptr<VertexBuffer> vertexBuffers[] = {
		shared_ptr<VertexBuffer>(new VertexBuffer(
			graphicsDevice, vertices)
		)
	};

	UINT strides[] = {
		sizeof(ReadModel::VertexPosition),
	};
	//�C���f�b�N�X�f�[�^�̔z��
	UINT32 indices[] =
	{
		0,1,2,
		3,2,1
	};

	shared_ptr<IndexBuffer> indexBuffer(
		new IndexBuffer(
			graphicsDevice, 
			indices
		)
	);

	titleObj->mesh.reset(new Mesh(vertexBuffers, strides, indexBuffer, inputElementDescs));


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
	texture.reset(new Texture2D(graphicsDevice, L"../resource/image/title.png"));


	// �����_���[���쐬
	titleObj->renderer.reset(new MeshRenderer(graphicsDevice, titleObj->mesh, material, texture));
}

// �t���[�����X�V����ۂɌĂяo����܂��B
std::shared_ptr<Scene> TitleScene::Update(float time, float elapsedTime)
{
	if (input->GetMouseButtonDown(0)) {
		std::shared_ptr<GameScene> gameScene(new GameScene(window, graphics, input));
		return gameScene;
	}
	titleObj->Update(time, elapsedTime);
	
	return SceneManager::GetInstance().GetScene();
}

// �t���[����`�悷��ۂɌĂяo����܂��B
void TitleScene::Draw(float time, float elapsedTime)
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
		const ConstantBufferDescForPerFrame srcDataA = {
			titleObj->GetTransform()->GetWorldMatrix(),
		};
		// �T�u���\�[�X���X�V
		deviceContext->UpdateSubresource(
			constantBufferForPerFrame->GetNativePointer().Get(), 0, NULL, &srcDataA, 0, 0);

		// �萔�o�b�t�@�[��ݒ�
		ID3D11Buffer* const constantBuffers[] = {
			constantBufferForPerFrame->GetNativePointer().Get(),
		};
		deviceContext->VSSetConstantBuffers(
			1, ARRAYSIZE(constantBuffers), constantBuffers);
		// �I�u�W�F�N�gA��`��
		titleObj->Draw(deviceContext.Get(), time, elapsedTime);
	}
}

void TitleScene::End() {
}
