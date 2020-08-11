//=============================================================================
// GeometryShader.h
// 
//=============================================================================
#pragma once

#include "Graphics.h"

// �W�I���g���[ �V�F�[�_�[��\���܂��B
class GeometryShader : public GraphicsResource
{
public:
	// ���̃N���X�̃C���X�^���X���쐬���܂��B
	template <SIZE_T _Size>
	static GeometryShader* Create(
		GraphicsDevice* graphicsDevice, const BYTE(&shaderBytecode)[_Size]);
	// ���̃N���X�̃C���X�^���X���쐬���܂��B
	static GeometryShader* Create(
		GraphicsDevice* graphicsDevice, const void* shaderBytecode, SIZE_T bytecodeLength);
	// ���̃N���X�̃C���X�^���X���쐬���܂��B
	static GeometryShader* Create(
		GraphicsDevice* graphicsDevice, LPCWSTR fileName, LPCSTR entryPoint, LPCSTR target);

	// �f�X�g���N�^�[
	~GeometryShader();
	// �l�C�e�B�u�����̃|�C���^�[���擾���܂��B
	ID3D11GeometryShader* GetNativePointer();

private:
	ID3D11GeometryShader* nativePointer = nullptr;
	// ���̃N���X�̃C���X�^���X�����������܂��B
	GeometryShader(GraphicsDevice* graphicsDevice,
		ID3D11GeometryShader* nativePointer);
	// �R�s�[ �R���X�g���N�^�[
	GeometryShader(const GeometryShader&);
};

// ���̃N���X�̃C���X�^���X���쐬���܂��B
template <SIZE_T _Size>
static GeometryShader* GeometryShader::Create(
	GraphicsDevice* graphicsDevice, const BYTE(&shaderBytecode)[_Size])
{
	return 	Create(graphicsDevice, shaderBytecode, _Size);
}
