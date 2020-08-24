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

DirectX::XMMATRIX Transform::GetWorldMatrix() const
{
	return XMMatrixTransformation(
		XMVectorZero(), XMQuaternionIdentity(), scale,
		XMVectorZero(), rotation,
		position);
}

// ���̃N���X�̐V�����C���X�^���X�����������܂��B
Transform::Transform()
{

}
