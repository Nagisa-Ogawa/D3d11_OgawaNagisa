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


GameScene::GameScene(
	std::shared_ptr<GameWindow> window,
	std::shared_ptr<Graphics> graphics,
	std::shared_ptr<Input> input) : Scene(window,graphics,input)
{
	
}

// シーンを初期化する際に呼び出されます。
void GameScene::Start()
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


	playerObject.reset(new PlayerObject(input));
	gameObjectB.reset(new GameObject(input));

	// 位置座標のみをもつ頂点データの型
	struct VertexPosition
	{
		DirectX::XMFLOAT3 position;	// 位置座標
		DirectX::XMFLOAT3 normal;	// 法線ベクトル
		DirectX::XMFLOAT2 texCoord;	// UV座標
	};
	// 頂点データについて入力要素を定義
	const D3D11_INPUT_ELEMENT_DESC inputElementDescs[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL" ,  0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"TEXCOORD" , 0, DXGI_FORMAT_R32G32_FLOAT,       0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	// 頂点データの配列
	// 横の分割数
	const int u_max = 64;
	// 縦の分割数
	const int v_max = 64;
	// 半径
	const float radius = 1.0f;
	const int vertex_num = (u_max + 1) * (v_max + 1);
	// UV座標のオフセット
	const float uOffset = 1.0f / u_max;
	const float vOffset = 1.0f / v_max;
	// 現在のUV座標
	float nowUPos = 0.0f;
	float nowVPos = 0.0f;

	// 頂点データ
	VertexPosition* vertices = new VertexPosition[vertex_num];

	for (int v = 0; v <= v_max; v++) {
		for (int u = 0; u <= u_max; u++) {
			// 0<=theta<=π
			float theta = XM_PI * v / v_max;
			// 0<=phi<2π
			float phi = XM_2PI * u / u_max;
			if (u == u_max)
			{

				float phi = XM_2PI * 0 / u_max;
				vertices[(u_max + 1) * v + u].position.x = radius * XMScalarSin(theta) * XMScalarCos(phi);
				vertices[(u_max + 1) * v + u].position.y = radius * XMScalarCos(theta);
				vertices[(u_max + 1) * v + u].position.z = radius * XMScalarSin(theta) * XMScalarSin(phi);
				// 法線
				vertices[(u_max + 1) * v + u].normal.x = XMScalarSin(theta) * XMScalarCos(phi);
				vertices[(u_max + 1) * v + u].normal.y = XMScalarCos(theta);
				vertices[(u_max + 1) * v + u].normal.z = XMScalarSin(theta) * XMScalarSin(phi);
				// UV座標
				vertices[(u_max + 1) * v + u].texCoord = { nowUPos,nowVPos };
			}
			else
			{
				// 三次元極座標から三次元直交座標への変換公式より(yとzを入れ替え)
				// x = r * sinθ * cosφ
				// y = r * cosθ
				// z = r * sinθ * sinφ
				vertices[(u_max + 1) * v + u].position.x = radius * XMScalarSin(theta) * XMScalarCos(phi);
				vertices[(u_max + 1) * v + u].position.y = radius * XMScalarCos(theta);
				vertices[(u_max + 1) * v + u].position.z = radius * XMScalarSin(theta) * XMScalarSin(phi);
				// 法線
				vertices[(u_max + 1) * v + u].normal.x = XMScalarSin(theta) * XMScalarCos(phi);
				vertices[(u_max + 1) * v + u].normal.y = XMScalarCos(theta);
				vertices[(u_max + 1) * v + u].normal.z = XMScalarSin(theta) * XMScalarSin(phi);
				// UV座標
				vertices[(u_max + 1) * v + u].texCoord = { nowUPos,nowVPos };

			}
			nowUPos += uOffset;
		}
		nowUPos = 0.0f;
		nowVPos += vOffset;
	}

	// 頂点バッファーを作成
	shared_ptr<VertexBuffer> vertexBuffers[] = {
		shared_ptr<VertexBuffer>(new VertexBuffer(graphicsDevice, vertices,sizeof(VertexPosition)*vertex_num))
	};

	delete[] vertices;
	UINT strides[] = {
		sizeof(VertexPosition),
	};
	// インデックスデータの要素数
	const int index_num = (v_max * u_max) * 6;
	//インデックスデータの配列
	UINT32* indices = new UINT32[index_num];
	for (int v = 0; v < v_max; v++) {
		// 縦方向のオフセット
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

	shared_ptr<IndexBuffer> indexBuffer(new IndexBuffer(graphicsDevice, indices,sizeof(UINT)*index_num, DXGI_FORMAT_R32_UINT));

	delete[] indices;
	playerObject->mesh.reset(new Mesh(vertexBuffers, strides, indexBuffer, inputElementDescs));
	gameObjectB->mesh.reset(new Mesh(vertexBuffers, strides, indexBuffer, inputElementDescs));


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
	texture.reset(new Texture2D(graphicsDevice, L"../resource/image/earth.png"));


	// レンダラーを作成
	playerObject->renderer.reset(new MeshRenderer(graphicsDevice, playerObject->mesh, material,texture));
	gameObjectB->renderer.reset(new MeshRenderer(graphicsDevice, gameObjectB->mesh, material, texture));
}

// フレームを更新する際に呼び出されます。
void GameScene::Update(float time, float elapsedTime)
{
	playerObject->Update(time, elapsedTime);
	gameObjectB->Update(time, elapsedTime);
}

// フレームを描画する際に呼び出されます。
void GameScene::Draw(float time, float elapsedTime)
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
			playerObject->GetTransform()->GetWorldMatrix(),
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
	}
	{
		// オブジェクトAを描画
		playerObject->Draw(deviceContext.Get(), time, elapsedTime);

		const ConstantBufferDescForPerFrame srcDataB = {
		gameObjectB->GetTransform()->GetWorldMatrix(),
		};
		deviceContext->UpdateSubresource(
			constantBufferForPerFrame->GetNativePointer().Get(), 0, NULL, &srcDataB, 0, 0);
		ID3D11Buffer* const constantBuffers[] = {
			constantBufferForPerFrame->GetNativePointer().Get(),
		};
		deviceContext->VSSetConstantBuffers(
			1, ARRAYSIZE(constantBuffers), constantBuffers);

		gameObjectB->Draw(deviceContext.Get(), time, elapsedTime);

	}


}

void GameScene::End() {

}
