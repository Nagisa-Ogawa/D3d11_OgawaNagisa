//=============================================================================
// GameWindow.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "GameWindow.h"
#include "Game.h"
#include "DirectXHelper.h"

using namespace DX;

namespace
{
	// ウィンドウ クラス名
	LPCWSTR const ClassName = L"GameWindow";

	// ウィンドウ メッセージを処理するプロシージャー
	LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg) {
		case WM_CLOSE:
			if (MessageBox(hWnd, L"ウィンドウを閉じますか？", L"メッセージ", MB_OKCANCEL) == IDOK) {
				DestroyWindow(hWnd);	// ウィンドウを閉じる
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

// メイン ウィンドウを初期化します。
GameWindow* GameWindow::Create(LPCWSTR title, int width, int height)
{
	GameWindow* retVal = nullptr;
	HINSTANCE hInstance = NULL;
	HWND hWnd = NULL;

	try {
		// 呼び出し側プロセスのモジュールハンドルを取得
		hInstance = GetModuleHandle(NULL);
		if (hInstance == NULL) {
			throw win32_exception(GetLastError());
		}
		// ウィンドウ クラスを登録する
		WNDCLASSEX wndClass = {};
		ZeroMemory(&wndClass, sizeof wndClass);
		wndClass.cbSize = sizeof(WNDCLASSEX);
		wndClass.lpfnWndProc = WindowProc;	// ウィンドウ プロシージャーを指定
		wndClass.hInstance = hInstance;
		wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndClass.hbrBackground = (HBRUSH)COLOR_BACKGROUND;
		wndClass.lpszClassName = ClassName;
		if (RegisterClassEx(&wndClass) == 0) {
			throw win32_exception(GetLastError());
		}

		// クライアント領域が指定した解像度になるウィンドウサイズを計算
		RECT rect = { 0, 0, width, height };
		if (!AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, FALSE, 0)) {
			throw win32_exception(GetLastError());
		}

		// ウィンドウを作成する
		hWnd = CreateWindowEx(
			0, ClassName, title, WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT,
			(rect.right - rect.left), (rect.bottom - rect.top),
			NULL, NULL, hInstance, NULL);
		if (hWnd == NULL) {
			throw win32_exception(GetLastError());
		}

		// ウィンドウを表示
		ShowWindow(hWnd, SW_SHOWNORMAL);
		UpdateWindow(hWnd);

		retVal = new GameWindow(title, width, height, hWnd);
		return retVal;
	}
	catch (...) {
		DestroyWindow(hWnd);
		UnregisterClass(ClassName, hInstance);
		throw;
	}
	return nullptr;
}

// メイン ウィンドウを破棄します。
void GameWindow::Close()
{
	auto result = DestroyWindow(hWnd);
	if (result == 0) {
		throw win32_exception(GetLastError());
	}
}

// クライアント領域の幅を取得します。
int GameWindow::GetWidth() const
{
	return width;
}

// クライアント領域の高さを取得します。
int GameWindow::GetHeight() const
{
	return height;
}

// ウィンドウ ハンドルを取得します。
HWND GameWindow::GetHwnd() const
{
	return hWnd;
}

// このクラスのインスタンスを初期化します。
GameWindow::GameWindow(LPCWSTR title, int width, int height, HWND hWnd)
{
	this->title = title;
	this->width = width;
	this->height = height;
	this->hWnd = hWnd;
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
