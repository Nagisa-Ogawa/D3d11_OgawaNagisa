//=============================================================================
// SceneManager.h
// 
//=============================================================================
#pragma once
#include <memory>

// 前方宣言
class Scene;
class GameWindow;
class Graphics;
class Input;

class SceneManager
{
private:
	// 現在のシーン
	std::shared_ptr<Scene> nowScene;
public:
	// このクラスのシングルトンインスタンスを取得します。
	static SceneManager& GetInstance();
	SceneManager();
	// 現在のシーンのUpdate()を呼び出します。
	void Update(float time, float elapsedTime);
	// 現在のシーンのDraw()を呼び出します。
	void Draw(float time, float elapsedTime);

	// 現在のシーンを切り替えます。
	void ChangeScene(const std::shared_ptr<Scene> nowScene);
	// 現在のシーンを取得します。
	std::shared_ptr<Scene> GetScene();

};

