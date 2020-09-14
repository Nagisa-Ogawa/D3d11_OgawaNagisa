#include "stdafx.h"
#include "Game.h"

// このクラスのシングルトンインスタンスを取得します。
SceneManager& SceneManager::GetInstance()
{
	static SceneManager instance;
	return instance;
}


SceneManager::SceneManager(){
}

// 現在のシーンのUpdate()を呼び出します。
void SceneManager::Update(float time, float elapsedTime) {
	this->nowScene->Update(time, elapsedTime);
}

// 現在のシーンのDraw()を呼び出します。
void SceneManager::Draw(float time, float elapsedTime) {
	this->nowScene->Draw(time, elapsedTime);
}

// 現在のシーンを切り替えます。
void SceneManager::ChangeScene(std::shared_ptr<Scene> nowScene)
{
	if (nowScene == NULL) {
		return;
	}
	// 現在のシーンの終了処理をします。
	if (this->nowScene != NULL) {
		this->nowScene->End();
	}
	// シーンを切り替えます。
	this->nowScene = nowScene;
	// 新しいシーンの初期化処理を呼び出します。
	this->nowScene->Start();
}

// 現在のシーンを取得します。
std::shared_ptr<Scene> SceneManager::GetScene()
{
	if (this->nowScene == NULL) {
		return NULL;
	}
	return this->nowScene;
}