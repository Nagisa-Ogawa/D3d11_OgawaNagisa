//=============================================================================
// Transform.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "Game.h"
#include "DirectXHelper.h"

using namespace std;
using namespace DirectX;
using namespace DX;

// ���̃N���X�̐V�����C���X�^���X���쐬���܂��B
std::shared_ptr<Transform> Transform::Create()
{
	return shared_ptr<Transform>(new Transform());
}

// ���̃N���X�̐V�����C���X�^���X�����������܂��B
Transform::Transform()
{

}

DirectX::XMMATRIX Transform::GetWorldMatrix() const
{
	return XMMatrixTransformation(
		XMVectorZero(), XMQuaternionIdentity(), scale,
		XMVectorZero(), rotation,
		position);
}
