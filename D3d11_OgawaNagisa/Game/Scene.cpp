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

// このクラスの新しいインスタンスを作成します。
Scene* Scene::Create(
	std::shared_ptr<GameWindow> window, std::shared_ptr<Graphics> graphics)
{
	return new Scene(window, graphics);
}

// このクラスの新しいインスタンスを初期化します。
Scene::Scene(
	std::shared_ptr<GameWindow> window, std::shared_ptr<Graphics> graphics)
{
	this->window = window;
	this->graphics = graphics;
}

// シーンを初期化する際に呼び出されます。
void Scene::Start()
{
	auto graphicsDevice = graphics->GetGraphicsDevice();

	// 定数バッファーを作成
	constantBufferForCamera = ConstantBuffer::Create(
		graphicsDevice, sizeof(ConstantBufferDescForCamera));
	constantBufferForPerFrame = ConstantBuffer::Create(
		graphicsDevice, sizeof(ConstantBufferDescForPerFrame));

	// カメラ オブジェクトを作成
	camera = Camera::Create(window);
	camera->Start();


	transform = Transform::Create();

	// 位置座標のみをもつ頂点データの型
	struct VertexPosition
	{
		DirectX::XMFLOAT3 position;	// 位置座標
	};
	// 頂点データについて入力要素を定義
	const D3D11_INPUT_ELEMENT_DESC inputElementDescs[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	// 頂点データ
	const VertexPosition vertices[] = {
		{ {-0.5f, 0.5f, 0.0f }, },	// 頂点0
		{ { 0.5f, 0.5f, 0.0f }, },	// 頂点1
		{ { -0.5f,-0.5f, 0.0f }, },	// 頂点2
		{ { 0.5f,-0.5f, 0.0f }, },	// 頂点3
	};
	// 頂点バッファーを作成
	shared_ptr<VertexBuffer> vertexBuffers[] = {
		VertexBuffer::Create(graphicsDevice, vertices),
	};
	UINT strides[] = {
		sizeof(VertexPosition),
	};
	// インデックス データ
	uint32_t indices[] = {
		0, 1, 2,
		3, 2, 1,
	};
	auto indexBuffer = IndexBuffer::Create(graphicsDevice, indices);
	mesh = Mesh::Create(vertexBuffers, strides, indexBuffer, inputElementDescs);

	// マテリアルを作成
	auto material = Material::Create(
		VertexShader::Create(graphicsDevice, BasicVertexShader),
		GeometryShader::Create(graphicsDevice, BasicGeometryShader),
		PixelShader::Create(graphicsDevice, BasicPixelShader));
	// レンダラーを作成
	renderer = MeshRenderer::Create(graphicsDevice, mesh, material);
}

// フレームを更新する際に呼び出されます。
void Scene::Update(float time, float elapsedTime)
{
	// トランスフォームを更新
	transform->scale = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	//transform->rotation = XMQuaternionIdentity();
	transform->rotation = XMQuaternionRotationRollPitchYaw(0.0f, 0.0f, time);
	transform->position = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
}

// フレームを描画する際に呼び出されます。
void Scene::Draw(float time, float elapsedTime)
{
	auto immediateContext = graphics->GetGraphicsDevice()->GetImmediateContext();

	// レンダーターゲットを設定
	ID3D11RenderTargetView* renderTargetViews[] = {
		graphics->GetRenderTarget()->GetView().Get(),
	};
	immediateContext->OMSetRenderTargets(
		ARRAYSIZE(renderTargetViews), renderTargetViews,
		graphics->GetDepthStencil()->GetView().Get());

	// 画面をクリアー
	immediateContext->ClearRenderTargetView(
		graphics->GetRenderTarget()->GetView().Get(), camera->clearColor);
	immediateContext->ClearDepthStencilView(
		graphics->GetDepthStencil()->GetView().Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// ビューとプロジェクションの変換行列を取得
	auto viewMatrix = camera->GetViewMatrix();
	auto projectionMatrix = camera->GetProjectionMatrix();
	{
		// 定数バッファーを設定
		const ConstantBufferDescForCamera srcData = {
			viewMatrix,
			projectionMatrix,
		};
		// サブリソースを更新
		immediateContext->UpdateSubresource(
			constantBufferForCamera->GetNativePointer().Get(), 0, NULL, &srcData, 0, 0);
		ID3D11Buffer* const constantBuffers[] = {
			constantBufferForCamera->GetNativePointer().Get(),
		};
		immediateContext->VSSetConstantBuffers(
			0, ARRAYSIZE(constantBuffers), constantBuffers);
	}

	// ビューポートを設定
	D3D11_VIEWPORT viewports[] = {
		camera->viewport,
	};
	immediateContext->RSSetViewports(ARRAYSIZE(viewports), viewports);


	// ワールド変換行列を更新
	auto worldMatrix = transform->GetWorldMatrix();
	{
		// 定数バッファーを設定
		const ConstantBufferDescForPerFrame srcData = {
			worldMatrix,
		};
		// サブリソースを更新
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
