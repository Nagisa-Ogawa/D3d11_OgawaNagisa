//=============================================================================
// Program.cpp
// エントリーポイント
//=============================================================================
#include "Game.h"

// アプリケーションのエントリーポイント
int WINAPI wWinMain(
	_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
// デバッグ時なら
#if defined(DEBUG) || defined(_DEBUG)
	// アプリケーション終了時にメモリリークレポートを出力する
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	// アプリケーションの起動設定
	Game::Initialize(L"Gameタイトル", 640, 480);
	// メッセージ ループを実行
	return Game::Run();
}
