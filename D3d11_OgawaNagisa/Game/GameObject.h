//=============================================================================
// GameObject.h
// 
//=============================================================================
#pragma once
#include <memory>

// 前方宣言
class Transform;
class Mesh;
class MeshRenderer;
class Input;

// ゲームオブジェクトを表します。
class GameObject
{
public:
	// トランスフォームを取得します。
	std::shared_ptr<Transform> GetTransform();
	// トランスフォームを取得します。
	std::shared_ptr<const Transform> GetTransform() const;
	// メッシュ
	std::shared_ptr<Mesh> mesh;
	// レンダリング
	std::shared_ptr<MeshRenderer> renderer;

	// このクラスの新しいインスタンスを初期化します。
	GameObject(std::shared_ptr<Input> input);

	// シーンを初期化する際に呼び出されます。
	virtual void Start();
	// フレームを更新する際に呼び出されます。
	virtual void Update(float time, float deltaTime);
	// フレームを描画する際に呼び出されます。
	virtual void Draw(ID3D11DeviceContext* deviceContext, float time, float deltaTime);

protected:
	// トランスフォーム
	std::shared_ptr<Transform> transform;
	// ユーザー入力デバイス
	std::shared_ptr<Input> input;

	// コピーと代入演算を禁止
	GameObject(const GameObject&) {}
	GameObject& operator=(const GameObject&) { return *this; }
private:
};


// プレイヤーのゲームオブジェクトを表します。
class PlayerObject : public GameObject
{
public:
	// このクラスの新しいインスタンスを作成します。
	PlayerObject(std::shared_ptr<Input> input);
	// シーンを初期化する際に呼び出されます。
	void Start();
	// フレームを更新する際に呼び出されます。
	void Update(float time, float deltaTime);
	// フレームを描画する際に呼び出されます。
	void Draw(ID3D11DeviceContext* deviceContext, float time, float deltaTime);

private:
};

