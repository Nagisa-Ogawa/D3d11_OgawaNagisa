//=============================================================================
// GameObject.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "Game.h"

using namespace DirectX;

// ���̃N���X�̐V�����C���X�^���X�����������܂��B
GameObject::GameObject(std::shared_ptr<Input> input)
{
	this->input = input;
	// �g�����X�t�H�[�����쐬
	transform.reset(new Transform());
}

// �g�����X�t�H�[�����擾���܂��B
std::shared_ptr<Transform> GameObject::GetTransform()
{
	return transform;
}

// �g�����X�t�H�[�����擾���܂��B
std::shared_ptr<const Transform> GameObject::GetTransform() const
{
	return transform;
}

// �V�[��������������ۂɌĂяo����܂��B
void GameObject::Start()
{

}

// �t���[�����X�V����ۂɌĂяo����܂��B
void GameObject::Update(float time, float deltaTime)
{
}

// �t���[����`�悷��ۂɌĂяo����܂��B
void GameObject::Draw(
	ID3D11DeviceContext* deviceContext, float time, float deltaTime)
{
	renderer->Draw(deviceContext);
}
