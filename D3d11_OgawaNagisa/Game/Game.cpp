//=============================================================================
// Game.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "Game.h"
#include <chrono>
#include "DirectXHelper.h"

using namespace std;
using namespace std::chrono;
using namespace DX;

// このクラスの新しいインスタンスを初期化します。
ApplicationSettings::ApplicationSettings()
{

}

// このクラスのシングルトンインスタンスを取得します。
Game& Game::GetInstance()
{
	static Game instance;
	return instance;
}

// メッセージループを実行します。
int Game::Run(const ApplicationSettings& settings)
{
	return GetInstance().OnRun(settings);

}

// メッセージループを実行します。
int Game::OnRun(const ApplicationSettings& settings)
{
	// 未初期化の場合のみ既定値で初期化する
	if (!isInitialized) {
		OnInitialize(settings);
	}

	std::shared_ptr<TitleScene> titleScene(new TitleScene(window, graphics, input));
	SceneManager::GetInstance().ChangeScene(titleScene);

	long long totalCount = 0;	// 累積時間（ミリ秒）
	long long deltaCount = 0;	// 経過時間（ミリ秒）

	// メッセージループを実行
	MSG msg = {};
	while (true) {
		// フレーム開始時のカウントを取得
		auto begin = system_clock::now();
		float time = totalCount / 1000.0f;
		float deltaTime = deltaCount / 1000.0f;

		input->Update();

		// このウィンドウのメッセージが存在するかを確認
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
			// メッセージを取得
			if (!GetMessage(&msg, NULL, 0, 0)) {
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// シーンのフレーム処理
		SceneManager::GetInstance().Update(time, deltaTime);
		SceneManager::GetInstance().Draw(time, deltaTime);

		// バックバッファーに描画したイメージをディスプレイに表示
		graphics->GetSwapChain()->Present(1, 0);

		// フレーム終了時のカウントを取得
		auto end = system_clock::now();
		// 経過時間と累積時間を更新
		auto duration = end - begin;
		deltaCount = duration_cast<milliseconds>(duration).count();
		totalCount += deltaCount;
	}

	return (int)msg.wParam;

}

// 初期化の際に呼び出されます。
void Game::OnInitialize(const ApplicationSettings& settings)
{
	// ウィンドウを作成
	window.reset(new GameWindow(settings.window));
	// グラフィックデバイスを作成
	graphics.reset(new Graphics(settings.graphics, window));
	// ユーザー入力デバイスを作成
	input.reset(new Input(settings.window.hInstance, window));

	isInitialized = true;
}

// アプリケーションを終了します。
void Game::Quit()
{
	GetInstance().window->Close();

}



