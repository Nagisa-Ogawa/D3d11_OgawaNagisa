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
	const float rotationSpeed = 2.0f;
	if (input->GetKey('A')) {
		transform->rotation = XMQuaternionMultiply(
			transform->rotation,
			XMQuaternionRotationRollPitchYaw(0.0f, rotationSpeed * deltaTime, 0.0f));
	}
	else if (input->GetKey('D')) {
		transform->rotation = XMQuaternionMultiply(
			transform->rotation,
			XMQuaternionRotationRollPitchYaw(0.0f, -rotationSpeed * deltaTime, 0.0f));
	}
	if (input->GetKey('W')) {
		transform->rotation = XMQuaternionMultiply(
			transform->rotation,
			XMQuaternionRotationRollPitchYaw(rotationSpeed * deltaTime, 0.0f, 0.0f));
	}
	else if (input->GetKey('S')) {
		transform->rotation = XMQuaternionMultiply(
			transform->rotation,
			XMQuaternionRotationRollPitchYaw(-rotationSpeed * deltaTime, 0.0f, 0.0f));
	}
	const float moveSpeed = 0.1f;
	if (input->GetKey(VK_UP)) {
		transform->position += XMVectorSet(0.0f, 0.0f, moveSpeed, 0.0f);
	}
	if (input->GetKey(VK_DOWN)) {
		transform->position += XMVectorSet(0.0f, 0.0f, -moveSpeed, 0.0f);
	}

	// トランスフォームを更新
	transform->scale = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	// transform->position = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
}

// フレームを描画する際に呼び出されます。
void GameObject::Draw(
	ID3D11DeviceContext* deviceContext, float time, float deltaTime)
{
	renderer->Draw(deviceContext);
}
