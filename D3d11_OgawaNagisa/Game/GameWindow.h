//=============================================================================
// GameWindow.h
// 
//=============================================================================
#pragma once

#include <memory>
#include <functional>
#include <map>
#include <Windows.h>

/// <summary>
/// ハンドラーを登録して管理するクラスを表します。
/// </summary>
template <typename Signature,
	typename SlotFunction = std::function<Signature>
> class Signal
{
public:
	typedef typename int KeyType;
	typedef typename std::map<KeyType, SlotFunction> ContainerType;
	typedef typename Signature SignatureType;
	typedef typename SlotFunction SlotType;

	KeyType key = 1;

	/// <summary>
	/// 登録されているハンドラーを取得します。
	/// </summary>
	ContainerType& GetConnections()
	{
		return connections;
	}
	/// <summary>
	/// イベントを処理するハンドラーを登録します。
	/// </summary>
	KeyType Connect(const SlotType&& slot)
	{
		connections[key] = slot;
		key++;
		return key;
	}
	/// <summary>
	/// 登録されているハンドラーを削除します。
	/// </summary>
	void Disconnect(KeyType key)
	{
		connections.erase(key);
	}
	/// <summary>
	/// 登録されているハンドラーをすべて実行します。
	/// </summary>
	void operator()(typename SlotType::argument_type args) const
	{
		for (auto& connection : connections) {
			connection.second(std::forward<SlotType::argument_type>(args));
		}
	}

private:
	ContainerType connections;
};

/// <summary>
/// <see cref="GameWindow" />を作成するための記述を表します。
/// </summary>
struct WindowSettings : public WNDCLASSEXW
{
	/// <summary>
	/// ウィンドウ タイトルです。
	/// </summary>
	LPCWSTR title = L"Gameタイトル";
	/// <summary>
	/// クライアント領域の幅です。
	/// </summary>
	UINT width = 640;
	/// <summary>
	/// クライアント領域の高さです。
	/// </summary>
	UINT height = 480;
	/// <summary>
	/// 
	/// </summary>
	int nCmdShow = SW_SHOWNORMAL;

	/// <summary>
	/// このクラスの新しいインスタンスを初期化します。
	/// </summary>
	WindowSettings();
};

/// <summary>
/// ウィンドウ メッセージを表します。
/// </summary>
struct WindowMessage
{
	HWND hWnd;
	UINT uMsg;
	WPARAM wParam;
	LPARAM lParam;
};

// メイン ウィンドウを表します。
class GameWindow final
{
public:
	typedef Signal<std::tuple<LRESULT, bool>(WindowMessage)> MessagePosted;

	// クライアント領域の幅を取得します。
	int GetWidth() const;
	// クライアント領域の高さを取得します。
	int GetHeight() const;
	// ウィンドウ ハンドルを取得します。
	HWND GetHwnd() const;
	// このウィンドウを表示しているモニターのハンドルを取得します。
	HMONITOR GetHMonitor() const;

	// このクラスの新しいインスタンスを初期化します。
	GameWindow(const WindowSettings& settings);
	// デストラクター
	~GameWindow();

	// メイン ウィンドウを破棄します。
	void Close();

	// ウィンドウ メッセージを処理するハンドラーを登録します。
	MessagePosted messagePosted;

private:
	// 
	HINSTANCE hInstance = NULL;
	// ウィンドウ クラス名
	std::wstring className;
	// ウィンドウのタイトル
	std::wstring title;
	// ウィンドウの幅
	int width = 640;
	// ウィンドウの高さ
	int height = 480;
	// ウィンドウのハンドル
	HWND hWnd = NULL;
	// このウィンドウを表示しているモニターのハンドル
	HMONITOR hMonitor = NULL;

	// ウィンドウ メッセージを処理するプロシージャー
	static LRESULT CALLBACK WndProc(
		HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	// ウィンドウにメッセージが送られてきた際に呼び出されます。
	LRESULT OnMessagePosted(const WindowMessage& message);

	// コピーと代入演算を禁止
	GameWindow(const GameWindow&) {}
	GameWindow& operator=(const GameWindow&) { return *this; }
};