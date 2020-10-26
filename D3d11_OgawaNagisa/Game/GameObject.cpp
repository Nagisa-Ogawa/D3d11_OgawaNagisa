//=============================================================================
// GameObject.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "Game.h"

using namespace DirectX;

// このクラスの新しいインスタンスを初期化します。
GameObject::GameObject(std::shared_ptr<Input> input)
{
	this->input = input;
	// トランスフォームを作成
	transform.reset(new Transform());
}

// トランスフォームを取得します。
std::shared_ptr<Transform> GameObject::GetTransform()
{
	return transform;
}

// トランスフォームを取得します。
std::shared_ptr<const Transform> GameObject::GetTransform() const
{
	return transform;
}

// シーンを初期化する際に呼び出されます。
void GameObject::Start()
{

}

// フレームを更新する際に呼び出されます。
void GameObject::Update(float time, float deltaTime)
{
}

// フレームを描画する際に呼び出されます。
void GameObject::Draw(
	ID3D11DeviceContext* deviceContext, float time, float deltaTime)
{
	renderer->Draw(deviceContext);
}
