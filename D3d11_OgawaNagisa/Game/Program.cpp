//=============================================================================
// Program.cpp
// �G���g���[�|�C���g
//=============================================================================
#include "stdafx.h"
#include "Game.h"

// �A�v���P�[�V�����̃G���g���[�|�C���g
int WINAPI wWinMain(
	HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPWSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

#if defined(DEBUG) || defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	// �A�v���P�[�V�����̋N���ݒ�
	ApplicationSettings settings;
	settings.window.hInstance = hInstance;
	settings.window.nCmdShow = nCmdShow;
	settings.window.title = L"�^�C�g��";
	settings.window.width = 640;
	settings.window.height = 480;
#if defined(DEBUG) || defined(_DEBUG)
	// DEBUG�r���h�̍ۂ�Direct3D�̃f�o�b�O�\���@�\����������
	settings.graphics.creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	// ���b�Z�[�W ���[�v�����s
	return Game::Run(settings);
}
