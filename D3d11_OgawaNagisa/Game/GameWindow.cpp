//=============================================================================
// GameWindow.cpp
// 
//=============================================================================
#include <functional>
#include "GameWindow.h"
#include "Game.h"
#include "DirectXHelper.h"

using namespace DX;

namespace
{
	// ウィンドウ　クラス名
	LPCWSTR const ClassName = L"GameWindow";

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
}

// メイン ウィンドウを取得します。
GameWindow& GameWindow::GetInstance()
{
	static GameWindow instance;
	return instance;
}

// メイン ウィンドウを初期化します。
void GameWindow::Initialize(LPCWSTR title, int width, int height)
{
	// 最初のインスタンス生成の場合を判定
	if (GetInstance().hWnd != NULL) {
		return;
	}

	HINSTANCE hInstance = NULL;
	// ウィンドウオブジェクトへの参照
	HWND hWnd = NULL;

	try {
		// 呼び出し側プロセスのモジュールハンドルを取得
		hInstance = GetModuleHandle(NULL);
		if (hInstance == NULL) {
			// 最後のエラーをthrow
			throw win32_exception(GetLastError());
		}
		// ウィンドウ クラスを登録する
		WNDCLASSEX wndClass = {};
		// メモリのブロックを0で埋める
		ZeroMemory(&wndClass, sizeof wndClass);
		wndClass.cbSize = sizeof(WNDCLASSEX);	// この構造体のサイズ
		wndClass.lpfnWndProc = WindowProc;	// ウィンドウ プロシージャーを指定
		wndClass.hInstance = hInstance;
		wndClass.hCursor = LoadCursor(NULL, IDC_HAND);	// カーソルクラスへのハンドル
		wndClass.hbrBackground = (HBRUSH)COLOR_BACKGROUND;	// 背景ブラシへのハンドル
		wndClass.lpszClassName = ClassName;		// ウィンドウクラス名
		if (RegisterClassEx(&wndClass) == 0) {
			throw win32_exception(GetLastError());
		}

		// クライアント領域が指定した解像度になるウィンドウサイズを計算
		RECT rect = { 0, 0, width, height };
		// クライアント領域が指定したサイズになるウィンドウを作成
		if (!AdjustWindowRectEx(
			&rect,	// クライアント領域へのポインター
			WS_OVERLAPPEDWINDOW,	// ウィンドウスタイル
			FALSE,		// ウィンドウにメニューがあるかどうか
			0	// 拡張ウィンドウのスタイル
		)) {
			throw win32_exception(GetLastError());
		}

		// ウィンドウを作成する
		hWnd = CreateWindowEx(
			0,			// 作成するウィンドウの拡張ウィンドウスタイル
			ClassName,	// クラス名
			title,		// ウィンドウの名前
			WS_OVERLAPPEDWINDOW,	// 作成するウィンドウのウィンドウスタイル
			CW_USEDEFAULT,		// ウィンドウの最初の位置(x軸)
			CW_USEDEFAULT,		// ウィンドウの最初の位置(y軸)
			(rect.right - rect.left),	// ウィンドウのデバイス単位での横幅
			(rect.bottom - rect.top),	// ウィンドウのデバイス単位での高さ
			NULL,		// 作成するウィンドウの親やownerへのハンドル
			NULL,		// メニューへのハンドル
			hInstance,
			NULL
		);

		if (hWnd == NULL) {
			throw win32_exception(GetLastError());
		}

		// ウィンドウを表示
		ShowWindow(hWnd, SW_SHOWNORMAL);
		// ウィンドウの更新
		UpdateWindow(hWnd);

		// シングルトン インスタンスを初期化
		GetInstance().title = title;
		GetInstance().width = width;
		GetInstance().height = height;
		GetInstance().hWnd = hWnd;
	}
	catch (...) {
		DestroyWindow(hWnd);
		// ウィンドウクラスの登録を解除し、クラスのメモリを開放
		UnregisterClass(ClassName, hInstance);
		throw;
	}
}

// メイン ウィンドウを破棄します。
void GameWindow::Close()
{
	auto result = DestroyWindow(GetInstance().hWnd);
	if (result == 0) {
		throw win32_exception(GetLastError());
	}
}

// クライアント領域の幅を取得します。
int GameWindow::GetWidth()
{
	return GetInstance().width;
}

// クライアント領域の高さを取得します。
int GameWindow::GetHeight()
{
	return GetInstance().height;
}

// ウィンドウ ハンドルを取得します。
HWND GameWindow::GetHwnd()
{
	return GetInstance().hWnd;
}

// このクラスのインスタンスを初期化します。
GameWindow::GameWindow()
{

}

// デストラクター
GameWindow::~GameWindow()
{
	auto result = UnregisterClass(ClassName, GetModuleHandle(NULL));
	if (result == 0) {
		char message[1024] = {};
		FormatMessageA(
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // デフォルト ユーザー言語
			message, ARRAYSIZE(message),
			NULL);
		OutputDebugStringA(message);
	}
}
