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

// ���[���h�ϊ��s����쐬���郁�\�b�h
DirectX::XMMATRIX Transform::GetWorldMatrix() const
{
	XMMATRIX worldMatrix = XMMatrixIdentity();
	worldMatrix = XMMatrixMultiply(scaleMatrix, worldMatrix);
	worldMatrix = XMMatrixMultiply(rotationMatrix, worldMatrix);
	XMFLOAT4X4 fWorldMatrix;
	XMFLOAT4 fPosition;
	XMStoreFloat4x4(&fWorldMatrix, worldMatrix);
	XMStoreFloat4(&fPosition, position);
	fWorldMatrix._41 = fPosition.x; fWorldMatrix._42 = fPosition.y; fWorldMatrix._43 = fPosition.z;
	worldMatrix = XMLoadFloat4x4(&fWorldMatrix);
	return worldMatrix;
}

// rotation���Z�b�g���郁�\�b�h
void Transform::SetRotation(float pitch, float yaw, float roll)
{
	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
}

// scale���Z�b�g���郁�\�b�h
void Transform::SetScale(float x, float y, float z)
{
	scaleMatrix = XMMatrixScaling(x, y, z);
}

// ���̃N���X�̐V�����C���X�^���X�����������܂��B
Transform::Transform()
{
}
