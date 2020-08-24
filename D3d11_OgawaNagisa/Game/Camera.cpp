//=============================================================================
// Camera.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "Game.h"

using namespace DirectX;

// ���̃N���X�̐V�����C���X�^���X�����������܂��B
Camera::Camera(std::shared_ptr<GameWindow> window)
{
	this->window = window;
}

// �r���[�ϊ��s����擾���܂��B
DirectX::XMMATRIX Camera::GetViewMatrix() const
{
	return XMMatrixLookAtLH(eyePosition, focusPosition, upDirection);
}

// �v���W�F�N�V�����ϊ��s����擾���܂��B
DirectX::XMMATRIX Camera::GetProjectionMatrix() const
{
	return XMMatrixPerspectiveFovLH(fov, aspectHeightByWidth, nearZ, farZ);
}

// ���̃I�u�W�F�N�g������������ۂɈ�x�Ăяo����܂��B
void Camera::Start()
{
	// �r���[�|�[�g
	viewport.Width = static_cast<FLOAT>(window->GetWidth());
	viewport.Height = static_cast<FLOAT>(window->GetHeight());
	// �J�����p�����[�^�[���X�V
	eyePosition = XMVectorSet(0.0f, 1.0f, -10.0f, 1.0f);
	focusPosition = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
	upDirection = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	// �v���W�F�N�V���� �p�����[�^�[���X�V
	fov = XM_PIDIV4;
	aspectHeightByWidth = static_cast<float>(window->GetWidth()) / window->GetHeight();
	nearZ = 0.1f;
	farZ = 1000.0f;
}
