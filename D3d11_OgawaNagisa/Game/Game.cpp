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
	{
		//// ���s�����̏��
		//struct ConstantDirectionalLight {
		//	XMFLOAT4 direction = XMFLOAT4(-0.5f, 0.5f, -1.0f, 0.0f);
		//	XMFLOAT4 color = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f);
		//};

		//// ���̕\�ʂ̎���
		//struct ConstantMaterial
		//{
		//	XMFLOAT3 baseColor = XMFLOAT3(1.0f, 1.0f, 1.0f);
		//	float metallic = 1.0f;
		//	XMFLOAT3 specular = XMFLOAT3(0.5f, 0.5f, 0.5f);
		//	float roughness = 0.5f;
		//};
		//
		//struct ConstantMatricesBuffer
		//{
		//	XMFLOAT4X4 worldMatrix;
		//	XMFLOAT4X4 viewMatrix;
		//	XMFLOAT4X4 projectionMatrix;
		//	XMFLOAT4X4 worldViewProjectionMatrix;
		//	// ���_���W
		//	XMFLOAT4 viewPosition = XMFLOAT4(0, 1, -10, 1);
		//	// ����
		//	ConstantDirectionalLight constantDirectionalLight;
		//	// ���̂̎���
		//	ConstantMaterial material;
		//};

		//// �萔�o�b�t�@�[���쐬
		//ConstantBuffer* constantBuffer = nullptr;
		//constantBuffer = ConstantBuffer::Create(graphicsDevice, sizeof(ConstantMatricesBuffer));
		//if (constantBuffer == nullptr)
		//{
		//	OutputDebugString(L"�萔�o�b�t�@�[���쐬�ł��܂���ł����B");
		//	return -1;
		//}


		//	//fopen_s(&fp,"earth.data", "rb");

		//	//if (fp == NULL)
		//	//{
		//	//	printf("���s");
		//	//	return -1;
		//	//}

		//	//fread(source, sizeof(uint32_t), 1024*1024, fp);

		//	//fclose(fp);
		//}

		//// bitmap�f�[�^�̓ǂݍ���
		//ReadBitMap readBitMap;
		//// �f�[�^���擾
		//uint32_t* source = readBitMap.ReadFromBitMap("../resource/image/earth.bmp");
		//if (source == nullptr)
		//{
		//	OutputDebugString(L"�摜�f�[�^��ǂݍ��߂܂���ł����B");
		//	return -1;
		//}
		//// �e�N�X�`���[���쐬
		//Texture2D* texture =
		//	Texture2D::Create(graphicsDevice, readBitMap.width, readBitMap.height, DXGI_FORMAT_R8G8B8A8_UNORM, false);
		//if (texture == nullptr) {
		//	OutputDebugString(L"�e�N�X�`���[���쐬�ł��܂���ł����B");
		//	return -1;
		//}
		//texture->width = readBitMap.width;
		//// �s�N�Z���f�[�^���X�V
		//texture->SetData(source);
		//// �f�[�^���J��
		//readBitMap.Release();

		//float time = 0;

	}
	return GetInstance().OnRun(settings);

}

// ���b�Z�[�W���[�v�����s���܂��B
int Game::OnRun(const ApplicationSettings& settings)
{
	// ���������̏ꍇ�̂݊���l�ŏ���������
	if (!isInitialized) {
		OnInitialize(settings);
	}

	shared_ptr<Scene> scene(new Scene(window, graphics, input));
	scene->Start();

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
		scene->Update(time, deltaTime);
		scene->Draw(time, deltaTime);

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



