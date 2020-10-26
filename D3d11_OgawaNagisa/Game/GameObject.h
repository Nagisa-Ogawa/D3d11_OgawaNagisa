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
class FloorObject;

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

// 当たり判定用の三角形の構造体
struct TriangleCollider {
	DirectX::XMVECTOR t1;
	DirectX::XMVECTOR t2;
	DirectX::XMVECTOR t3;
};

// プレイヤーのゲームオブジェクトを表します。
class PlayerObject : public GameObject
{
public:

	// 前回のポジション
	DirectX::XMVECTOR beforPosition = { 0.0f,0.0f,0.0f,1.0f };
	// 今回のフレームの合計ベクトル
	DirectX::XMVECTOR Vector = { 0.0f,0.0f,0.0f,0.0f };

	// 床との当たり判定を行う座標
	DirectX::XMVECTOR hitPos = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	// 他のオブジェクトと行う当たり判定用の球の半径
	float colliderRadius = 2.0f;

	// このクラスの新しいインスタンスを作成します。
	PlayerObject(std::shared_ptr<Input> input);
	// シーンを初期化する際に呼び出されます。
	void Start();
	// フレームを更新する際に呼び出されます。
	void Update(float time, float deltaTime,float rad, const std::unique_ptr<FloorObject>& floor);
	// フレームを描画する際に呼び出されます。
	void Draw(ID3D11DeviceContext* deviceContext, float time, float deltaTime);
	// 床との当たり判定をするメソッド
	bool CollisionWithFloor(DirectX::XMVECTOR hitPos, DirectX::XMVECTOR ofterHitPos ,TriangleCollider tri);

private:
};

class FloorObject : public GameObject
{
public:

	TriangleCollider tri1;
	TriangleCollider tri2;

	// このクラスの新しいインスタンスを作成します。
	FloorObject(std::shared_ptr<Input> input);
	// シーンを初期化する際に呼び出されます。
	void Start();
	// フレームを更新する際に呼び出されます。
	void Update(float time, float deltaTime);
	// フレームを描画する際に呼び出されます。
	void Draw(ID3D11DeviceContext* deviceContext, float time, float deltaTime);
private:
};



