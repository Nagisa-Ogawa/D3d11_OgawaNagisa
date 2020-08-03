//=============================================================================
// Game.cpp
// 
//=============================================================================
#include <DirectXMath.h>	// DirectXの算術ライブラリー
#include <DirectXColors.h>	// DirectXのカラーライブラリー
#include "Game.h"

using namespace DirectX;

// 関数のプロトタイプ宣言
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// アプリケーションを初期化します。
void Game::Initialize(LPCWSTR windowTitle, int screenWidth, int screenHeight)
{
	// ウィンドウのタイトル
	WindowTitle = windowTitle;
	// ウィンドウの幅
	ScreenWidth = screenWidth;
	// ウィンドウの高さ
	ScreenHeight = screenHeight;
}

// ウィンドウを作成します。
bool Game::InitWindow()
{
	HINSTANCE hInstance = GetModuleHandle(NULL);
	// ウィンドウ クラスを登録する
	const wchar_t CLASS_NAME[] = L"GameWindow";
	WNDCLASSEX wndClass = {};
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.lpfnWndProc = WindowProc;	// ウィンドウ プロシージャーを指定
	wndClass.hInstance = hInstance;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)COLOR_BACKGROUND;
	wndClass.lpszClassName = CLASS_NAME;
	if (!RegisterClassEx(&wndClass)) {
		return false;
	}

	// クライアント領域が指定した解像度になるウィンドウサイズを計算
	RECT rect = { 0, 0, ScreenWidth, ScreenHeight };
	AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, FALSE, 0);

	// ウィンドウを作成する
	auto hWnd = CreateWindowEx(
		0, CLASS_NAME, WindowTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		(rect.right - rect.left), (rect.bottom - rect.top),
		NULL, NULL, hInstance, NULL);
	if (hWnd == NULL) {
		return false;
	}

	// ウィンドウを表示
	ShowWindow(hWnd, SW_SHOWNORMAL);
	UpdateWindow(hWnd);

	this->hWnd = hWnd;

	return true;
}

// ウィンドウ メッセージを処理するプロシージャー
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_CLOSE:
		// ウィンドウを閉じる
		if (MessageBox(hWnd, L"ウィンドウを閉じますか？", L"メッセージ", MB_OKCANCEL) == IDOK) {
			DestroyWindow(hWnd);
		}
		return 0;

	case WM_DESTROY:
		// アプリケーションを終了
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


// グラフィックデバイスを作成します
bool Game::InitGraphicsDevice()
{
	HRESULT hr = S_OK;	// 関数の実行結果を受け取る変数

// デバイス作成時のオプションフラグ
	UINT creationFlags = 0;
#if defined(_DEBUG)
	// DEBUGビルドの際にDirect3Dのデバッグ表示機能を持たせる
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif


	// 作成するスワップチェーンについての記述
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	{
		// 解像度の幅を表す。0を指定すると出力ウィンドウから幅を取得し
		// スワップチェーンに割り当てる
		swapChainDesc.BufferDesc.Width = ScreenWidth;
		// 解像度の高さを表す。0を指定すると出力ウィンドウから高さを取得し
		// スワップチェーンに割り当てる
		swapChainDesc.BufferDesc.Height = ScreenHeight;
		// リフレッシュレートを表す
		swapChainDesc.BufferDesc.RefreshRate = { 60,1 };
		// データをどのように扱うかの形式
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		// マルチサンプリングのパラメータ
		swapChainDesc.SampleDesc = { 1,0 };
		// バックバッファーの表面使用量とCPUアクセスオプションを設定するメンバー
		// シェーダーリソースとして使用することを設定
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT |
			DXGI_USAGE_SHADER_INPUT;
		// スワップチェーン内のバッファの数。通常はこの数に
		// フロントバッファも含める
		swapChainDesc.BufferCount = 2;
		// 出力ウィンドウへのHWNDハンドル。NULLではいけない
		swapChainDesc.OutputWindow = hWnd;
		// バッファの切り替え方のパラメータ
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		// 出力がウィンドウモードであるかを指定するパラメータ
		swapChainDesc.Windowed = TRUE;
	}

	// デバイス、デバイスコンテキスト、スワップチェーンを作成
	hr = D3D11CreateDeviceAndSwapChain(
		NULL,						// デバイスの作成時に使用するビデオアダプターへのポインター。NULLを指定するとデフォルトアダプター
		D3D_DRIVER_TYPE_HARDWARE,	// 作成するドライバーの種類。基本はHARDWARE
		0,							// ソフトウェアラスタライザーを実装したDLLのハンドル。DriverTypeがSOFTWAREの場合はNULLにすることができない
		creationFlags,				// デバイス作成時のオプションフラグ
		featureLevels,				// 作成する機能レベルの配列へのポインター。NULLを指定して省略できる
		6,							// 作成する機能レベルの配列へのポインター。NULLを指定して省略できる
		D3D11_SDK_VERSION,			// いつでもD3D11_SDK_VERSION
		&swapChainDesc,
		&swapChain,
		&graphicsDevice,			// [出力]Direct3D 11 デバイス
		&featureLevel,				// 実際に作成された機能レベル
		&immediateContext			// [出力]Direct3D 11 デバイス コンテキスト
	);
	if (FAILED(hr)) {
		MessageBox(hWnd, L"Direct3D 11デバイスを作成できませんでした。", L"エラー", MB_OK);
		return false;
	}

	// スワップチェーンからバックバッファを取得
	ID3D11Texture2D* backBuffer = nullptr;
	// スワップチェーンのバックバッファにアクセス
	hr = swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
	if (FAILED(hr))
	{
		MessageBox(hWnd, L"バックバッファを取得できませんでした。", L"エラー", MB_OK);
		return false;
	}

	// バックバッファにアクセスするためのレンダーターゲットビューを作成
	hr = graphicsDevice->CreateRenderTargetView(backBuffer, NULL, &renderTargetViews[0]);
	if (FAILED(hr))
	{
		MessageBox(hWnd, L"レンダーターゲットビューを作成できませんでした。", L"エラー", MB_OK);
		return false;
	}

	// バックバッファにシェーダーからアクセスするためのリソースビューを作成
	hr = graphicsDevice->CreateShaderResourceView(
		// シェーダーへの入力として機能するリソースへのポインター
		// 引数はID3D11Resource型だがID3D11Texture2DはIDID3D11Resourceを
		// 継承しているため引数に使える
		backBuffer,
		// シェーダーリソースビュー説明へのポインター
		// NULLを指定するとリソース全体にアクセスするビューを作成する
		NULL,
		&renderTargetResourceView);
	if (FAILED(hr))
	{
		MessageBox(hWnd, L"レンダーターゲット リソース ビューを作成できませんでした。", L"エラー", MB_OK);
		return false;
	}
	SAFE_RELEASE(backBuffer);


	// テクスチャのフォーマットを設定
	DXGI_FORMAT textureFormat = depthStencilFormat;
	DXGI_FORMAT resourceFormat = depthStencilFormat;
	// 深度ステンシルのフォーマットの形式に
	// テクスチャとシェーダーリソースのフォーマットも合わせる
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

	// 深度ステンシルを作成
	ID3D11Texture2D* depthStencil = nullptr;
	D3D11_TEXTURE2D_DESC depthStencilDesc = {};
	depthStencilDesc.Width = swapChainDesc.BufferDesc.Width;
	depthStencilDesc.Height = swapChainDesc.BufferDesc.Height;
	// テクスチャ内のミップマップレベルの最大数。
	// マルチサンプルテクスチャには1を使用する
	depthStencilDesc.MipLevels = 1;
	// テクスチャ配列内のテクスチャ
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = textureFormat;
	depthStencilDesc.SampleDesc = swapChainDesc.SampleDesc;
	// テクスチャの読み書き方法を識別する値
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	// パイプラインステージにバインドするためのフラグ
	// シェーダーリソースとして使用することを設定
	depthStencilDesc.BindFlags =
		D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	// 許可されるCPUアクセスのタイプを指定するフラグ
	// CPUアクセスが必要ない場合は0を指定
	depthStencilDesc.CPUAccessFlags = 0;
	// リソースオプションを指定するフラグ
	// 使用しない場合は0を指定
	depthStencilDesc.MiscFlags = 0;
	// 深度ステンシルを作成
	hr = graphicsDevice->CreateTexture2D(&depthStencilDesc, NULL, &depthStencil);
	if (FAILED(hr))
	{
		MessageBox(hWnd, L"深度ステンシルを作成できませんでした。", L"エラー", MB_OK);
		return false;
	}

	// 深度ステンシルにアクセスするためのビューを作成
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
	// 深度ステンシルと同じフォーマットにする
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

	// 深度ステンシルビューを作成
	hr = graphicsDevice->CreateDepthStencilView(depthStencil, &depthStencilViewDesc, &depthStencilView);
	if (FAILED(hr)) {
		MessageBox(hWnd, L"深度ステンシル ビューを作成できませんでした。", L"エラー", MB_OK);
		return false;
	}

	// 深度ステンシルにシェーダーからアクセスするためのリソースビューを作成
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
		MessageBox(hWnd, L"深度ステンシル深度ステンシル リソース ビューを作成できませんでした。", L"エラー", MB_OK);
		return false;
	}
	SAFE_RELEASE(depthStencil);

	// ビューポートを設定
	viewports[0].Width = static_cast<FLOAT>(ScreenWidth);
	viewports[0].Height = static_cast<FLOAT>(ScreenHeight);
	viewports[0].MinDepth = 0.0f;
	viewports[0].MaxDepth = 1.0f;
	viewports[0].TopLeftX = 0.0f;
	viewports[0].TopLeftY = 0.0f;

	return true;

}


// グラフィックリソースを開放します
void Game::ReleaseGraphicsDevice()
{
	// リソースを解放
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

// メッセージループを実行します。
int Game::Run()
{
	// ウィンドウを作成
	if (!InitWindow()) {
		MessageBox(NULL, L"ウィンドウの作成に失敗しました。", L"メッセージ", MB_OK);
		return -1;
	}
	// グラフィックデバイスを作成
	if (!InitGraphicsDevice())
	{
		MessageBox(NULL, L"グラフィックデバイスを初期化できませんでした。", L"メッセージ", MB_OK);
		return -1;
	}

	// 結果
	// マイナス以下ならエラー
	HRESULT hr = S_OK;

	// 頂点データの配列
	// 横の分割数
	const int u_max = 64;
	// 縦の分割数
	const int v_max = 64;
	// 半径
	const float radius = 3.0f;
	const int vertex_num = (u_max + 1) * (v_max + 1);
	// UV座標のオフセット
	const float uOffset = 1.0f / u_max;
	const float vOffset = 1.0f / v_max;
	// 現在のUV座標
	float nowUPos = 0.0f;
	float nowVPos = 0.0f;

	VertexPositionNormalTexture vertices[vertex_num] = {};

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


	//// 頂点データの配列
	//VertexPositionNormalTexture vertices[divideCount + divideCount + 2 * (divideCount + 1)] = {};
	//// Top
	//for (size_t triangle = 0; triangle < divideCount; triangle++)
	//{
	//	// 角度
	//	const auto angle = XM_2PI / divideCount * triangle;
	//	// 位置座標
	//	vertices[triangle].position.x = radius * XMScalarCos(angle);
	//	vertices[triangle].position.y = height / 2;
	//	vertices[triangle].position.z = radius * -XMScalarSin(angle);
	//	// 法線ベクトル
	//	vertices[triangle].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	//	// UV座標

	//}
	//// Bottom
	//for (size_t triangle = 0; triangle < divideCount; triangle++)
	//{
	//	const auto angle = XM_2PI / divideCount * triangle;
	//	// vertices配列にはTop面の頂点数分(divideCount)だけずらしてアクセスする
	//	// 位置座標
	//	vertices[triangle + divideCount].position.x = radius * XMScalarCos(angle);
	//	vertices[triangle + divideCount].position.y = -height / 2;
	//	vertices[triangle + divideCount].position.z = radius * XMScalarSin(angle);
	//	// 法線ベクトル
	//	vertices[triangle + divideCount].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
	//	// UV座標
	//}
	//// Side
	//for (size_t triangle = 0; triangle <= divideCount; triangle++)
	//{
	//	const auto angle = XM_2PI / divideCount * triangle;
	//	// vertices配列にはTop面とBottom面の頂点数の分(2 * divideCount)だけずらしてアクセスする
	//	auto& top = vertices[2 * triangle + 0 + 2 * divideCount];
	//	auto& bottom = vertices[2 * triangle + 1 + 2 * divideCount];

	//	// Top面に隣接する頂点
	//	// 法線ベクトル
	//	top.normal = XMFLOAT3(XMScalarCos(angle), 0.0f, XMScalarSin(angle));
	//	// 位置座標
	//	top.position.x = radius * top.normal.x;
	//	top.position.y = height / 2;
	//	top.position.z = radius * top.normal.z;

	//	// Bottom面に隣接する頂点
	//	// 法線ベクトル
	//	bottom.normal = top.normal;
	//	// 位置座標
	//	bottom.position = top.position;
	//	bottom.position.y = -height / 2;
	//	// UV座標
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

		// 頂点データの配列
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


	// 頂点バッファーを作成
	VertexBuffer* vertexBuffer = nullptr;
	vertexBuffer = VertexBuffer::Create(graphicsDevice, sizeof vertices);
	if (vertexBuffer == nullptr)
	{
		OutputDebugString(L"頂点バッファーを作成できませんでした。");
		return -1;
	}
	// リソースを更新
	vertexBuffer->SetData(vertices);

	// インデックスデータの要素数
	const int index_num = (v_max * u_max) * 6;
	//インデックスデータの配列
    UINT32 indices[index_num] = {};

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

	////インデックスデータの配列
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

		// インデックスデータの配列
	//UINT32 indices[] = {
	//	 0,  1,  2,  3,  2,  1,	// Front
	//	// 4,  5,  6,  7,  6,  5,	// Back
	//	// 8,  9, 10, 11, 10,  9,	// Right
	//	//12, 13, 14, 15, 14, 13,	// Left
	//	//16, 17, 18, 19, 18, 17,	// Top
	//	//20, 21, 22, 23, 22, 21,	// Bottom
	//};


	// インデックスの数
	UINT indexCount = ARRAYSIZE(indices);

	// インデックスバッファーを作成
	IndexBuffer* indexBuffer = nullptr;
	indexBuffer = IndexBuffer::Create(graphicsDevice, indexCount);
	if (indexBuffer == nullptr) {
		return -1;
	}
	// リソースを更新
	indexBuffer->SetData(indices);



	// 定数バッファーを介してシェーダーに毎フレーム送る行列データを表しています。
	//struct ConstantLight
	//{
	//	// ライトの位置座標(x,y,z,w 平行光源 w = 0, 点光源 w = 1)
	//	DirectX::XMFLOAT4 lightPosition;
	//	// ライトの拡散反射の色(r,g,b)
	//	DirectX::XMFLOAT4 lightDiffuse;
	//	// ライトの鏡面反射の色(r,g,b)
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
		// 質感
		XMFLOAT4 roughness_metallic = XMFLOAT4(0.5f, 1.0f, 0, 0);
		XMFLOAT4 albedo = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f);
	};

	struct ConstantMatricesBuffer
	{
		XMFLOAT4X4 worldMatrix;
		XMFLOAT4X4 viewMatrix;
		XMFLOAT4X4 projectionMatrix;
		XMFLOAT4X4 worldViewProjectionMatrix;
		// 視点座標
		XMFLOAT4 viewPosition = XMFLOAT4(0, 1, -10, 1);
		// 環境光
		// 光源
		ConstantDirectionalLight constantDirectionalLight;
		ConstantPointLight constantPointLight;
		// 物体の質感
		ConstantMaterial material;
	};

	// 定数バッファーを作成
	ConstantBuffer* constantBuffer = nullptr;
	constantBuffer = ConstantBuffer::Create(graphicsDevice, sizeof(ConstantMatricesBuffer));
	if (constantBuffer == nullptr)
	{
		OutputDebugString(L"定数バッファーを作成できませんでした。");
		return -1;
	}

	// シェーダ―を作成
	BasicVertexShader* vertexShader = nullptr;
	vertexShader = BasicVertexShader::Create(graphicsDevice);
	if (vertexShader == nullptr)
	{
		OutputDebugString(L"頂点シェーダーを作成できませんでした。");
		return -1;
	}
	BasicGeometryShader* geometryShader = nullptr;
	geometryShader = BasicGeometryShader::Create(graphicsDevice);
	if (geometryShader == nullptr)
	{
		OutputDebugString(L"ジオメトリーシェーダーを作成できませんでした。");
		return -1;
	}
	BasicPixelShader* pixelShader = nullptr;
	pixelShader = BasicPixelShader::Create(graphicsDevice);
	if (pixelShader == nullptr)
	{
		OutputDebugString(L"ピクセルシェーダーを作成できませんでした。");
		return -1;
	}

	// 入力レイアウトを作成
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

	// 画像のダミー
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
		//	printf("失敗");
		//	return -1;
		//}

		//fread(source, sizeof(uint32_t), 1024*1024, fp);

		//fclose(fp);
	}

	// bitmapデータの読み込み
	ReadBitMap readBitMap;
	// データを取得
	uint32_t* source = readBitMap.ReadFromBitMap("../resource/image/gold.bmp");
	if (source == nullptr)
	{
		OutputDebugString(L"画像データを読み込めませんでした。");
		return -1;
	}
	// テクスチャーを作成
	Texture2D* texture =
		Texture2D::Create(graphicsDevice, readBitMap.width, readBitMap.height, DXGI_FORMAT_R8G8B8A8_UNORM, false);
	if (texture == nullptr) {
		OutputDebugString(L"テクスチャーを作成できませんでした。");
		return -1;
	}
	texture->width = readBitMap.width;
	// ピクセルデータを更新
	texture->SetData(source);
	// データを開放
	readBitMap.Release();

	float time = 0;

	// メッセージループを実行
	MSG msg = {};
	while (true) {
		time += 0.00666f;

		// 定数バッファーへ転送するデータソースを準備
		XMMATRIX worldMatrix = XMMatrixIdentity();
		worldMatrix *= XMMatrixScaling(1.0f, 1.0f, 1.0f);
		XMVECTOR axis = XMVectorSet(1, 1, 0, 0);
		worldMatrix *= XMMatrixRotationAxis(axis, time);
		worldMatrix *= XMMatrixTranslation(0.0f, 0.0f, 0.0f);
		// ビュー行列を計算
		XMVECTOR viewPosition = XMVectorSet(0, 1, -10, 1);	// カメラの位置
		XMVECTOR focusPosition = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);	// 焦点の位置
		XMVECTOR upDirection = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);		// カメラの上方向 (通常は <0.0f, 1.0f, 0.0f>)
		XMMATRIX viewMatrix =
			// 左手座標系のビュー行列を作成
			XMMatrixLookAtLH(viewPosition, focusPosition, upDirection);
		XMMATRIX worldView = worldMatrix * viewMatrix;
		// プロジェクション行列を計算
		float fovAngleY = 60.0f;			// 視野角
		float aspectRatio = 640 / 480.0f;	// アスペクト比
		float nearZ = 0.1f;					// クリッピング距離(これより近いのは描画しない）
		float farZ = 100.0f;				// クリッピング距離(これより遠いのは描画しない)

		// プロジェクション変換行列を作成
		XMMATRIX projectonMatrix =
			XMMatrixPerspectiveFovLH(
				XMConvertToRadians(fovAngleY),
				aspectRatio,
				nearZ,
				farZ
			);

		// 光源の位置
		//XMVECTOR lightPosition = XMVectorSet(-2.0f, 2.0f, -1.0f, 0.0f);
		//// 拡散反射成分
		//XMVECTOR lightDiffuse = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);
		//// 鏡面反射成分
		//XMVECTOR lightSpecular = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);
		//// 環境反射成分
		//XMVECTOR lightAmbient = XMVectorSet(0.2f, 0.2f, 0.2f, 0.0f);
		//// 拡散反射率
		//XMVECTOR materialDiffuse = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);
		//// 鏡面反射率と強さ(光沢度係数)
		//XMVECTOR materialSpecular = XMVectorSet(1.0f, 1.0f, 1.0f, 20.0f);
		//// 環境反射率
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

		// 定数バッファーを更新
		constantBuffer->SetData(&constantMatricesBuffer);

		// レンダーターゲットを設定
		immediateContext->OMSetRenderTargets(
			1,					// バインドするレンダーターゲットの数
			renderTargetViews,	// デバイスにバインドするレンダーターゲット
			depthStencilView	// デバイスにバインドする深度ステンシルビュー
		);
		// 画面をクリアー
		immediateContext->ClearRenderTargetView(
			renderTargetViews[0],	// レンダーターゲットのポインター
			clearColor				// レンダーターゲットを塗りつぶす色
		);

		// 深度ステンシルビューをクリア
		immediateContext->ClearDepthStencilView(
			depthStencilView,							// 対象の深度ステンシルビュー
			D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,	// クリアするデータのタイプ
			1.0f,										// この値で深度バッファーを初期化	
			0											// この値でステンシルバッファーを初期化
		);

		// ビューポートを設定
		immediateContext->RSSetViewports(
			1,			// バインドするビューポートの数
			viewports	// バインドするビューポート
		);

		// 頂点バッファーを設定
		ID3D11Buffer* vertexBuffers[1] = { vertexBuffer->GetNativePointer() };
		UINT strides[1] = { sizeof(VertexPositionNormalTexture) };	// 頂点バッファの1頂点分の要素のサイズ
		UINT offsets[1] = { 0 };	// 頂点バッファの最初の要素と使用される最初の頂点バッファの間のバイト数
		// 頂点バッファの配列をInput-Assembler(IA) stageへバインド(D3D11のパイプライン参照)
		immediateContext->IASetVertexBuffers(
			0,	// バインディングする最初の入力スロット
			ARRAYSIZE(vertexBuffers),	// 配列内の頂点バッファの数
			vertexBuffers,	// 頂点バッファの配列へのポインタ
			strides,	// ストライド値への配列のポインタ
			offsets		// オフセット値への配列のポインタ
		);

		// シェーダ―を設定
		immediateContext->VSSetShader(
			vertexShader->GetNativePointer(),	// 頂点シェーダ―へのポインター
			NULL,			// クラスインスタンスインターフェイスの配列へのポインター
			0				// クラスインスタンスインターフェイスの配列の要素数
		);
		immediateContext->GSSetShader(
			geometryShader->GetNativePointer(),
			NULL,
			0
		);
		immediateContext->PSSetShader(
			pixelShader->GetNativePointer(),	// ピクセルシェーダ―へのポインター
			NULL,			// クラスインスタンスインターフェイスの配列へのポインター
			0				// クラスインスタンスインターフェイスの配列の要素数
		);

		// 各シェーダーに定数バッファーを設定
		ID3D11Buffer* constantBuffers[1] = { constantBuffer->GetNativePointer() };
		immediateContext->VSSetConstantBuffers(
			0,
			1,					// セットするバッファーの数
			constantBuffers		// 定数バッファーの配列
		);
		immediateContext->GSSetConstantBuffers(0, 1, constantBuffers);
		immediateContext->PSSetConstantBuffers(0, 1, constantBuffers);

		// シェーダ―リソースビューを作成
		ID3D11ShaderResourceView* textureViews[1] = { texture->GetShaderResourceView() };
		// シェーダーリソースの配列をピクセルシェーダーステージにバインド
		immediateContext->PSSetShaderResources(0, 1, textureViews);
		// サンプラーステートを作成
		ID3D11SamplerState* samplerStates[1] = { texture->GetSamplerState() };
		// ピクセルシェーダーパイプラインステージにサンプラーステートの配列を渡します
		immediateContext->PSSetSamplers(0, 1, samplerStates);

		// 頂点バッファ―と頂点シェーダ―の組合せに対応した入力レイアウトを設定
		immediateContext->IASetInputLayout(
			inputLayout->GetNativePointer()		// 入力レイアウトに記述するオブジェクトのポインター
		);

		// プリミティブトポロジーとしてトライアングルを設定
		immediateContext->IASetPrimitiveTopology(
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST		// 頂点データを三角形のリストとして解釈する
		);

		// インデックスバッファーを設定
		immediateContext->IASetIndexBuffer(
			indexBuffer->GetNativePointer(),
			DXGI_FORMAT_R32_UINT,
			0
		);

		// 描画
		immediateContext->DrawIndexed(
			ARRAYSIZE(indices),		// 描画する頂点の数
			0,		// 最初の頂点のインデックス
			0
		);

		// バックバッファに描画したイメージをディスプレイに表示
		HRESULT hr = S_OK;
		hr = swapChain->Present(
			1,	// フレームの表示を垂直ブランクと同期する方法を指定する整数
			0	// スワップチェーン表示オプションを含む整数値
		);
		if (FAILED(hr))
		{
			MessageBox(hWnd,
				L"グラフィックデバイスが物理的に取り外されたか、ドライバーがアップデートされました。",
				L"エラー", MB_OK);
			return -1;
		}

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


	// 開放処理
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


