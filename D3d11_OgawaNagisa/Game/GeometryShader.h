//=============================================================================
// GeometryShader.h
// 
//=============================================================================
#pragma once

#include <d3d11.h>

// �W�I���g���[ �V�F�[�_�[��\���܂��B
class GeometryShader
{
public:
	// ���̃N���X�̃C���X�^���X���쐬���܂��B
	template <SIZE_T _Size>
	static GeometryShader* Create(const BYTE(&shaderBytecode)[_Size]);
	// ���̃N���X�̃C���X�^���X���쐬���܂��B
	static GeometryShader* Create(const void* shaderBytecode, SIZE_T bytecodeLength);
	// ���̃N���X�̃C���X�^���X���쐬���܂��B
	static GeometryShader* Create(LPCWSTR fileName, LPCSTR entryPoint, LPCSTR target);

	// �f�X�g���N�^�[
	~GeometryShader();
	// �l�C�e�B�u�����̃|�C���^�[���擾���܂��B
	ID3D11GeometryShader* GetNativePointer();

private:
	ID3D11GeometryShader* nativePointer = nullptr;
	// ���̃N���X�̃C���X�^���X�����������܂��B
	GeometryShader(ID3D11GeometryShader* nativePointer);
	// �R�s�[ �R���X�g���N�^�[
	GeometryShader(const GeometryShader&) {}
};

// ���̃N���X�̃C���X�^���X���쐬���܂��B
template <SIZE_T _Size>
static GeometryShader* GeometryShader::Create(const BYTE(&shaderBytecode)[_Size])
{
	return 	Create(shaderBytecode, _Size);
}


