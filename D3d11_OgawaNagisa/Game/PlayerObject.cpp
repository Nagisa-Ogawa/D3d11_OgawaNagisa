//=============================================================================
// PlayerObject.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "Game.h"

using namespace DirectX;

// このクラスの新しいインスタンスを作成します。
PlayerObject::PlayerObject(
	std::shared_ptr<Input> input) : GameObject(input)
{

}

void PlayerObject::Start() {
	GameObject::Start();
}

void PlayerObject::Update(float time, float deltaTime) {
	GameObject::Update(time, deltaTime);
	if (input->GetKey(VK_SPACE)) {
		transform->rotation = XMQuaternionIdentity();
	}
	const float moveSpeed = 0.1f;
	if (input->GetKey(VK_RIGHT)) {
		transform->position += XMVectorSet(moveSpeed, 0.0f, 0.0f, 0.0f);
	}
	if (input->GetKey(VK_LEFT)) {
		transform->position += XMVectorSet(-moveSpeed, 0.0f, 0.0f, 0.0f);
	}
	if (input->GetKey(VK_UP)) {
		transform->position += XMVectorSet(0.0f, 0.0f, -moveSpeed, 0.0f);
	}
	if (input->GetKey(VK_DOWN)) {
		transform->position += XMVectorSet(0.0f, 0.0f, moveSpeed, 0.0f);
	}

	if (input->GetMouseButtonDown(0)) {
		wchar_t message[1024];
		swprintf_s(message, L"GetMouseButtonDown(0) : mousePosition=(%f, %f)\n",
			input->GetMousePosition().x, input->GetMousePosition().y);
		OutputDebugString(message);
	}
	else if (input->GetMouseButtonUp(0)) {
		wchar_t message[1024];
		swprintf_s(message, L"GetMouseButtonUp(0) : mousePosition=(%f, %f)\n",
			input->GetMousePosition().x, input->GetMousePosition().y);
		OutputDebugString(message);
	}

}

// フレームを描画する際に呼び出されます。
void PlayerObject::Draw(ID3D11DeviceContext* deviceContext, float time, float deltaTime) {
	GameObject::Draw(deviceContext, time, deltaTime);
}
