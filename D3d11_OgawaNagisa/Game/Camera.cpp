//=============================================================================
// Camera.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "Game.h"
#include "DirectXHelper.h"

using namespace DX;
using namespace DirectX;

// このクラスの新しいインスタンスを作成します。
std::shared_ptr<Camera> Camera::Create(std::shared_ptr<GameWindow> window)
{
	return std::shared_ptr<Camera>(new Camera(window));
}

// このクラスの新しいインスタンスを初期化します。
Camera::Camera(std::shared_ptr<GameWindow> window)
{
	this->window = window;
}

// ビュー変換行列を取得します。
XMMATRIX Camera::GetViewMatrix() const
{
	return XMMatrixLookAtLH(eyePosition, focusPosition, upDirection);
}

// プロジェクション変換行列を取得します。
XMMATRIX Camera::GetProjectionMatrix() const
{
	return XMMatrixPerspectiveFovLH(fov, aspectHeightByWidth, nearZ, farZ);
}

// このオブジェクトを初期化する際に一度呼び出されます。
void Camera::Start()
{
	// ビューポート
	viewport.Width = static_cast<FLOAT>(window->GetWidth());
	viewport.Height = static_cast<FLOAT>(window->GetHeight());
	// カメラパラメーターを更新
	eyePosition = XMVectorSet(0.0f, 1.0f, -10.0f, 1.0f);
	focusPosition = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
	upDirection = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	// プロジェクション パラメーターを更新
	fov = XM_PIDIV4;
	aspectHeightByWidth = static_cast<float>(window->GetWidth()) / window->GetHeight();
	nearZ = 0.1f;
	farZ = 1000.0f;
}
