//=============================================================================
// Game.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "Game.h"
#include <chrono>
#include "DirectXHelper.h"

using namespace std;
using namespace std::chrono;
using namespace DX;

// このクラスの新しいインスタンスを初期化します。
ApplicationSettings::ApplicationSettings()
{

}

// このクラスのシングルトンインスタンスを取得します。
Game& Game::GetInstance()
{
	static Game instance;
	return instance;
}

// メッセージループを実行します。
int Game::Run(const ApplicationSettings& settings)
{
	{
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

	}
	return GetInstance().OnRun(settings);

}

// メッセージループを実行します。
int Game::OnRun(const ApplicationSettings& settings)
{
	// 未初期化の場合のみ既定値で初期化する
	if (!isInitialized) {
		OnInitialize(settings);
	}

	shared_ptr<Scene> scene(new Scene(window, graphics, input));
	scene->Start();

	long long totalCount = 0;	// 累積時間（ミリ秒）
	long long deltaCount = 0;	// 経過時間（ミリ秒）

	// メッセージループを実行
	MSG msg = {};
	while (true) {
		// フレーム開始時のカウントを取得
		auto begin = system_clock::now();
		float time = totalCount / 1000.0f;
		float deltaTime = deltaCount / 1000.0f;

		input->Update();

		// このウィンドウのメッセージが存在するかを確認
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
			// メッセージを取得
			if (!GetMessage(&msg, NULL, 0, 0)) {
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// シーンのフレーム処理
		scene->Update(time, deltaTime);
		scene->Draw(time, deltaTime);

		// バックバッファーに描画したイメージをディスプレイに表示
		graphics->GetSwapChain()->Present(1, 0);

		// フレーム終了時のカウントを取得
		auto end = system_clock::now();
		// 経過時間と累積時間を更新
		auto duration = end - begin;
		deltaCount = duration_cast<milliseconds>(duration).count();
		totalCount += deltaCount;
	}

	return (int)msg.wParam;

}

// 初期化の際に呼び出されます。
void Game::OnInitialize(const ApplicationSettings& settings)
{
	// ウィンドウを作成
	window.reset(new GameWindow(settings.window));
	// グラフィックデバイスを作成
	graphics.reset(new Graphics(settings.graphics, window));
	// ユーザー入力デバイスを作成
	input.reset(new Input(settings.window.hInstance, window));

	isInitialized = true;
}

// アプリケーションを終了します。
void Game::Quit()
{
	GetInstance().window->Close();

}



