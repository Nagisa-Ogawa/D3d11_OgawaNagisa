//=============================================================================
// Scene.h
// 
//=============================================================================
#pragma once

#include <memory>
#include <wrl.h>
#include <DirectXMath.h>

// 前方宣言
class Camera;
class GameWindow;
class Graphics;
class GameObject;
class PlayerObject;
class Input;

// ゲーム画面を表します。
class Scene
{
public:
	// このクラスの新しいインスタンスを初期化します。
	// このクラスの新しいインスタンスを初期化します。
	Scene::Scene(
		std::shared_ptr<GameWindow> window,
		std::shared_ptr<Graphics> graphics,
		std::shared_ptr<Input> input)
	{
		this->window = window;
		this->graphics = graphics;
		this->input = input;
	}


	// シーンを初期化する際に呼び出されます。
	virtual void Start() = 0;
	// フレームを更新する際に呼び出されます。
	virtual void Update(float time, float elapsedTime) = 0;
	// フレームを描画する際に呼び出されます。
	virtual void Draw(float time, float elapsedTime) = 0;
	// シーンを削除する際に呼び出されます。
	virtual void End() = 0;
	int a;

protected:
	// このシーンと関連付けられたウィンドウ
	std::shared_ptr<GameWindow> window;
	// このシーンと関連付けられたグラフィックス機能
	std::shared_ptr<Graphics> graphics;
	// ユーザー入力デバイス
	std::shared_ptr<Input> input;
	// カメラ パラメーターのための定数バッファーの定義
	struct ConstantBufferDescForCamera
	{
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};
	// カメラ用の定数バッファー
	std::shared_ptr<ConstantBuffer> constantBufferForCamera;

	// ライティングのための定数バッファーの定義
	struct ConstantBufferDescForLighting
	{
		DirectX::XMFLOAT3A lightPosition;
		DirectX::XMFLOAT4 diffuseLightColor;
		DirectX::XMFLOAT4 specularLightColor;
		DirectX::XMFLOAT4 ambientLightColor;

		DirectX::XMFLOAT3 viewPosition;
		//マテリアル プロパティ
		float specularPower;
		float specularIntensity;
	};
	// ライティング用の定数バッファー
	std::shared_ptr<ConstantBuffer> constantBufferForLighting;

#pragma region CustomPixelShader用

	struct LightingDesc
	{
		// 平行光源の情報
		DirectX::XMFLOAT4 direction;
		DirectX::XMFLOAT4 color;
	};

	struct MaterialDesc
	{
		// 物体表面の質感
		DirectX::XMFLOAT3 baseColor;
		float metallic;
		DirectX::XMFLOAT3 specular;
		float roughness;
	};

	struct ConstantBufferDescForCustomLighting
	{
		LightingDesc lightDesc;
		MaterialDesc materialDesc;
		DirectX::XMFLOAT4 cameraPosition;
	};

	// ライティング用の定数バッファー
	std::shared_ptr<ConstantBuffer> constantBufferForCustomLighting;

#pragma endregion


	// フレームごとに更新される定数バッファーの定義
	struct ConstantBufferDescForPerFrame
	{
		DirectX::XMMATRIX world;
	};
	// フレームごとに更新されるの定数バッファー
	std::shared_ptr<ConstantBuffer> constantBufferForPerFrame;

	// カメラ オブジェクト
	std::shared_ptr<Camera> camera;

	// コピーと代入演算を禁止
	Scene(const Scene&) {}
	Scene& operator=(const Scene&) { return *this; }

private:

};

class TitleScene : public Scene
{
public:
	TitleScene(
		std::shared_ptr<GameWindow> window,
		std::shared_ptr<Graphics> graphics,
		std::shared_ptr<Input> input);
	void Start();
	void Update(float time, float elapsedTime);
	void Draw(float time, float elapsedTime);
	void End();

private:
	// ゲームオブジェクト
	std::shared_ptr<GameObject> titleObj;

};

class GameScene : public Scene
{
public:
	GameScene(
		std::shared_ptr<GameWindow> window,
		std::shared_ptr<Graphics> graphics,
		std::shared_ptr<Input> input);
	void Start();
	void Update(float time, float elapsedTime);
	void Draw(float time, float elapsedTime);
	void End();

private:
	// ゲームオブジェクト
	std::shared_ptr<PlayerObject> playerObject;
	std::shared_ptr<GameObject> gameObjectB;

};



