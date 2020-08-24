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

// このクラスの新しいインスタンスを初期化します。
Transform::Transform()
{

}
