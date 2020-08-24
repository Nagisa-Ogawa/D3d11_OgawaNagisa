//=============================================================================
// GameWindow.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "GameWindow.h"
#include "Game.h"
#include "DirectXHelper.h"

using namespace std;
using namespace DX;

/// <summary>
/// このクラスの新しいインスタンスを初期化します。
/// </summary>
WindowSettings::WindowSettings()
{
	cbSize = sizeof(WNDCLASSEXW);
	style = CS_HREDRAW | CS_VREDRAW;
	lpfnWndProc = nullptr;
	cbClsExtra = 0;
	cbWndExtra = 0;
	// 呼び出し側プロセスのモジュールハンドルを取得
	hInstance = GetModuleHandle(NULL);
	if (hInstance == NULL) {
		throw Win32Exception(GetLastError());
	}
	hIcon = NULL;
	hCursor = LoadCursor(hInstance, IDC_ARROW);
	hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	lpszMenuName = NULL;
	lpszClassName = L"GameWindowClass";
	hIconSm = NULL;
}
#include <string>
/// <summary>
/// このクラスの新しいインスタンスを初期化します。
/// </summary>
GameWindow::GameWindow(const WindowSettings& settings)
{
	try {
		// ウィンドウ設定を修正
		WindowSettings wndClass(settings);
		if (wndClass.lpfnWndProc == nullptr) {
			wndClass.lpfnWndProc = WndProc;
		}
		if (wndClass.hInstance == NULL) {
			// 呼び出し側プロセスのモジュールハンドルを取得
			wndClass.hInstance = GetModuleHandle(NULL);
			ThrowIfFailed(wndClass.hInstance != NULL);
		}

		title = wndClass.title;
		width = wndClass.width;
		height = wndClass.height;
		className = wndClass.lpszClassName;
		hInstance = wndClass.hInstance;

		// ウィンドウ クラスを登録する
		ThrowIfFailed(0 != RegisterClassEx(&wndClass));

		// クライアント領域が指定した解像度になるウィンドウサイズを計算
		RECT rect = { 0, 0, width, height };
		ThrowIfFailed(AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, FALSE, 0));

		// ウィンドウを作成する
		hWnd = CreateWindowEx(
			0, wndClass.lpszClassName, title.c_str(), WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT,
			(rect.right - rect.left), (rect.bottom - rect.top),
			NULL, NULL, hInstance, NULL);
		ThrowIfFailed(hWnd != NULL);

		// 作成したウィンドウを表示しているモニターを取得
		hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTOPRIMARY);

		// ユーザーデータとしてインスタンスを登録
		auto previousValue = GetWindowLongPtr(hWnd, GWLP_USERDATA);
		SetLastError(0);
		auto result = SetWindowLongPtr(
			hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
		ThrowIfFailed((result == previousValue) && (previousValue != 0));

		// ウィンドウを表示
		ShowWindow(hWnd, SW_SHOWNORMAL);
		UpdateWindow(hWnd);
	}
	catch (...) {
		DestroyWindow(hWnd);
		UnregisterClass(settings.lpszClassName, hInstance);
		throw;
	}
}

// デストラクター
GameWindow::~GameWindow()
{
	DestroyWindow(hWnd);
	UnregisterClass(className.c_str(), hInstance);
}

// メイン ウィンドウを破棄します。
void GameWindow::Close()
{
	ThrowIfFailed(0 != DestroyWindow(hWnd));
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

// このウィンドウを表示しているモニターのハンドルを取得します。
HMONITOR GameWindow::GetHMonitor() const
{
	return hMonitor;
}

// ウィンドウ メッセージを処理するプロシージャー
LRESULT CALLBACK GameWindow::WndProc(
	HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// ユーザーデータとしてインスタンスを登録
	auto instance = reinterpret_cast<GameWindow*>(
		GetWindowLongPtr(hWnd, GWLP_USERDATA));
	if (instance != nullptr) {
		return instance->OnMessagePosted(
			WindowMessage{ hWnd, uMsg, wParam, lParam });
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

// ウィンドウ メッセージを処理するプロシージャー
LRESULT GameWindow::OnMessagePosted(const WindowMessage& message)
{
	for (auto connection : messagePosted.GetConnections()) {
		auto result = connection.second(message);
		if (get<1>(result)) {
			return 0;
		}
	}

	switch (message.uMsg) {
	case WM_CLOSE:
		if (MessageBox(hWnd, L"ウィンドウを閉じますか？", L"メッセージ", MB_OKCANCEL) == IDOK) {
			DestroyWindow(hWnd);	// ウィンドウを閉じる
		}
		return 0;
	case WM_DESTROY:
		// アプリケーションを終了
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(
			message.hWnd, message.uMsg, message.wParam, message.lParam);
	}
}
