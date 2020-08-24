//=============================================================================
// GameWindow.h
// 
//=============================================================================
#pragma once

#include <memory>
#include <functional>
#include <map>
#include <Windows.h>

/// <summary>
/// �n���h���[��o�^���ĊǗ�����N���X��\���܂��B
/// </summary>
template <typename Signature,
	typename SlotFunction = std::function<Signature>
> class Signal
{
public:
	typedef typename int KeyType;
	typedef typename std::map<KeyType, SlotFunction> ContainerType;
	typedef typename Signature SignatureType;
	typedef typename SlotFunction SlotType;

	KeyType key = 1;

	/// <summary>
	/// �o�^����Ă���n���h���[���擾���܂��B
	/// </summary>
	ContainerType& GetConnections()
	{
		return connections;
	}
	/// <summary>
	/// �C�x���g����������n���h���[��o�^���܂��B
	/// </summary>
	KeyType Connect(const SlotType&& slot)
	{
		connections[key] = slot;
		key++;
		return key;
	}
	/// <summary>
	/// �o�^����Ă���n���h���[���폜���܂��B
	/// </summary>
	void Disconnect(KeyType key)
	{
		connections.erase(key);
	}
	/// <summary>
	/// �o�^����Ă���n���h���[�����ׂĎ��s���܂��B
	/// </summary>
	void operator()(typename SlotType::argument_type args) const
	{
		for (auto& connection : connections) {
			connection.second(std::forward<SlotType::argument_type>(args));
		}
	}

private:
	ContainerType connections;
};

/// <summary>
/// <see cref="GameWindow" />���쐬���邽�߂̋L�q��\���܂��B
/// </summary>
struct WindowSettings : public WNDCLASSEXW
{
	/// <summary>
	/// �E�B���h�E �^�C�g���ł��B
	/// </summary>
	LPCWSTR title = L"Game�^�C�g��";
	/// <summary>
	/// �N���C�A���g�̈�̕��ł��B
	/// </summary>
	UINT width = 640;
	/// <summary>
	/// �N���C�A���g�̈�̍����ł��B
	/// </summary>
	UINT height = 480;
	/// <summary>
	/// 
	/// </summary>
	int nCmdShow = SW_SHOWNORMAL;

	/// <summary>
	/// ���̃N���X�̐V�����C���X�^���X�����������܂��B
	/// </summary>
	WindowSettings();
};

/// <summary>
/// �E�B���h�E ���b�Z�[�W��\���܂��B
/// </summary>
struct WindowMessage
{
	HWND hWnd;
	UINT uMsg;
	WPARAM wParam;
	LPARAM lParam;
};

// ���C�� �E�B���h�E��\���܂��B
class GameWindow final
{
public:
	typedef Signal<std::tuple<LRESULT, bool>(WindowMessage)> MessagePosted;

	// �N���C�A���g�̈�̕����擾���܂��B
	int GetWidth() const;
	// �N���C�A���g�̈�̍������擾���܂��B
	int GetHeight() const;
	// �E�B���h�E �n���h�����擾���܂��B
	HWND GetHwnd() const;
	// ���̃E�B���h�E��\�����Ă��郂�j�^�[�̃n���h�����擾���܂��B
	HMONITOR GetHMonitor() const;

	// ���̃N���X�̐V�����C���X�^���X�����������܂��B
	GameWindow(const WindowSettings& settings);
	// �f�X�g���N�^�[
	~GameWindow();

	// ���C�� �E�B���h�E��j�����܂��B
	void Close();

	// �E�B���h�E ���b�Z�[�W����������n���h���[��o�^���܂��B
	MessagePosted messagePosted;

private:
	// 
	HINSTANCE hInstance = NULL;
	// �E�B���h�E �N���X��
	std::wstring className;
	// �E�B���h�E�̃^�C�g��
	std::wstring title;
	// �E�B���h�E�̕�
	int width = 640;
	// �E�B���h�E�̍���
	int height = 480;
	// �E�B���h�E�̃n���h��
	HWND hWnd = NULL;
	// ���̃E�B���h�E��\�����Ă��郂�j�^�[�̃n���h��
	HMONITOR hMonitor = NULL;

	// �E�B���h�E ���b�Z�[�W����������v���V�[�W���[
	static LRESULT CALLBACK WndProc(
		HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	// �E�B���h�E�Ƀ��b�Z�[�W�������Ă����ۂɌĂяo����܂��B
	LRESULT OnMessagePosted(const WindowMessage& message);

	// �R�s�[�Ƒ�����Z���֎~
	GameWindow(const GameWindow&) {}
	GameWindow& operator=(const GameWindow&) { return *this; }
};