//=============================================================================
// Game.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "Game.h"
#include <chrono>
#include "DirectXHelper.h"

using namespace std;
using namespace std::chrono;
using namespace DX;

// ���̃N���X�̐V�����C���X�^���X�����������܂��B
ApplicationSettings::ApplicationSettings()
{

}

// ���̃N���X�̃V���O���g���C���X�^���X���擾���܂��B
Game& Game::GetInstance()
{
	static Game instance;
	return instance;
}

// ���b�Z�[�W���[�v�����s���܂��B
int Game::Run(const ApplicationSettings& settings)
{
	return GetInstance().OnRun(settings);

}

// ���b�Z�[�W���[�v�����s���܂��B
int Game::OnRun(const ApplicationSettings& settings)
{
	// ���������̏ꍇ�̂݊���l�ŏ���������
	if (!isInitialized) {
		OnInitialize(settings);
	}

	std::shared_ptr<TitleScene> titleScene(new TitleScene(window, graphics, input));
	SceneManager::GetInstance().ChangeScene(titleScene);

	long long totalCount = 0;	// �ݐώ��ԁi�~���b�j
	long long deltaCount = 0;	// �o�ߎ��ԁi�~���b�j

	// ���b�Z�[�W���[�v�����s
	MSG msg = {};
	while (true) {
		// �t���[���J�n���̃J�E���g���擾
		auto begin = system_clock::now();
		float time = totalCount / 1000.0f;
		float deltaTime = deltaCount / 1000.0f;

		input->Update();

		// ���̃E�B���h�E�̃��b�Z�[�W�����݂��邩���m�F
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
			// ���b�Z�[�W���擾
			if (!GetMessage(&msg, NULL, 0, 0)) {
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// �V�[���̃t���[������
		SceneManager::GetInstance().Update(time, deltaTime);
		SceneManager::GetInstance().Draw(time, deltaTime);

		// �o�b�N�o�b�t�@�[�ɕ`�悵���C���[�W���f�B�X�v���C�ɕ\��
		graphics->GetSwapChain()->Present(1, 0);

		// �t���[���I�����̃J�E���g���擾
		auto end = system_clock::now();
		// �o�ߎ��ԂƗݐώ��Ԃ��X�V
		auto duration = end - begin;
		deltaCount = duration_cast<milliseconds>(duration).count();
		totalCount += deltaCount;
	}

	return (int)msg.wParam;

}

// �������̍ۂɌĂяo����܂��B
void Game::OnInitialize(const ApplicationSettings& settings)
{
	// �E�B���h�E���쐬
	window.reset(new GameWindow(settings.window));
	// �O���t�B�b�N�f�o�C�X���쐬
	graphics.reset(new Graphics(settings.graphics, window));
	// ���[�U�[���̓f�o�C�X���쐬
	input.reset(new Input(settings.window.hInstance, window));

	isInitialized = true;
}

// �A�v���P�[�V�������I�����܂��B
void Game::Quit()
{
	GetInstance().window->Close();

}



