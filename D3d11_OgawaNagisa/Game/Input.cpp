//=============================================================================
// Input.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "Game.h"
#include "DirectXHelper.h"

using namespace std;
using namespace Microsoft::WRL;
using namespace DirectX;
using namespace DX;

// ���̃N���X�̐V�����C���X�^���X�����������܂��B
Input::Input(HINSTANCE hInstance, std::shared_ptr<GameWindow> window)
{
	try {
		// IDirectInput8�I�u�W�F�N�g���쐬
		ThrowIfFailed(DirectInput8Create(
			hInstance,
			DIRECTINPUT_VERSION,
			IID_IDirectInput8,
			reinterpret_cast<void**>(directInput.GetAddressOf()),
			NULL));
		// �f�o�C�X���
		ThrowIfFailed(directInput->EnumDevices(
			DI8DEVCLASS_GAMECTRL,
			DIEnumDevicesCallback,
			this,
			DIEDFL_ATTACHEDONLY));

		this->window = window;
		messagePostedKey = window->messagePosted.Connect(
			std::bind1st(std::mem_fun(&Input::MessagePostedHandler), this));
	}
	catch (...) {
		directInput.Reset();
		window->messagePosted.Disconnect(messagePostedKey);
		throw;
	}
}

// �f�X�g���N�^�[
Input::~Input()
{
	window->messagePosted.Disconnect(messagePostedKey);
}

void Input::Update()
{
	// �L�[�{�[�h���͂��擾
	CopyMemory(lastKeyState, keyState, sizeof keyState);
	auto keyboardResult = GetKeyboardState(keyState);
	// �}�E�X���͂��X�V
	CopyMemory(lastMouseState, mouseState, sizeof mouseState);
	// XInput�f�o�C�X���͂��X�V
	CopyMemory(lastXInputStates, xInputStates, sizeof xInputStates);
	DWORD result;
	// �ڑ��ő吔�܂ŃR���g���[���[������
	for (DWORD index = 0; index < XUSER_MAX_COUNT; index++) {
		XINPUT_STATE state = {};
		// XInput����R���g���[���[�̏�Ԃ��擾
		result = XInputGetState(index, &state);
		if (result == ERROR_SUCCESS) {
			// ����UserIndex�ɃR���g���[���[���ڑ�����Ă���ꍇ
			xInputStates[index] = state;
		}
		else {
			// ����UserIndex�ɂ̓R���g���[���[���ڑ�����Ă��Ȃ��ꍇ
		}
	}
}

// �E�B���h�E ���b�Z�[�W����������v���V�[�W���[
std::tuple<LRESULT, bool> Input::MessagePostedHandler(WindowMessage message)
{
	switch (message.uMsg) {
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_LBUTTONDBLCLK:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MBUTTONDBLCLK:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_RBUTTONDBLCLK:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_XBUTTONDBLCLK:
	case WM_MOUSEWHEEL:
	case WM_MOUSEMOVE:
	{
		int positionX = static_cast<short>(LOWORD(message.lParam));
		int positionY = static_cast<short>(HIWORD(message.lParam));
		if (message.uMsg == WM_MOUSEWHEEL)
		{
			// WM_MOUSEWHEEL�C�x���g�̏ꍇ�A�X�N���[�����W�Ŏ擾�����̂ŁA
			// �N���C�A���g���W�ɕϊ�����B
			POINT point = { positionX, positionY };
			ScreenToClient(message.hWnd, &point);
			positionX = point.x;
			positionY = point.y;
		}

		int mouseButtonState = LOWORD(message.wParam);
		const bool isMouseButtons[] = {
			(mouseButtonState & MK_LBUTTON) != 0,
			(mouseButtonState & MK_RBUTTON) != 0,
			(mouseButtonState & MK_MBUTTON) != 0,
			(mouseButtonState & MK_XBUTTON1) != 0,
			(mouseButtonState & MK_XBUTTON2) != 0,
		};
		mousePosition = XMFLOAT2(
			static_cast<float>(positionX), static_cast<float>(positionY));
		CopyMemory(mouseState, isMouseButtons, sizeof isMouseButtons);

		std::tuple<LRESULT, bool>(0, true);
	}
	}

	return std::tuple<LRESULT, bool>(0, false);
}

bool Input::GetKeyDown(int virtualKey)
{
	if ((keyState[virtualKey] & 0x80) &&
		(lastKeyState[virtualKey] & 0x80) == 0) {
		return true;
	}
	return false;
}

bool Input::GetKey(int virtualKey)
{
	if ((keyState[virtualKey] & 0x80)) {
		return true;
	}
	return false;
}

bool Input::GetKeyUp(int virtualKey)
{
	if ((keyState[virtualKey] & 0x80) == 0 &&
		(lastKeyState[virtualKey] & 0x80)) {
		return true;
	}
	return false;
}

// �}�E�X�̃N���C�A���g���W���擾���܂��B
DirectX::XMFLOAT2 Input::GetMousePosition()
{
	return mousePosition;
}

bool Input::GetMouseButtonDown(int button)
{
	if (mouseState[button] &&
		!lastMouseState[button]) {
		return true;
	}
	return false;
}

bool Input::GetMouseButton(int button)
{
	if (mouseState[button]) {
		return true;
	}
	return false;
}

bool Input::GetMouseButtonUp(int button)
{
	if (!mouseState[button] &&
		lastMouseState[button]) {
		return true;
	}
	return false;
}

Microsoft::WRL::ComPtr<IDirectInput8> Input::GetDirectInput()
{
	return directInput;
}

Microsoft::WRL::ComPtr<IDirectInputDevice8> Input::GetJoystick(int userIndex)
{
	return joystick[userIndex];
}

Microsoft::WRL::ComPtr<IDirectInputDevice8> Input::GetGamepad(int userIndex)
{
	return gamepad[userIndex];
}

BOOL CALLBACK Input::DIEnumDevicesCallback(
	LPCDIDEVICEINSTANCE deviceInstance, LPVOID ref)
{
	auto input = static_cast<Input*>(ref);

	auto devType = GET_DIDEVICE_TYPE(deviceInstance->dwDevType);
	auto subType = GET_DIDEVICE_SUBTYPE(deviceInstance->dwDevType);

	//wchar_t message[1024];
	//swprintf_s(message,
	//	L"dwDevType : %x, devType : %x, subType : %x, tszInstanceName : %s, tszProductName : %s\n",
	//	deviceInstance->dwDevType,
	//	devType,
	//	subType,
	//	deviceInstance->tszInstanceName,
	//	deviceInstance->tszProductName);
	//OutputDebugString(message);

	ComPtr<IDirectInputDevice8> device;
	switch (devType) {
	case DI8DEVTYPE_JOYSTICK:
	{
		ThrowIfFailed(input->directInput->CreateDevice(
			deviceInstance->guidInstance, &device, NULL));
		input->joystick.push_back(device);
		break;
	}
	case DI8DEVTYPE_GAMEPAD:
	{
		ThrowIfFailed(input->directInput->CreateDevice(
			deviceInstance->guidInstance, &device, NULL));
		input->gamepad.push_back(device);
		break;
	}
	default:
		break;
	}

	return DIENUM_CONTINUE;
}
