//=============================================================================
// GameWindow.h
// 
//=============================================================================
#pragma once

#include <Windows.h>

// ���C���E�B���h�E��\���܂�
class GameWindow
{
public:
	// ���C�� �E�B���h�E�����������܂��B
	static void Initialize(LPCWSTR title, int width, int height);
	// ���C�� �E�B���h�E��j�����܂��B
	static void Close();
	// �N���C�A���g�̈�̕����擾���܂��B
	static int GetWidth();
	// �N���C�A���g�̈�̍������擾���܂��B
	static int GetHeight();
	// �E�B���h�E �n���h�����擾���܂��B
	static HWND GetHwnd();

	// �f�X�g���N�^�[
	~GameWindow();

private:
	// ���C�� �E�B���h�E���擾���܂��B
	static GameWindow& GetInstance();
	// �E�B���h�E�̃^�C�g��
	LPCWSTR title = L"�^�C�g��";
	// �E�B���h�E�̕�
	int width = 640;
	// �E�B���h�E�̍���
	int height = 480;
	// �E�B���h�E�̃n���h��
	HWND hWnd = NULL;

	// ���̃N���X�̃C���X�^���X�����������܂��B
	GameWindow();
	// �R�s�[ �R���X�g���N�^�[
	GameWindow(const GameWindow&) {}

};