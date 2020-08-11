//=============================================================================
// Game.cpp
// 
//=============================================================================
#include "stdafx.h"
#include <DirectXMath.h>	// DirectXの算術ライブラリー
#include <d3dcompiler.h>
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

// このクラスのシングルトンインスタンスを取得します。
Game& Game::GetInstance()
{
	static Game instance;
	return instance;
}


// アプリケーションを初期化します。
void Game::Initialize(LPCWSTR windowTitle, int screenWidth, int screenHeight)
{
	GetInstance().OnInitialize(windowTitle, screenWidth, screenHeight);
}

// 初期化の際に呼び出されます。
void Game::OnInitialize(LPCWSTR windowTitle, int screenWidth, int screenHeight)
{
	if (isInitialized) {
		return;
	}

	// ウィンドウを作成
	window = GameWindow::Create(windowTitle, screenWidth, screenHeight);
	// グラフィックデバイスを作成
	graphics = Graphics::Create(window);

	isInitialized = true;
}

// メッセージループを実行します。
int Game::Run()
{
	{
		//// 頂点データの配列
		//// 横の分割数
		//const int u_max = 64;
		//// 縦の分割数
		//const int v_max = 64;
		//// 半径
		//const float radius = 3.0f;
		//const int vertex_num = (u_max + 1) * (v_max + 1);
		//// UV座標のオフセット
		//const float uOffset = 1.0f / u_max;
		//const float vOffset = 1.0f / v_max;
		//// 現在のUV座標
		//float nowUPos = 0.0f;
		//float nowVPos = 0.0f;

		//VertexPositionNormalTexture* vertices = new VertexPositionNormalTexture[vertex_num];

		//for (int v = 0; v <= v_max; v++) {
		//	for (int u = 0; u <= u_max; u++) {
		//		// 0<=theta<=π
		//		float theta = XM_PI * v / v_max;
		//		// 0<=phi<2π
		//		float phi = XM_2PI * u / u_max;
		//		if (u == u_max)
		//		{

		//			float phi = XM_2PI * 0 / u_max;
		//			vertices[(u_max + 1) * v + u].position.x = radius * XMScalarSin(theta) * XMScalarCos(phi);
		//			vertices[(u_max + 1) * v + u].position.y = radius * XMScalarCos(theta);
		//			vertices[(u_max + 1) * v + u].position.z = radius * XMScalarSin(theta) * XMScalarSin(phi);
		//			// 法線
		//			vertices[(u_max + 1) * v + u].normal.x = XMScalarSin(theta) * XMScalarCos(phi);
		//			vertices[(u_max + 1) * v + u].normal.y = XMScalarCos(theta);
		//			vertices[(u_max + 1) * v + u].normal.z = XMScalarSin(theta) * XMScalarSin(phi);
		//			// UV座標
		//			vertices[(u_max + 1) * v + u].texCoord = { nowUPos,nowVPos };
		//		}
		//		else
		//		{
		//			// 三次元極座標から三次元直交座標への変換公式より(yとzを入れ替え)
		//			// x = r * sinθ * cosφ
		//			// y = r * cosθ
		//			// z = r * sinθ * sinφ
		//			vertices[(u_max + 1) * v + u].position.x = radius * XMScalarSin(theta) * XMScalarCos(phi);
		//			vertices[(u_max + 1) * v + u].position.y = radius * XMScalarCos(theta);
		//			vertices[(u_max + 1) * v + u].position.z = radius * XMScalarSin(theta) * XMScalarSin(phi);
		//			// 法線
		//			vertices[(u_max + 1) * v + u].normal.x = XMScalarSin(theta) * XMScalarCos(phi);
		//			vertices[(u_max + 1) * v + u].normal.y = XMScalarCos(theta);
		//			vertices[(u_max + 1) * v + u].normal.z = XMScalarSin(theta) * XMScalarSin(phi);
		//			// UV座標
		//			vertices[(u_max + 1) * v + u].texCoord = { nowUPos,nowVPos };

		//		}
		//		nowUPos += uOffset;
		//	}
		//	nowUPos = 0.0f;
		//	nowVPos += vOffset;
		//}

		//	// 頂点データの配列
		////VertexPositionNormalTexture vertices[] = {
		//	//{ {-2.0f, 2.0f,0.0f }, { 0.0f, 0.0f,-1.0f }, { 0.0f, 0.0f } },
		//	//{ { 2.0f, 2.0f,0.0f }, { 0.0f, 0.0f,-1.0f }, { 1.0f, 0.0f } },
		//	//{ {-2.0f,-2.0f,0.0f }, { 0.0f, 0.0f,-1.0f }, { 0.0f, 1.0f } },
		//	//{ { 2.0f,-2.0f,0.0f }, { 0.0f, 0.0f,-1.0f }, { 1.0f, 1.0f } },
		////};


		//// 頂点バッファーを作成
		//VertexBuffer* vertexBuffer = nullptr;
		//vertexBuffer = VertexBuffer::Create(graphicsDevice, sizeof(VertexPositionNormalTexture) * vertex_num);
		//if (vertexBuffer == nullptr)
		//{
		//	OutputDebugString(L"頂点バッファーを作成できませんでした。");
		//	return -1;
		//}
		//// リソースを更新
		//vertexBuffer->SetData(vertices);

		//delete[] vertices;

		//// インデックスデータの要素数
		//const int index_num = (v_max * u_max) * 6;
		////インデックスデータの配列
		//UINT32* indices = new UINT32[index_num];

		//for (int v = 0; v < v_max; v++) {
		//	// 縦方向のオフセット
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

		//	// インデックスデータの配列
		////UINT32 indices[] = {
		////	 0,  1,  2,  3,  2,  1,	// Front
		////	// 4,  5,  6,  7,  6,  5,	// Back
		////	// 8,  9, 10, 11, 10,  9,	// Right
		////	//12, 13, 14, 15, 14, 13,	// Left
		////	//16, 17, 18, 19, 18, 17,	// Top
		////	//20, 21, 22, 23, 22, 21,	// Bottom
		////};


		//// インデックスの数
		//UINT indexCount = index_num;

		//// インデックスバッファーを作成
		//IndexBuffer* indexBuffer = nullptr;
		//indexBuffer = IndexBuffer::Create(graphicsDevice, indexCount);
		//if (indexBuffer == nullptr) {
		//	return -1;
		//}
		//// リソースを更新
		//indexBuffer->SetData(indices);

		//delete[] indices;

		//// 平行光源の情報
		//struct ConstantDirectionalLight {
		//	XMFLOAT4 direction = XMFLOAT4(-0.5f, 0.5f, -1.0f, 0.0f);
		//	XMFLOAT4 color = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f);
		//};

		//// 物体表面の質感
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
		//	// 視点座標
		//	XMFLOAT4 viewPosition = XMFLOAT4(0, 1, -10, 1);
		//	// 光源
		//	ConstantDirectionalLight constantDirectionalLight;
		//	// 物体の質感
		//	ConstantMaterial material;
		//};

		//// 定数バッファーを作成
		//ConstantBuffer* constantBuffer = nullptr;
		//constantBuffer = ConstantBuffer::Create(graphicsDevice, sizeof(ConstantMatricesBuffer));
		//if (constantBuffer == nullptr)
		//{
		//	OutputDebugString(L"定数バッファーを作成できませんでした。");
		//	return -1;
		//}



		//// 画像のダミー
		////const uint32_t source[] = {
		////	0xFF00FFFF, 0xFF000000, 0xFF00FFFF, 0xFF000000,
		////	0xFF000000, 0xFF00FFFF, 0xFF000000, 0xFF00FFFF,
		////	0xFF00FFFF, 0xFF000000, 0xFF00FFFF, 0xFF000000,
		////	0xFF000000, 0xFF00FFFF, 0xFF000000, 0xFF00FFFF,
		////};
		//// rawデータの読み込み
		//{
		//	//FILE* fp;

		//	//fopen_s(&fp,"earth.data", "rb");

		//	//if (fp == NULL)
		//	//{
		//	//	printf("失敗");
		//	//	return -1;
		//	//}

		//	//fread(source, sizeof(uint32_t), 1024*1024, fp);

		//	//fclose(fp);
		//}

		//// bitmapデータの読み込み
		//ReadBitMap readBitMap;
		//// データを取得
		//uint32_t* source = readBitMap.ReadFromBitMap("../resource/image/earth.bmp");
		//if (source == nullptr)
		//{
		//	OutputDebugString(L"画像データを読み込めませんでした。");
		//	return -1;
		//}
		//// テクスチャーを作成
		//Texture2D* texture =
		//	Texture2D::Create(graphicsDevice, readBitMap.width, readBitMap.height, DXGI_FORMAT_R8G8B8A8_UNORM, false);
		//if (texture == nullptr) {
		//	OutputDebugString(L"テクスチャーを作成できませんでした。");
		//	return -1;
		//}
		//texture->width = readBitMap.width;
		//// ピクセルデータを更新
		//texture->SetData(source);
		//// データを開放
		//readBitMap.Release();

		//float time = 0;

		//// メッセージループを実行
		//MSG msg = {};
		//while (true) {
		//	time += 0.00666f;

		//	// 定数バッファーへ転送するデータソースを準備
		//	XMMATRIX worldMatrix = XMMatrixIdentity();
		//	worldMatrix *= XMMatrixScaling(1.0f, 1.0f, 1.0f);
		//	XMVECTOR axis = XMVectorSet(1, 1, 0, 0);
		//	worldMatrix *= XMMatrixRotationAxis(axis, time);
		//	worldMatrix *= XMMatrixTranslation(0.0f, 0.0f, 0.0f);
		//	// ビュー行列を計算
		//	XMVECTOR viewPosition = XMVectorSet(0, 1, -10, 1);	// カメラの位置
		//	XMVECTOR focusPosition = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);	// 焦点の位置
		//	XMVECTOR upDirection = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);		// カメラの上方向 (通常は <0.0f, 1.0f, 0.0f>)
		//	XMMATRIX viewMatrix =
		//		// 左手座標系のビュー行列を作成
		//		XMMatrixLookAtLH(viewPosition, focusPosition, upDirection);
		//	XMMATRIX worldView = worldMatrix * viewMatrix;
		//	// プロジェクション行列を計算
		//	float fovAngleY = 60.0f;			// 視野角
		//	float aspectRatio = 640 / 480.0f;	// アスペクト比
		//	float nearZ = 0.1f;					// クリッピング距離(これより近いのは描画しない）
		//	float farZ = 100.0f;				// クリッピング距離(これより遠いのは描画しない)

		//	// プロジェクション変換行列を作成
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

		//	// 定数バッファーを更新
		//	constantBuffer->SetData(&constantMatricesBuffer);

		//	// 各シェーダーに定数バッファーを設定
		//	ID3D11Buffer* constantBuffers[1] = { constantBuffer->GetNativePointer() };
		//	immediateContext->VSSetConstantBuffers(
		//		0,
		//		1,					// セットするバッファーの数
		//		constantBuffers		// 定数バッファーの配列
		//	);
		//	immediateContext->GSSetConstantBuffers(0, 1, constantBuffers);
		//	immediateContext->PSSetConstantBuffers(0, 1, constantBuffers);

		//	// シェーダ―リソースビューを作成
		//	ID3D11ShaderResourceView* textureViews[1] = { texture->GetShaderResourceView() };
		//	// シェーダーリソースの配列をピクセルシェーダーステージにバインド
		//	immediateContext->PSSetShaderResources(0, 1, textureViews);
		//	// サンプラーステートを作成
		//	ID3D11SamplerState* samplerStates[1] = { texture->GetSamplerState() };
		//	// ピクセルシェーダーパイプラインステージにサンプラーステートの配列を渡します
		//	immediateContext->PSSetSamplers(0, 1, samplerStates);

		//	// 頂点バッファ―と頂点シェーダ―の組合せに対応した入力レイアウトを設定
		//	immediateContext->IASetInputLayout(
		//		inputLayout->GetNativePointer()		// 入力レイアウトに記述するオブジェクトのポインター
		//	);

		//	// プリミティブトポロジーとしてトライアングルを設定
		//	immediateContext->IASetPrimitiveTopology(
		//		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST		// 頂点データを三角形のリストとして解釈する
		//	);

		//	// インデックスバッファーを設定
		//	immediateContext->IASetIndexBuffer(
		//		indexBuffer->GetNativePointer(),
		//		DXGI_FORMAT_R32_UINT,
		//		0
		//	);

		//	// 描画
		//	immediateContext->DrawIndexed(
		//		indexCount,		// 描画する頂点の数
		//		0,		// 最初の頂点のインデックス
		//		0
		//	);

		//	// バックバッファに描画したイメージをディスプレイに表示
		//	HRESULT hr = S_OK;
		//	hr = swapChain->Present(
		//		1,	// フレームの表示を垂直ブランクと同期する方法を指定する整数
		//		0	// スワップチェーン表示オプションを含む整数値
		//	);
		//	if (FAILED(hr))
		//	{
		//		MessageBox(hWnd,
		//			L"グラフィックデバイスが物理的に取り外されたか、ドライバーがアップデートされました。",
		//			L"エラー", MB_OK);
		//		return -1;
		//	}

		//	// このウィンドウのメッセージが存在するかを確認
		//	if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
		//		// メッセージを取得
		//		if (!GetMessage(&msg, NULL, 0, 0)) {
		//			break;
		//		}
		//		TranslateMessage(&msg);
		//		DispatchMessage(&msg);
		//	}
		//}


		//// 開放処理
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

// メッセージループを実行します。
int Game::OnRun()
{
	// 未初期化の場合のみ既定値で初期化する
	if (!isInitialized) {
		Initialize(L"タイトル", 640, 480);
	}

	auto graphicsDevice = graphics->GetGraphicsDevice();

	// ビューポート
	viewport.Width = static_cast<FLOAT>(window->GetWidth());
	viewport.Height = static_cast<FLOAT>(window->GetHeight());
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	// 頂点シェーダー
	VertexShader* vertexShader = nullptr;
	// ジオメトリー シェーダー
	GeometryShader* geometryShader = nullptr;
	// ピクセル シェーダー
	PixelShader* pixelShader = nullptr;
	// シェーダーを作成
	vertexShader = VertexShader::Create(graphicsDevice, BasicVertexShader);
	geometryShader = GeometryShader::Create(graphicsDevice, BasicGeometryShader);
	pixelShader = PixelShader::Create(graphicsDevice, BasicPixelShader);

	// 位置座標のみをもつ頂点データの型
	struct VertexPosition
	{
		XMFLOAT3 position;	// 位置座標
	};
	// 頂点データ
	const VertexPosition vertices[] = {
		{ { -1.0f, 0.0f, 0.0f }, },	// 頂点0
		{ { 0.0f, 1.0f, 0.0f }, },	// 頂点1
		{ { 1.0f, 0.0f, 0.0f }, },	// 頂点2
	};
	// 頂点データについて入力要素を定義
	const D3D11_INPUT_ELEMENT_DESC inputElementDescs[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	// 頂点バッファーを作成
	VertexBuffer* vertexBuffer = nullptr;
	vertexBuffer = VertexBuffer::Create(graphicsDevice, vertices);

	// 入力レイアウトを作成
	InputLayout* inputLayout = nullptr;
	inputLayout = InputLayout::Create(
		graphicsDevice, inputElementDescs, vertexShader->GetShaderBytecode());


	// カメラ パラメーターのための定数バッファーの定義
	struct ConstantBufferDescForCamera
	{
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};
	// 定数バッファーを作成
	ID3D11Buffer* constantBufferForCamera = nullptr;
	{
		D3D11_BUFFER_DESC constantBufferDesc = {};
		constantBufferDesc.ByteWidth = sizeof(ConstantBufferDescForCamera);
		constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constantBufferDesc.CPUAccessFlags = 0;
		constantBufferDesc.MiscFlags = 0;
		constantBufferDesc.StructureByteStride = 0;
		ThrowIfFailed(graphicsDevice->GetDevice()->CreateBuffer(
			&constantBufferDesc, NULL, &constantBufferForCamera));
	}

	// フレームごとに更新される定数バッファーの定義
	struct ConstantBufferDescForPerFrame
	{
		DirectX::XMMATRIX world;
	};
	// 定数バッファーを作成
	ID3D11Buffer* constantBufferForPerFrame = nullptr;
	{
		D3D11_BUFFER_DESC constantBufferDesc = {};
		constantBufferDesc.ByteWidth = sizeof(ConstantBufferDescForPerFrame);
		constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constantBufferDesc.CPUAccessFlags = 0;
		constantBufferDesc.MiscFlags = 0;
		constantBufferDesc.StructureByteStride = 0;
		ThrowIfFailed(graphicsDevice->GetDevice()->CreateBuffer(
			&constantBufferDesc, NULL, &constantBufferForPerFrame));
	}

	// メッセージループを実行
	MSG msg = {};
	while (true) {
		auto immediateContext = graphicsDevice->GetImmediateContext();
		// 画面をクリアー
		immediateContext->ClearRenderTargetView(
			graphics->GetRenderTarget()->GetView(), GetInstance().clearColor);
		immediateContext->ClearDepthStencilView(
			graphics->GetDepthStencil()->GetView(),	// 対象の深度ステンシルビュー
			D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, // クリアするデータのタイプ
			1.0f,	// この値で深度バッファーを初期化	
			0	// この値でステンシルバッファーを初期化
		);
		// レンダーターゲットを設定
		ID3D11RenderTargetView* renderTargetViews[] = {
			graphics->GetRenderTarget()->GetView(),
		};
		immediateContext->OMSetRenderTargets(
			ARRAYSIZE(renderTargetViews),	// バインドするレンダーターゲットの数
			renderTargetViews,		// デバイスにバインドするレンダーターゲット
			graphics->GetDepthStencil()->GetView()	// デバイスにバインドする深度ステンシルビュー
		);
		// ビューポートを設定
		D3D11_VIEWPORT viewports[] = {
			viewport,
		};
		immediateContext->RSSetViewports(
			ARRAYSIZE(viewports), // バインドするビューポートの数
			viewports	// バインドするビューポート
		);

		// シェーダーを設定
		immediateContext->VSSetShader(
			vertexShader->GetNativePointer(), // 頂点シェーダ―へのポインター
			NULL,	// クラスインスタンスインターフェイスの配列へのポインター
			0	// クラスインスタンスインターフェイスの配列の要素数
		);
		immediateContext->GSSetShader(geometryShader->GetNativePointer(), NULL, 0);
		immediateContext->PSSetShader(pixelShader->GetNativePointer(), NULL, 0);
		// 入力アセンブラーに頂点バッファーを設定
		UINT stride = sizeof(VertexPosition);	// 頂点バッファの1頂点分の要素のサイズ
		UINT offset = 0;	// 頂点バッファの最初の要素と使用される最初の頂点バッファの間のバイト数
		ID3D11Buffer* vertexBuffers[1];
		vertexBuffers[0] = vertexBuffer->GetNativePointer();
		// 頂点バッファの配列をInput-Assembler(IA) stageへバインド(D3D11のパイプライン参照)
		immediateContext->IASetVertexBuffers(
			0,	// バインディングする最初の入力スロット
			ARRAYSIZE(vertexBuffers),	// 配列内の頂点バッファの数
			vertexBuffers,	// 頂点バッファの配列へのポインタ
			&stride,	// ストライド値への配列のポインタ
			&offset		// オフセット値への配列のポインタ
		);
		// 入力レイアウトを設定
		immediateContext->IASetInputLayout(inputLayout->GetNativePointer());
		// プリミティブの種類を設定
		immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// トランスフォームを更新
		XMVECTOR scale = {};
		XMVECTOR rotation = {};
		XMVECTOR position = {};
		scale = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
		rotation = XMQuaternionIdentity();
		position = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

		// ワールド変換行列を更新
		XMMATRIX worldMatrix = {};
		worldMatrix = XMMatrixTransformation(
			XMVectorZero(),
			XMQuaternionIdentity(),
			scale,
			XMVectorZero(),
			rotation,
			position);

		// カメラパラメーターを更新
		XMVECTOR eyePosition = {};
		XMVECTOR focusPosition = {};
		XMVECTOR upDirection = {};
		eyePosition = XMVectorSet(0.0f, 1.0f, -10.0f, 1.0f);
		focusPosition = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
		upDirection = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		// ビュー変換行列を更新
		XMMATRIX viewMatrix = {};
		viewMatrix = XMMatrixLookAtLH(eyePosition, focusPosition, upDirection);

		// プロジェクション パラメーターを更新
		float fov = XM_PIDIV4;
		float aspectHeightByWidth = static_cast<float>(window->GetWidth()) / window->GetHeight();
		float nearZ = 0.1f;
		float farZ = 1000.0f;
		// プロジェクション変換行列を更新
		XMMATRIX projectionMatrix = {};
		projectionMatrix = XMMatrixPerspectiveFovLH(fov, aspectHeightByWidth, nearZ, farZ);
		{
			// 定数バッファーを設定
			ConstantBufferDescForCamera constantBufferDesc = {};
			constantBufferDesc.view = viewMatrix;
			constantBufferDesc.projection = projectionMatrix;
			// サブリソースを更新
			immediateContext->UpdateSubresource(
				constantBufferForCamera, 0, NULL, &constantBufferDesc, 0, 0);
			ID3D11Buffer* constantBuffers[] = {
				constantBufferForCamera,
			};
			immediateContext->VSSetConstantBuffers(
				0, ARRAYSIZE(constantBuffers), constantBuffers);
		}
		{
			// 定数バッファーを設定
			ConstantBufferDescForPerFrame constantBufferDesc = {};
			constantBufferDesc.world = worldMatrix;
			// サブリソースを更新
			immediateContext->UpdateSubresource(
				constantBufferForPerFrame, 0, NULL, &constantBufferDesc, 0, 0);
			ID3D11Buffer* constantBuffers[] = {
				constantBufferForPerFrame,
			};
			immediateContext->VSSetConstantBuffers(
				1, ARRAYSIZE(constantBuffers), constantBuffers);
		}

		// 描画
		immediateContext->Draw(3, 0);


		// バックバッファーに描画したイメージをディスプレイに表示
		HRESULT hr = S_OK;
		graphics->GetSwapChain()->Present(1, 0);

		// このウィンドウのメッセージが存在するかを確認
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
			// メッセージを取得
			if (!GetMessage(&msg, NULL, 0, 0)) {
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	SafeRelease(constantBufferForPerFrame);
	SafeRelease(constantBufferForCamera);
	SafeDelete(inputLayout);
	SafeDelete(vertexBuffer);
	SafeDelete(vertexShader);
	SafeDelete(geometryShader);
	SafeDelete(pixelShader);

	SafeDelete(graphics);
	SafeDelete(window);

	return (int)msg.wParam;
}

// アプリケーションを終了します。
void Game::Quit()
{
	GetInstance().window->Close();

}



