//=============================================================================
// stdafx.h
// 
// �W���̃V�X�e�� �C���N���[�h �t�@�C���̃C���N���[�h �t�@�C���A�܂���
// �Q�Ɖ񐔂������A�����܂�ύX����Ȃ��A�v���W�F�N�g��p�̃C���N���[�h �t�@�C��
// ���L�q���܂��B
//=============================================================================

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN		// Windows �w�b�_�[����g�p����Ă��Ȃ����������O���܂��B
// Windows �w�b�_�[ �t�@�C��:
#include <windows.h>

// CRT�ɂ�郁�����[���[�N���o
#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
// new ���Z�q�̍Ē�`
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif

#include <exception>
#include <string>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
