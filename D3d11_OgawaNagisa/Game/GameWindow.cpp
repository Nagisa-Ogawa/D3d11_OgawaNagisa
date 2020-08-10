//=============================================================================
// GameWindow.cpp
// 
//=============================================================================
#include <functional>
#include "GameWindow.h"
#include "Game.h"
#include "DirectXHelper.h"

using namespace DX;

namespace
{
	// �E�B���h�E�@�N���X��
	LPCWSTR const ClassName = L"GameWindow";

	// �E�B���h�E ���b�Z�[�W����������v���V�[�W���[
	LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg) {
		case WM_CLOSE:
			// �E�B���h�E�����
			if (MessageBox(hWnd, L"�E�B���h�E����܂����H", L"���b�Z�[�W", MB_OKCANCEL) == IDOK) {
				DestroyWindow(hWnd);
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

// ���C�� �E�B���h�E���擾���܂��B
GameWindow& GameWindow::GetInstance()
{
	static GameWindow instance;
	return instance;
}

// ���C�� �E�B���h�E�����������܂��B
void GameWindow::Initialize(LPCWSTR title, int width, int height)
{
	// �ŏ��̃C���X�^���X�����̏ꍇ�𔻒�
	if (GetInstance().hWnd != NULL) {
		return;
	}

	HINSTANCE hInstance = NULL;
	// �E�B���h�E�I�u�W�F�N�g�ւ̎Q��
	HWND hWnd = NULL;

	try {
		// �Ăяo�����v���Z�X�̃��W���[���n���h�����擾
		hInstance = GetModuleHandle(NULL);
		if (hInstance == NULL) {
			// �Ō�̃G���[��throw
			throw win32_exception(GetLastError());
		}
		// �E�B���h�E �N���X��o�^����
		WNDCLASSEX wndClass = {};
		// �������̃u���b�N��0�Ŗ��߂�
		ZeroMemory(&wndClass, sizeof wndClass);
		wndClass.cbSize = sizeof(WNDCLASSEX);	// ���̍\���̂̃T�C�Y
		wndClass.lpfnWndProc = WindowProc;	// �E�B���h�E �v���V�[�W���[���w��
		wndClass.hInstance = hInstance;
		wndClass.hCursor = LoadCursor(NULL, IDC_HAND);	// �J�[�\���N���X�ւ̃n���h��
		wndClass.hbrBackground = (HBRUSH)COLOR_BACKGROUND;	// �w�i�u���V�ւ̃n���h��
		wndClass.lpszClassName = ClassName;		// �E�B���h�E�N���X��
		if (RegisterClassEx(&wndClass) == 0) {
			throw win32_exception(GetLastError());
		}

		// �N���C�A���g�̈悪�w�肵���𑜓x�ɂȂ�E�B���h�E�T�C�Y���v�Z
		RECT rect = { 0, 0, width, height };
		// �N���C�A���g�̈悪�w�肵���T�C�Y�ɂȂ�E�B���h�E���쐬
		if (!AdjustWindowRectEx(
			&rect,	// �N���C�A���g�̈�ւ̃|�C���^�[
			WS_OVERLAPPEDWINDOW,	// �E�B���h�E�X�^�C��
			FALSE,		// �E�B���h�E�Ƀ��j���[�����邩�ǂ���
			0	// �g���E�B���h�E�̃X�^�C��
		)) {
			throw win32_exception(GetLastError());
		}

		// �E�B���h�E���쐬����
		hWnd = CreateWindowEx(
			0,			// �쐬����E�B���h�E�̊g���E�B���h�E�X�^�C��
			ClassName,	// �N���X��
			title,		// �E�B���h�E�̖��O
			WS_OVERLAPPEDWINDOW,	// �쐬����E�B���h�E�̃E�B���h�E�X�^�C��
			CW_USEDEFAULT,		// �E�B���h�E�̍ŏ��̈ʒu(x��)
			CW_USEDEFAULT,		// �E�B���h�E�̍ŏ��̈ʒu(y��)
			(rect.right - rect.left),	// �E�B���h�E�̃f�o�C�X�P�ʂł̉���
			(rect.bottom - rect.top),	// �E�B���h�E�̃f�o�C�X�P�ʂł̍���
			NULL,		// �쐬����E�B���h�E�̐e��owner�ւ̃n���h��
			NULL,		// ���j���[�ւ̃n���h��
			hInstance,
			NULL
		);

		if (hWnd == NULL) {
			throw win32_exception(GetLastError());
		}

		// �E�B���h�E��\��
		ShowWindow(hWnd, SW_SHOWNORMAL);
		// �E�B���h�E�̍X�V
		UpdateWindow(hWnd);

		// �V���O���g�� �C���X�^���X��������
		GetInstance().title = title;
		GetInstance().width = width;
		GetInstance().height = height;
		GetInstance().hWnd = hWnd;
	}
	catch (...) {
		DestroyWindow(hWnd);
		// �E�B���h�E�N���X�̓o�^���������A�N���X�̃��������J��
		UnregisterClass(ClassName, hInstance);
		throw;
	}
}

// ���C�� �E�B���h�E��j�����܂��B
void GameWindow::Close()
{
	auto result = DestroyWindow(GetInstance().hWnd);
	if (result == 0) {
		throw win32_exception(GetLastError());
	}
}

// �N���C�A���g�̈�̕����擾���܂��B
int GameWindow::GetWidth()
{
	return GetInstance().width;
}

// �N���C�A���g�̈�̍������擾���܂��B
int GameWindow::GetHeight()
{
	return GetInstance().height;
}

// �E�B���h�E �n���h�����擾���܂��B
HWND GameWindow::GetHwnd()
{
	return GetInstance().hWnd;
}

// ���̃N���X�̃C���X�^���X�����������܂��B
GameWindow::GameWindow()
{

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
