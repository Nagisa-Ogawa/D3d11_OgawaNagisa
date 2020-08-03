//=============================================================================
// Shaders.cpp
// 
//=============================================================================
#include "Game.h"
#include "BasicVertexShader.h"
#include "BasicPixelShader.h"
#include "BasicGeometryShader.h"

// ���̃N���X�̐V�����C���X�^���X���쐬���܂��B
BasicVertexShader* BasicVertexShader::Create(ID3D11Device* graphicsDevice)
{
	// ���̃N���X�̃������[���m��
	auto result = new BasicVertexShader();
	if (result == nullptr)
	{
		OutputDebugString(L"�������[���m�ۂł��܂���ł����B");
		return nullptr;
	}

	// ���_�V�F�[�_�\�̍쐬
	auto hr = graphicsDevice->CreateVertexShader(
		g_BasicVertexShader,	// �R���p�C�����ꂽ�V�F�[�_�[�̃|�C���^�[
		ARRAYSIZE(g_BasicVertexShader),		// �R���p�C�����ꂽ���_�V�F�[�_�\�̃T�C�Y
		NULL,	// �N���X�����P�[�W�C���^�[�t�F�C�X�ւ̃|�C���^�[
		&result->shader);
	if (FAILED(hr)) {
		OutputDebugString(L"���_�V�F�[�_�[���쐬�ł��܂���ł����B");
		delete result;
		return nullptr;
	}

	return result;	// ����ɏ����������������ꍇ

}

// ���\�[�X���J�����܂��B
void BasicVertexShader::Release()
{
	SAFE_RELEASE(shader);
	delete this;
}

// D3D11�̃l�C�e�B�u�|�C���^�[���擾���܂��B
ID3D11VertexShader* BasicVertexShader::GetNativePointer()
{
	return shader;
}

// ���̃V�F�[�_�[�̃o�C�g�R�[�h���擾���܂��B
const BYTE* BasicVertexShader::GetBytecode()
{
	return g_BasicVertexShader;
}

// �o�C�g�R�[�h�̃T�C�Y���擾���܂��B
SIZE_T BasicVertexShader::GetBytecodeLength()
{
	return ARRAYSIZE(g_BasicVertexShader);
}

// ���̃N���X�̐V�����C���X�^���X���쐬���܂��B
BasicGeometryShader* BasicGeometryShader::Create(ID3D11Device* graphicsDevice)
{
	// ���̃N���X�̃������[���m��
	auto result = new BasicGeometryShader();
	if (result == nullptr) {
		OutputDebugString(L"�������[���m�ۂł��܂���ł����B");
		return nullptr;
	}

	// �W�I���g���[�V�F�[�_�[�̍쐬
	auto hr = graphicsDevice->CreateGeometryShader(
		g_BasicGeometryShader,
		ARRAYSIZE(g_BasicGeometryShader),
		NULL,
		&result->shader);
	if (FAILED(hr)) {
		OutputDebugString(L"�W�I���g���[�V�F�[�_�[���쐬�ł��܂���ł����B");
		delete result;
		return nullptr;
	}

	return result;
}

// ���\�[�X��������܂��B
void BasicGeometryShader::Release()
{
	SAFE_RELEASE(shader);
	delete this;
}

// D3D11�̃l�C�e�B�u�|�C���^�[���擾���܂��B
ID3D11GeometryShader* BasicGeometryShader::GetNativePointer()
{
	return shader;
}

// ���̃N���X�̐V�����C���X�^���X���쐬���܂��B
BasicPixelShader* BasicPixelShader::Create(ID3D11Device* graphicsDevice)
{
	// ���̃N���X�̃������[���m��
	auto result = new BasicPixelShader();
	if (result == nullptr) {
		OutputDebugString(L"�������[���m�ۂł��܂���ł����B");
		return nullptr;
	}

	// �s�N�Z���V�F�[�_�[�̍쐬
	auto hr = graphicsDevice->CreatePixelShader(
		g_BasicPixelShader, ARRAYSIZE(g_BasicPixelShader), NULL,
		&result->shader);
	if (FAILED(hr)) {
		OutputDebugString(L"�s�N�Z���V�F�[�_�[���쐬�ł��܂���ł����B");
		delete result;
		return nullptr;
	}

	return result;
}

// ���\�[�X��������܂��B
void BasicPixelShader::Release()
{
	SAFE_RELEASE(shader);
	delete this;
}

// D3D11�̃l�C�e�B�u�|�C���^�[���擾���܂��B
ID3D11PixelShader* BasicPixelShader::GetNativePointer()
{
	return shader;
}