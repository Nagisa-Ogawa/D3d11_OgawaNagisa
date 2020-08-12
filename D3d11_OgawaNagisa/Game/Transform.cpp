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

// このクラスの新しいインスタンスを作成します。
std::shared_ptr<Transform> Transform::Create()
{
	return shared_ptr<Transform>(new Transform());
}

// このクラスの新しいインスタンスを初期化します。
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
