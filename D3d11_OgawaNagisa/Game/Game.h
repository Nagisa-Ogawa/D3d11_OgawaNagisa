//=============================================================================
// Game.h
// 
//=============================================================================
#pragma once

#include <memory>
#include <wrl.h>
#include <vector>
#include <Windows.h>
#include <DirectXMath.h>
#include <Xinput.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include "Graphics.h"
#include "Scene.h"
#include "SceneManager.h"
#include "GameObject.h"

// 入力データを処理するクラスを表します。
class Input final
{
public:
	// このクラスの新しいインスタンスを初期化します。
	Input(HINSTANCE hInstance, std::shared_ptr<GameWindow> window);
	// デストラクター
	~Input();

	void Update();

	// ウィンドウ メッセージを処理するプロシージャー
	std::tuple<LRESULT, bool> MessagePostedHandler(WindowMessage message);

	bool GetKeyDown(int virtualKey);
	bool GetKey(int virtualKey);
	bool GetKeyUp(int virtualKey);

	// マウスのクライアント座標を取得します。
	DirectX::XMFLOAT2 GetMousePosition();
	// 
	bool GetMouseButtonDown(int button);
	bool GetMouseButton(int button);
	bool GetMouseButtonUp(int button);

	Microsoft::WRL::ComPtr<IDirectInput8> GetDirectInput();
	Microsoft::WRL::ComPtr<IDirectInputDevice8> GetJoystick(int userIndex);
	Microsoft::WRL::ComPtr<IDirectInputDevice8> GetGamepad(int userIndex);

private:
	HINSTANCE hInstance;
	std::shared_ptr<GameWindow> window;
	GameWindow::MessagePosted::KeyType messagePostedKey;

	// キーボード
	BYTE lastKeyState[256] = {};
	BYTE keyState[256] = {};
	// マウス
	DirectX::XMFLOAT2 mousePosition = { 0.0f,0.0f };
	bool lastMouseState[5] = {};
	bool mouseState[5] = {};
	// XInputデバイス入力
	XINPUT_STATE lastXInputStates[XUSER_MAX_COUNT] = {};
	XINPUT_STATE xInputStates[XUSER_MAX_COUNT] = {};
	Microsoft::WRL::ComPtr<IDirectInput8> directInput;
	std::vector<Microsoft::WRL::ComPtr<IDirectInputDevice8>> joystick;
	std::vector<Microsoft::WRL::ComPtr<IDirectInputDevice8>> gamepad;

	static BOOL CALLBACK DIEnumDevicesCallback(
		LPCDIDEVICEINSTANCE deviceInstance, LPVOID ref);

	// コピーと代入演算を禁止
	Input(const Input&) {}
	Input& operator=(const Input&) { return *this; }
};

// トランスフォーム
class Transform final
{
public:
	DirectX::XMVECTOR scale = { 1.0f, 1.0f, 1.0f, 1.0f };
	DirectX::XMVECTOR rotation = DirectX::XMQuaternionIdentity();
	DirectX::XMVECTOR position = { 0.0f, 0.0f, 0.0f, 1.0f };

	DirectX::XMMATRIX GetWorldMatrix() const;

	// このクラスの新しいインスタンスを初期化します。
	Transform();

private:
	// コピーと代入演算を禁止
	Transform(const Transform&) {}
	Transform& operator=(const Transform&) { return *this; }
};

// カメラを表します。
class Camera
{
public:
	// 画面クリアーに使用するカラー
	DirectX::XMVECTORF32 clearColor = { 53 / 255.0f, 70 / 255.0f, 166 / 255.0f, 1.0f };
	// 視点の位置座標
	DirectX::XMVECTOR eyePosition = { 0.0f, 1.0f, -10.0f, 1.0f };
	// 注視点
	DirectX::XMVECTOR focusPosition = { 0.0f, 1.0f, 0.0f, 1.0f };
	// カメラの UP ベクトル
	DirectX::XMVECTOR upDirection = { 0.0f, 1.0f, 0.0f, 0.0f };
	// プロジェクション パラメーター
	float fov = DirectX::XM_PIDIV4;
	float aspectHeightByWidth = 640.0f / 480;
	float nearZ = 0.1f;
	float farZ = 1000.0f;
	// ビューポート
	D3D11_VIEWPORT viewport = { 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f };

	// ビュー変換行列を取得します。
	DirectX::XMMATRIX GetViewMatrix() const;
	// プロジェクション変換行列を取得します。
	DirectX::XMMATRIX GetProjectionMatrix() const;

	// このクラスの新しいインスタンスを初期化します。
	Camera(std::shared_ptr<GameWindow> window);

	// このオブジェクトを初期化する際に一度呼び出されます。
	void Start();

private:
	std::shared_ptr<GameWindow> window;

	// コピーと代入演算を禁止
	Camera(const Camera&) {}
	Camera& operator=(const Camera&) { return *this; }
};

// アプリケーション初期化についての記述を表します。
struct ApplicationSettings
{
	// ウィンドウ設定
	WindowSettings window;
	// グラフィックス設定
	GraphicsSettings graphics;

	// このクラスの新しいインスタンスを初期化します。
	ApplicationSettings();
};

// アプリケーション全体を表します。
class Game
{
public:
	// メッセージループを実行します。
	static int Run(const ApplicationSettings& settings);
	// アプリケーションを終了します。
	static void Quit();

private:
	// このクラスのシングルトンインスタンスを取得します。
	static Game& GetInstance();

	// すでに初期化済みの場合にtrue、それ以外はfalse
	bool isInitialized = false;
	// メイン ウィンドウ
	std::shared_ptr<GameWindow> window;
	// グラフィックス機能
	std::shared_ptr<Graphics> graphics;
	// ユーザー入力デバイス
	std::shared_ptr<Input> input;

	// 初期化の際に呼び出されます。
	void OnInitialize(const ApplicationSettings& settings);
	// メッセージループを実行する際に呼び出されます。
	int OnRun(const ApplicationSettings& settings);
};
