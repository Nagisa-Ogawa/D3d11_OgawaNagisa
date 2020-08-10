//=============================================================================
// Program.cpp
// �G���g���[�|�C���g
//=============================================================================
#include "Game.h"

// �A�v���P�[�V�����̃G���g���[�|�C���g
int WINAPI wWinMain(
	_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
// �f�o�b�O���Ȃ�
#if defined(DEBUG) || defined(_DEBUG)
	// �A�v���P�[�V�����I�����Ƀ��������[�N���|�[�g���o�͂���
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	// �A�v���P�[�V�����̋N���ݒ�
	Game::Initialize(L"Game�^�C�g��", 640, 480);
	// ���b�Z�[�W ���[�v�����s
	return Game::Run();
}
