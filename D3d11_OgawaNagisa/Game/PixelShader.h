//=============================================================================
// PixelShader.h
// 
//=============================================================================
#pragma once

#include "Graphics.h"

// �s�N�Z�� �V�F�[�_�[��\���܂��B
class PixelShader : public GraphicsResource
{
public:
	// ���̃N���X�̃C���X�^���X���쐬���܂��B
	template <SIZE_T _Size>
	static PixelShader* Create(
		GraphicsDevice* graphicsDevice, const BYTE(&shaderBytecode)[_Size]);
	// ���̃N���X�̃C���X�^���X���쐬���܂��B
	static PixelShader* Create(
		GraphicsDevice* graphicsDevice, const void* shaderBytecode, SIZE_T bytecodeLength);
	// ���̃N���X�̃C���X�^���X���쐬���܂��B
	static PixelShader* Create(
		GraphicsDevice* graphicsDevice, LPCWSTR fileName, LPCSTR entryPoint, LPCSTR target);

	// �f�X�g���N�^�[
	~PixelShader();
	// �l�C�e�B�u�����̃|�C���^�[���擾���܂��B
	ID3D11PixelShader* GetNativePointer();

private:
	ID3D11PixelShader* nativePointer = nullptr;
	// ���̃N���X�̃C���X�^���X�����������܂��B
	PixelShader(GraphicsDevice* graphicsDevice, ID3D11PixelShader* nativePointer);
	// �R�s�[ �R���X�g���N�^�[
	PixelShader(const PixelShader&);
};

// ���̃N���X�̃C���X�^���X���쐬���܂��B
template <SIZE_T _Size>
static PixelShader* PixelShader::Create(GraphicsDevice* graphicsDevice, const BYTE(&shaderBytecode)[_Size])
{
	return 	Create(graphicsDevice, shaderBytecode, _Size);
}
