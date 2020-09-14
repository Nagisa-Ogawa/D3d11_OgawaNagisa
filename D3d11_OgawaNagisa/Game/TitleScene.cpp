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

// シーンを初期化する際に呼び出されます。
void TitleScene::Start()
{
	auto graphicsDevice = graphics->GetGraphicsDevice();

	// 定数バッファーを作成
	constantBufferForCamera.reset(
		new ConstantBuffer(graphicsDevice, sizeof(ConstantBufferDescForCamera)));
	//constantBufferForLighting.reset(
	//	new ConstantBuffer(graphicsDevice, sizeof(ConstantBufferDescForLighting)));
	constantBufferForCustomLighting.reset(
		new ConstantBuffer(graphicsDevice, sizeof(ConstantBufferDescForCustomLighting)));
	constantBufferForPerFrame.reset(
		new ConstantBuffer(graphicsDevice, sizeof(ConstantBufferDescForPerFrame)));

	// カメラ オブジェクトを作成
	camera.reset(new Camera(window));
	camera->Start();


	titleObj.reset(new GameObject(input));

	ReadModel readModel;
	readModel.ReadModelData("human.obj");

	//// 位置座標のみをもつ頂点データの型
	//struct VertexPosition
	//{
	//	DirectX::XMFLOAT3 position;	// 位置座標
	//	DirectX::XMFLOAT3 normal;	// 法線ベクトル
	//	DirectX::XMFLOAT2 texCoord;	// UV座標
	//};

	// 頂点データについて入力要素を定義
	const D3D11_INPUT_ELEMENT_DESC inputElementDescs[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL" ,  0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"TEXCOORD" , 0, DXGI_FORMAT_R32G32_FLOAT,       0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	//// 頂点データ
	//VertexPosition vertices[] =
	//{
	//	{ {-3.0f, 3.0f,0.0f},{0.0f,0.0f,-1.0f},{0.0f,0.0f} },
	//	{ { 3.0f, 3.0f,0.0f},{0.0f,0.0f,-1.0f},{1.0f,0.0f} },
	//	{ {-3.0f,-3.0f,0.0f},{0.0f,0.0f,-1.0f},{0.0f,1.0f} },
	//	{ { 3.0f,-3.0f,0.0f},{0.0f,0.0f,-1.0f},{1.0f,1.0f} },
	//};

	// 頂点バッファーを作成
	shared_ptr<VertexBuffer> vertexBuffers[] = {
		shared_ptr<VertexBuffer>(new VertexBuffer(
			graphicsDevice, readModel.GetVertexData(),readModel.GetVerticesSize())
		)
	};

	UINT strides[] = {
		sizeof(ReadModel::VertexPosition),
	};
	////インデックスデータの配列
	//UINT32 indices[] =
	//{
	//	0,1,2,
	//	3,2,1
	//};

	shared_ptr<IndexBuffer> indexBuffer(
		new IndexBuffer(
			graphicsDevice, 
			readModel.GetIndexData(),readModel.GetIndicesSize(),
			DXGI_FORMAT_R32_UINT
		)
	);

	titleObj->mesh.reset(new Mesh(vertexBuffers, strides, indexBuffer, inputElementDescs));


	// マテリアルを作成
	shared_ptr<Material> material(
		new Material(
			shared_ptr<VertexShader>(
				new VertexShader(graphicsDevice, g_BasicVertexShader)),
			shared_ptr<GeometryShader>(
				new GeometryShader(graphicsDevice, g_BasicGeometryShader)),
			shared_ptr<PixelShader>(
				new PixelShader(graphicsDevice, g_CustomPixelShader))));


	std::shared_ptr<Texture2D> texture;
	// テクスチャーを読み込み
	texture.reset(new Texture2D(graphicsDevice, L"../resource/image/test.png"));


	// レンダラーを作成
	titleObj->renderer.reset(new MeshRenderer(graphicsDevice, titleObj->mesh, material, texture));
}

// フレームを更新する際に呼び出されます。
void TitleScene::Update(float time, float elapsedTime)
{
	if (input->GetKeyDown(VK_RETURN)) {
		std::shared_ptr<GameScene> gameScene(new GameScene(window, graphics, input));
		SceneManager::GetInstance().ChangeScene(gameScene);
	}
	titleObj->Update(time, elapsedTime);
}

// フレームを描画する際に呼び出されます。
void TitleScene::Draw(float time, float elapsedTime)
{
	auto deviceContext = graphics->GetGraphicsDevice()->GetImmediateContext();

	// レンダーターゲットを設定
	ID3D11RenderTargetView* renderTargetViews[] = {
		graphics->GetRenderTarget()->GetView().Get(),
	};
	deviceContext->OMSetRenderTargets(
		ARRAYSIZE(renderTargetViews), renderTargetViews,
		graphics->GetDepthStencil()->GetView().Get());

	// 画面をクリアー
	deviceContext->ClearRenderTargetView(
		graphics->GetRenderTarget()->GetView().Get(), camera->clearColor);
	deviceContext->ClearDepthStencilView(
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
		deviceContext->UpdateSubresource(
			constantBufferForCamera->GetNativePointer().Get(), 0, NULL, &srcData, 0, 0);
		ID3D11Buffer* const constantBuffers[] = {
			constantBufferForCamera->GetNativePointer().Get(),
		};
		deviceContext->VSSetConstantBuffers(
			0, ARRAYSIZE(constantBuffers), constantBuffers);
	}

	// ライティング用の定数バッファーを更新
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
		// 定数バッファーを設定
		const ConstantBufferDescForCustomLighting srcData =
		{
			lightDesc,
			materialDesc,
			cameraPosition,
		};
		// サブリソースを更新
		deviceContext->UpdateSubresource(
			constantBufferForCustomLighting->GetNativePointer().Get(), 0, NULL, &srcData, 0, 0);
		ID3D11Buffer* const constantBuffers[] = {
			constantBufferForCustomLighting->GetNativePointer().Get(),
		};
		deviceContext->PSSetConstantBuffers(
			0, ARRAYSIZE(constantBuffers), constantBuffers);
	}

	// ビューポートを設定
	D3D11_VIEWPORT viewports[] = {
		camera->viewport,
	};
	deviceContext->RSSetViewports(ARRAYSIZE(viewports), viewports);

	// ゲームオブジェクトを描画
	{
		// ワールド変換行列を更新
		const ConstantBufferDescForPerFrame srcDataA = {
			titleObj->GetTransform()->GetWorldMatrix(),
		};
		// サブリソースを更新
		deviceContext->UpdateSubresource(
			constantBufferForPerFrame->GetNativePointer().Get(), 0, NULL, &srcDataA, 0, 0);

		// 定数バッファーを設定
		ID3D11Buffer* const constantBuffers[] = {
			constantBufferForPerFrame->GetNativePointer().Get(),
		};
		deviceContext->VSSetConstantBuffers(
			1, ARRAYSIZE(constantBuffers), constantBuffers);
		// オブジェクトAを描画
		titleObj->Draw(deviceContext.Get(), time, elapsedTime);
	}
}

void TitleScene::End() {
}
