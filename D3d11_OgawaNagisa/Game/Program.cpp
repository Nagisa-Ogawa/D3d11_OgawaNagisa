//=============================================================================
// Program.cpp
// エントリーポイント
//=============================================================================
#include "stdafx.h"
#include "Game.h"

// アプリケーションのエントリーポイント
int WINAPI wWinMain(
	HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPWSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

#if defined(DEBUG) || defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	// アプリケーションの起動設定
	ApplicationSettings settings;
	settings.window.hInstance = hInstance;
	settings.window.nCmdShow = nCmdShow;
	settings.window.title = L"タイトル";
	settings.window.width = 640;
	settings.window.height = 480;
#if defined(DEBUG) || defined(_DEBUG)
	// DEBUGビルドの際にDirect3Dのデバッグ表示機能を持たせる
	settings.graphics.creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	// メッセージ ループを実行
	return Game::Run(settings);
}
