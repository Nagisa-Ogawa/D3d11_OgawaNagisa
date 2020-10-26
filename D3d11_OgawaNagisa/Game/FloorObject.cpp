//=============================================================================
// PlayerObject.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "GameObject.h"

using namespace DirectX;

// ���̃N���X�̐V�����C���X�^���X���쐬���܂��B
FloorObject::FloorObject(
	std::shared_ptr<Input> input) : GameObject(input)
{
	tri1.t1 = { 0.0f,0.0f,1.0f };
	tri1.t2 = { 0.0f,0.0f,1.0f }; 
	tri1.t3 = { 0.0f,0.0f,1.0f };
	tri2.t1 = { 0.0f,0.0f,1.0f }; 
	tri1.t2 = { 0.0f,0.0f,1.0f }; 
	tri1.t3 = { 0.0f,0.0f,1.0f };
}

void FloorObject::Start() {
	GameObject::Start();
}

void FloorObject::Update(float time, float deltaTime) {
	GameObject::Update(time, deltaTime);
}

// �t���[����`�悷��ۂɌĂяo����܂��B
void FloorObject::Draw(ID3D11DeviceContext* deviceContext, float time, float deltaTime) {
	GameObject::Draw(deviceContext, time, deltaTime);
}

