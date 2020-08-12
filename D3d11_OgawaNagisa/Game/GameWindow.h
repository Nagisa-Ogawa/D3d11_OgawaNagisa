//=============================================================================
// GameWindow.h
// 
//=============================================================================
#pragma once

#include <memory>
#include <Windows.h>

// メイン ウィンドウを表します。
class GameWindow final
{
public:
	// メイン ウィンドウを初期化します。
	static std::shared_ptr<GameWindow> Create(
		LPCWSTR title, int width, int height);

	// メイン ウィンドウを破棄します。
	void Close();
	// クライアント領域の幅を取得します。
	int GetWidth() const;
	// クライアント領域の高さを取得します。
	int GetHeight() const;
	// ウィンドウ ハンドルを取得します。
	HWND GetHwnd() const;

	// デストラクター
	~GameWindow();

private:
	// ウィンドウのタイトル
	LPCWSTR title = L"タイトル";
	// ウィンドウの幅
	int width = 640;
	// ウィンドウの高さ
	int height = 480;
	// ウィンドウのハンドル
	HWND hWnd = NULL;

	// このクラスのインスタンスを初期化します。
	GameWindow(LPCWSTR title, int width, int height, HWND hWnd);
	// コピー コンストラクター
	GameWindow(const GameWindow&) {}
	GameWindow& operator=(const GameWindow&) { return *this; }
};