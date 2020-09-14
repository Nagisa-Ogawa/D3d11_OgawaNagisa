//=============================================================================
// Game.h
// 
//=============================================================================
#pragma once

#include <memory>
#include <wrl.h>
#include <vector>
#include <Windows.h>
#include <DirectXMath.h>
#include <Xinput.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include "Graphics.h"
#include "Scene.h"
#include "SceneManager.h"
#include "GameObject.h"

// ���̓f�[�^����������N���X��\���܂��B
class Input final
{
public:
	// ���̃N���X�̐V�����C���X�^���X�����������܂��B
	Input(HINSTANCE hInstance, std::shared_ptr<GameWindow> window);
	// �f�X�g���N�^�[
	~Input();

	void Update();

	// �E�B���h�E ���b�Z�[�W����������v���V�[�W���[
	std::tuple<LRESULT, bool> MessagePostedHandler(WindowMessage message);

	bool GetKeyDown(int virtualKey);
	bool GetKey(int virtualKey);
	bool GetKeyUp(int virtualKey);

	// �}�E�X�̃N���C�A���g���W���擾���܂��B
	DirectX::XMFLOAT2 GetMousePosition();
	// 
	bool GetMouseButtonDown(int button);
	bool GetMouseButton(int button);
	bool GetMouseButtonUp(int button);

	Microsoft::WRL::ComPtr<IDirectInput8> GetDirectInput();
	Microsoft::WRL::ComPtr<IDirectInputDevice8> GetJoystick(int userIndex);
	Microsoft::WRL::ComPtr<IDirectInputDevice8> GetGamepad(int userIndex);

private:
	HINSTANCE hInstance;
	std::shared_ptr<GameWindow> window;
	GameWindow::MessagePosted::KeyType messagePostedKey;

	// �L�[�{�[�h
	BYTE lastKeyState[256] = {};
	BYTE keyState[256] = {};
	// �}�E�X
	DirectX::XMFLOAT2 mousePosition = { 0.0f,0.0f };
	bool lastMouseState[5] = {};
	bool mouseState[5] = {};
	// XInput�f�o�C�X����
	XINPUT_STATE lastXInputStates[XUSER_MAX_COUNT] = {};
	XINPUT_STATE xInputStates[XUSER_MAX_COUNT] = {};
	Microsoft::WRL::ComPtr<IDirectInput8> directInput;
	std::vector<Microsoft::WRL::ComPtr<IDirectInputDevice8>> joystick;
	std::vector<Microsoft::WRL::ComPtr<IDirectInputDevice8>> gamepad;

	static BOOL CALLBACK DIEnumDevicesCallback(
		LPCDIDEVICEINSTANCE deviceInstance, LPVOID ref);

	// �R�s�[�Ƒ�����Z���֎~
	Input(const Input&) {}
	Input& operator=(const Input&) { return *this; }
};

// �g�����X�t�H�[��
class Transform final
{
public:
	DirectX::XMVECTOR scale = { 1.0f, 1.0f, 1.0f, 1.0f };
	DirectX::XMVECTOR rotation = DirectX::XMQuaternionIdentity();
	DirectX::XMVECTOR position = { 0.0f, 0.0f, 0.0f, 1.0f };

	DirectX::XMMATRIX GetWorldMatrix() const;

	// ���̃N���X�̐V�����C���X�^���X�����������܂��B
	Transform();

private:
	// �R�s�[�Ƒ�����Z���֎~
	Transform(const Transform&) {}
	Transform& operator=(const Transform&) { return *this; }
};

// �J������\���܂��B
class Camera
{
public:
	// ��ʃN���A�[�Ɏg�p����J���[
	DirectX::XMVECTORF32 clearColor = { 53 / 255.0f, 70 / 255.0f, 166 / 255.0f, 1.0f };
	// ���_�̈ʒu���W
	DirectX::XMVECTOR eyePosition = { 0.0f, 1.0f, -10.0f, 1.0f };
	// �����_
	DirectX::XMVECTOR focusPosition = { 0.0f, 1.0f, 0.0f, 1.0f };
	// �J������ UP �x�N�g��
	DirectX::XMVECTOR upDirection = { 0.0f, 1.0f, 0.0f, 0.0f };
	// �v���W�F�N�V���� �p�����[�^�[
	float fov = DirectX::XM_PIDIV4;
	float aspectHeightByWidth = 640.0f / 480;
	float nearZ = 0.1f;
	float farZ = 1000.0f;
	// �r���[�|�[�g
	D3D11_VIEWPORT viewport = { 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f };

	// �r���[�ϊ��s����擾���܂��B
	DirectX::XMMATRIX GetViewMatrix() const;
	// �v���W�F�N�V�����ϊ��s����擾���܂��B
	DirectX::XMMATRIX GetProjectionMatrix() const;

	// ���̃N���X�̐V�����C���X�^���X�����������܂��B
	Camera(std::shared_ptr<GameWindow> window);

	// ���̃I�u�W�F�N�g������������ۂɈ�x�Ăяo����܂��B
	void Start();

private:
	std::shared_ptr<GameWindow> window;

	// �R�s�[�Ƒ�����Z���֎~
	Camera(const Camera&) {}
	Camera& operator=(const Camera&) { return *this; }
};

// �A�v���P�[�V�����������ɂ��Ă̋L�q��\���܂��B
struct ApplicationSettings
{
	// �E�B���h�E�ݒ�
	WindowSettings window;
	// �O���t�B�b�N�X�ݒ�
	GraphicsSettings graphics;

	// ���̃N���X�̐V�����C���X�^���X�����������܂��B
	ApplicationSettings();
};

// �A�v���P�[�V�����S�̂�\���܂��B
class Game
{
public:
	// ���b�Z�[�W���[�v�����s���܂��B
	static int Run(const ApplicationSettings& settings);
	// �A�v���P�[�V�������I�����܂��B
	static void Quit();

private:
	// ���̃N���X�̃V���O���g���C���X�^���X���擾���܂��B
	static Game& GetInstance();

	// ���łɏ������ς݂̏ꍇ��true�A����ȊO��false
	bool isInitialized = false;
	// ���C�� �E�B���h�E
	std::shared_ptr<GameWindow> window;
	// �O���t�B�b�N�X�@�\
	std::shared_ptr<Graphics> graphics;
	// ���[�U�[���̓f�o�C�X
	std::shared_ptr<Input> input;

	// �������̍ۂɌĂяo����܂��B
	void OnInitialize(const ApplicationSettings& settings);
	// ���b�Z�[�W���[�v�����s����ۂɌĂяo����܂��B
	int OnRun(const ApplicationSettings& settings);
};
