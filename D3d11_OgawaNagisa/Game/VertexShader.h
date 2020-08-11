//=============================================================================
// VertexShader.h
// 
//=============================================================================
#pragma once

#include "Graphics.h"

// ���_�V�F�[�_�[��\���܂��B
class VertexShader : public GraphicsResource
{
public:
	// ���̃N���X�̃C���X�^���X���쐬���܂��B
	template <SIZE_T _Size>
	static VertexShader* Create(
		GraphicsDevice* graphicsDevice, const BYTE(&shaderBytecode)[_Size]);
	// ���̃N���X�̃C���X�^���X���쐬���܂��B
	static VertexShader* Create(
		GraphicsDevice* graphicsDevice, const void* shaderBytecode, SIZE_T bytecodeLength);
	// ���̃N���X�̃C���X�^���X���쐬���܂��B
	static VertexShader* Create(
		GraphicsDevice* graphicsDevice, LPCWSTR fileName, LPCSTR entryPoint, LPCSTR target);

	// �f�X�g���N�^�[
	~VertexShader();
	// �l�C�e�B�u�����̃|�C���^�[���擾���܂��B
	ID3D11VertexShader* GetNativePointer();
	// ���̃V�F�[�_�[�̃o�C�g�R�[�h���擾���܂��B
	ID3DBlob* GetShaderBytecode();

private:
	ID3D11VertexShader* nativePointer = nullptr;
	ID3DBlob* shaderBytecode = nullptr;
	// ���̃N���X�̃C���X�^���X�����������܂��B
	VertexShader(GraphicsDevice* graphicsDevice,
		ID3D11VertexShader* nativePointer, ID3DBlob* shaderBytecode);
	// �R�s�[ �R���X�g���N�^�[
	VertexShader(const VertexShader&);
};

// ���̃N���X�̃C���X�^���X���쐬���܂��B
template <SIZE_T _Size>
static VertexShader* VertexShader::Create(GraphicsDevice* graphicsDevice, const BYTE(&shaderBytecode)[_Size])
{
	return 	Create(graphicsDevice, shaderBytecode, _Size);
}
