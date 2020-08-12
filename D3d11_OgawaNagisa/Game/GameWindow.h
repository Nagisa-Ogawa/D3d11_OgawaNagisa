//=============================================================================
// GameWindow.h
// 
//=============================================================================
#pragma once

#include <memory>
#include <Windows.h>

// ���C�� �E�B���h�E��\���܂��B
class GameWindow final
{
public:
	// ���C�� �E�B���h�E�����������܂��B
	static std::shared_ptr<GameWindow> Create(
		LPCWSTR title, int width, int height);

	// ���C�� �E�B���h�E��j�����܂��B
	void Close();
	// �N���C�A���g�̈�̕����擾���܂��B
	int GetWidth() const;
	// �N���C�A���g�̈�̍������擾���܂��B
	int GetHeight() const;
	// �E�B���h�E �n���h�����擾���܂��B
	HWND GetHwnd() const;

	// �f�X�g���N�^�[
	~GameWindow();

private:
	// �E�B���h�E�̃^�C�g��
	LPCWSTR title = L"�^�C�g��";
	// �E�B���h�E�̕�
	int width = 640;
	// �E�B���h�E�̍���
	int height = 480;
	// �E�B���h�E�̃n���h��
	HWND hWnd = NULL;

	// ���̃N���X�̃C���X�^���X�����������܂��B
	GameWindow(LPCWSTR title, int width, int height, HWND hWnd);
	// �R�s�[ �R���X�g���N�^�[
	GameWindow(const GameWindow&) {}
	GameWindow& operator=(const GameWindow&) { return *this; }
};