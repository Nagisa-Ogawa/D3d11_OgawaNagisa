//=============================================================================
// GameWindow.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "GameWindow.h"
#include "Game.h"
#include "DirectXHelper.h"

using namespace DX;

namespace
{
	// �E�B���h�E �N���X��
	LPCWSTR const ClassName = L"GameWindow";

	// �E�B���h�E ���b�Z�[�W����������v���V�[�W���[
	LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg) {
		case WM_CLOSE:
			if (MessageBox(hWnd, L"�E�B���h�E����܂����H", L"���b�Z�[�W", MB_OKCANCEL) == IDOK) {
				DestroyWindow(hWnd);	// �E�B���h�E�����
			}
			return 0;

		case WM_DESTROY:
			// �A�v���P�[�V�������I��
			PostQuitMessage(0);
			return 0;
		}

		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}

// ���C�� �E�B���h�E�����������܂��B
GameWindow* GameWindow::Create(LPCWSTR title, int width, int height)
{
	GameWindow* retVal = nullptr;
	HINSTANCE hInstance = NULL;
	HWND hWnd = NULL;

	try {
		// �Ăяo�����v���Z�X�̃��W���[���n���h�����擾
		hInstance = GetModuleHandle(NULL);
		if (hInstance == NULL) {
			throw win32_exception(GetLastError());
		}
		// �E�B���h�E �N���X��o�^����
		WNDCLASSEX wndClass = {};
		ZeroMemory(&wndClass, sizeof wndClass);
		wndClass.cbSize = sizeof(WNDCLASSEX);
		wndClass.lpfnWndProc = WindowProc;	// �E�B���h�E �v���V�[�W���[���w��
		wndClass.hInstance = hInstance;
		wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndClass.hbrBackground = (HBRUSH)COLOR_BACKGROUND;
		wndClass.lpszClassName = ClassName;
		if (RegisterClassEx(&wndClass) == 0) {
			throw win32_exception(GetLastError());
		}

		// �N���C�A���g�̈悪�w�肵���𑜓x�ɂȂ�E�B���h�E�T�C�Y���v�Z
		RECT rect = { 0, 0, width, height };
		if (!AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, FALSE, 0)) {
			throw win32_exception(GetLastError());
		}

		// �E�B���h�E���쐬����
		hWnd = CreateWindowEx(
			0, ClassName, title, WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT,
			(rect.right - rect.left), (rect.bottom - rect.top),
			NULL, NULL, hInstance, NULL);
		if (hWnd == NULL) {
			throw win32_exception(GetLastError());
		}

		// �E�B���h�E��\��
		ShowWindow(hWnd, SW_SHOWNORMAL);
		UpdateWindow(hWnd);

		retVal = new GameWindow(title, width, height, hWnd);
		return retVal;
	}
	catch (...) {
		DestroyWindow(hWnd);
		UnregisterClass(ClassName, hInstance);
		throw;
	}
	return nullptr;
}

// ���C�� �E�B���h�E��j�����܂��B
void GameWindow::Close()
{
	auto result = DestroyWindow(hWnd);
	if (result == 0) {
		throw win32_exception(GetLastError());
	}
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

// ���̃N���X�̃C���X�^���X�����������܂��B
GameWindow::GameWindow(LPCWSTR title, int width, int height, HWND hWnd)
{
	this->title = title;
	this->width = width;
	this->height = height;
	this->hWnd = hWnd;
}

// �f�X�g���N�^�[
GameWindow::~GameWindow()
{
	auto result = UnregisterClass(ClassName, GetModuleHandle(NULL));
	if (result == 0) {
		char message[1024] = {};
		FormatMessageA(
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // �f�t�H���g ���[�U�[����
			message, ARRAYSIZE(message),
			NULL);
		OutputDebugStringA(message);
	}
}
