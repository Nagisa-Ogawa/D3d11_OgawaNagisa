//=============================================================================
// GameWindow.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "GameWindow.h"
#include "Game.h"
#include "DirectXHelper.h"

using namespace std;
using namespace DX;

/// <summary>
/// ���̃N���X�̐V�����C���X�^���X�����������܂��B
/// </summary>
WindowSettings::WindowSettings()
{
	cbSize = sizeof(WNDCLASSEXW);
	style = CS_HREDRAW | CS_VREDRAW;
	lpfnWndProc = nullptr;
	cbClsExtra = 0;
	cbWndExtra = 0;
	// �Ăяo�����v���Z�X�̃��W���[���n���h�����擾
	hInstance = GetModuleHandle(NULL);
	if (hInstance == NULL) {
		throw Win32Exception(GetLastError());
	}
	hIcon = NULL;
	hCursor = LoadCursor(hInstance, IDC_ARROW);
	hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	lpszMenuName = NULL;
	lpszClassName = L"GameWindowClass";
	hIconSm = NULL;
}
#include <string>
/// <summary>
/// ���̃N���X�̐V�����C���X�^���X�����������܂��B
/// </summary>
GameWindow::GameWindow(const WindowSettings& settings)
{
	try {
		// �E�B���h�E�ݒ���C��
		WindowSettings wndClass(settings);
		if (wndClass.lpfnWndProc == nullptr) {
			wndClass.lpfnWndProc = WndProc;
		}
		if (wndClass.hInstance == NULL) {
			// �Ăяo�����v���Z�X�̃��W���[���n���h�����擾
			wndClass.hInstance = GetModuleHandle(NULL);
			ThrowIfFailed(wndClass.hInstance != NULL);
		}

		title = wndClass.title;
		width = wndClass.width;
		height = wndClass.height;
		className = wndClass.lpszClassName;
		hInstance = wndClass.hInstance;

		// �E�B���h�E �N���X��o�^����
		ThrowIfFailed(0 != RegisterClassEx(&wndClass));

		// �N���C�A���g�̈悪�w�肵���𑜓x�ɂȂ�E�B���h�E�T�C�Y���v�Z
		RECT rect = { 0, 0, width, height };
		ThrowIfFailed(AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, FALSE, 0));

		// �E�B���h�E���쐬����
		hWnd = CreateWindowEx(
			0, wndClass.lpszClassName, title.c_str(), WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT,
			(rect.right - rect.left), (rect.bottom - rect.top),
			NULL, NULL, hInstance, NULL);
		ThrowIfFailed(hWnd != NULL);

		// �쐬�����E�B���h�E��\�����Ă��郂�j�^�[���擾
		hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTOPRIMARY);

		// ���[�U�[�f�[�^�Ƃ��ăC���X�^���X��o�^
		auto previousValue = GetWindowLongPtr(hWnd, GWLP_USERDATA);
		SetLastError(0);
		auto result = SetWindowLongPtr(
			hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
		ThrowIfFailed((result == previousValue) && (previousValue != 0));

		// �E�B���h�E��\��
		ShowWindow(hWnd, SW_SHOWNORMAL);
		UpdateWindow(hWnd);
	}
	catch (...) {
		DestroyWindow(hWnd);
		UnregisterClass(settings.lpszClassName, hInstance);
		throw;
	}
}

// �f�X�g���N�^�[
GameWindow::~GameWindow()
{
	DestroyWindow(hWnd);
	UnregisterClass(className.c_str(), hInstance);
}

// ���C�� �E�B���h�E��j�����܂��B
void GameWindow::Close()
{
	ThrowIfFailed(0 != DestroyWindow(hWnd));
}

// �N���C�A���g�̈�̕����擾���܂��B
int GameWindow::GetWidth() const
{
	return width;
}

// �N���C�A���g�̈�̍������擾���܂��B
int GameWindow::GetHeight() const
{
	return height;
}

// �E�B���h�E �n���h�����擾���܂��B
HWND GameWindow::GetHwnd() const
{
	return hWnd;
}

// ���̃E�B���h�E��\�����Ă��郂�j�^�[�̃n���h�����擾���܂��B
HMONITOR GameWindow::GetHMonitor() const
{
	return hMonitor;
}

// �E�B���h�E ���b�Z�[�W����������v���V�[�W���[
LRESULT CALLBACK GameWindow::WndProc(
	HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// ���[�U�[�f�[�^�Ƃ��ăC���X�^���X��o�^
	auto instance = reinterpret_cast<GameWindow*>(
		GetWindowLongPtr(hWnd, GWLP_USERDATA));
	if (instance != nullptr) {
		return instance->OnMessagePosted(
			WindowMessage{ hWnd, uMsg, wParam, lParam });
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

// �E�B���h�E ���b�Z�[�W����������v���V�[�W���[
LRESULT GameWindow::OnMessagePosted(const WindowMessage& message)
{
	for (auto connection : messagePosted.GetConnections()) {
		auto result = connection.second(message);
		if (get<1>(result)) {
			return 0;
		}
	}

	switch (message.uMsg) {
	case WM_CLOSE:
		if (MessageBox(hWnd, L"�E�B���h�E����܂����H", L"���b�Z�[�W", MB_OKCANCEL) == IDOK) {
			DestroyWindow(hWnd);	// �E�B���h�E�����
		}
		return 0;
	case WM_DESTROY:
		// �A�v���P�[�V�������I��
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(
			message.hWnd, message.uMsg, message.wParam, message.lParam);
	}
}
