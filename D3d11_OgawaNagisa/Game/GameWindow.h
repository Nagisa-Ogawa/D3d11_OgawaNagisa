//=============================================================================
// GameWindow.h
// 
//=============================================================================
#pragma once

#include <Windows.h>

// メインウィンドウを表します
class GameWindow
{
public:
	// メイン ウィンドウを初期化します。
	static void Initialize(LPCWSTR title, int width, int height);
	// メイン ウィンドウを破棄します。
	static void Close();
	// クライアント領域の幅を取得します。
	static int GetWidth();
	// クライアント領域の高さを取得します。
	static int GetHeight();
	// ウィンドウ ハンドルを取得します。
	static HWND GetHwnd();

	// デストラクター
	~GameWindow();

private:
	// メイン ウィンドウを取得します。
	static GameWindow& GetInstance();
	// ウィンドウのタイトル
	LPCWSTR title = L"タイトル";
	// ウィンドウの幅
	int width = 640;
	// ウィンドウの高さ
	int height = 480;
	// ウィンドウのハンドル
	HWND hWnd = NULL;

	// このクラスのインスタンスを初期化します。
	GameWindow();
	// コピー コンストラクター
	GameWindow(const GameWindow&) {}

};