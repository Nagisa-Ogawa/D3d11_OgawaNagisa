//=============================================================================
// VertexShader.cpp
// 
//=============================================================================
#include <d3dcompiler.h>
#include "VertexShader.h"
#include "Game.h"
#include "DirectXHelper.h"

namespace
{
	// �t�@�C������V�F�[�_�[���R���p�C�����܂��B
	ID3DBlob* CompileShaderFromFile(LPCWSTR fileName, LPCSTR entryPoint, LPCSTR target)
	{
		ID3DBlob* retVal = nullptr;
		ID3DBlob* errorMsgs = nullptr;
		try {
			// �R���p�C�� �t���O
			UINT creationFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
			creationFlags |= D3DCOMPILE_DEBUG;
#endif
			// �V�F�[�_�[���R���p�C��
			DX::ThrowIfFailed(
				D3DCompileFromFile(
					fileName,
					NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE,
					entryPoint, target, creationFlags, 0,
					&retVal, &errorMsgs));

			return retVal;
		}
		catch (...) {
			SAFE_RELEASE(retVal);
			if (errorMsgs != nullptr) {
				auto message = static_cast<LPCSTR>(errorMsgs->GetBufferPointer());
				OutputDebugStringA(message);
				SAFE_RELEASE(errorMsgs);
			}
			throw;
		}
		return nullptr;
	}
}

// ���̃N���X�̃C���X�^���X���쐬���܂��B
VertexShader* VertexShader::Create(const void* shaderBytecode, SIZE_T bytecodeLength)
{
	VertexShader* retVal = nullptr;
	ID3D11VertexShader* nativePointer = nullptr;
	ID3DBlob* blob = nullptr;
	try {
		// �V�F�[�_�[���쐬
		DX::ThrowIfFailed(
			Graphics::GetGraphicsDevice()->CreateVertexShader(
				shaderBytecode,		// �R���p�C�����ꂽ�V�F�[�_�[�̃|�C���^�[
				bytecodeLength,		// �R���p�C�����ꂽ���_�V�F�[�_�\�̃T�C�Y
				NULL,	// �N���X�����P�[�W�C���^�[�t�F�C�X�ւ̃|�C���^�[
				&nativePointer));
		// �����Ŏw�肳�ꂽ�o�C�g�R�[�h�̃R�s�[���쐬
		DX::ThrowIfFailed(D3DCreateBlob(bytecodeLength, &blob));
		CopyMemory(blob->GetBufferPointer(), shaderBytecode, bytecodeLength);
		// ���̃N���X�̃C���X�^���X��������
		retVal = new VertexShader(nativePointer, blob);
		SAFE_RELEASE(nativePointer);
		SAFE_RELEASE(blob);

		return retVal;
	}
	catch (...) {
		SAFE_RELEASE(nativePointer);
		SAFE_RELEASE(blob);
		SAFE_DELETE(retVal);
		throw;
	}
	return nullptr;
}

// ���̃N���X�̃C���X�^���X���쐬���܂��B
VertexShader* VertexShader::Create(LPCWSTR fileName, LPCSTR entryPoint, LPCSTR target)
{
	VertexShader* retVal = nullptr;
	ID3D11VertexShader* nativePointer = nullptr;
	ID3DBlob* shaderBytecode = nullptr;
	try {
		// �V�F�[�_�[���R���p�C��
		shaderBytecode = CompileShaderFromFile(fileName, entryPoint, target);
		// �V�F�[�_�[���쐬
		DX::ThrowIfFailed(
			Graphics::GetGraphicsDevice()->CreateVertexShader(
				shaderBytecode->GetBufferPointer(),
				shaderBytecode->GetBufferSize(),
				NULL,
				&nativePointer));
		// ���̃N���X�̃C���X�^���X��������
		retVal = new VertexShader(nativePointer, shaderBytecode);
		SAFE_RELEASE(nativePointer);
		SAFE_RELEASE(shaderBytecode);

		return retVal;
	}
	catch (...) {
		SAFE_RELEASE(shaderBytecode);
		SAFE_RELEASE(nativePointer);
		SAFE_DELETE(retVal);
		throw;
	}
	return nullptr;
}

// ���̃N���X�̃C���X�^���X�����������܂��B
VertexShader::VertexShader(
	ID3D11VertexShader* nativePointer, ID3DBlob* shaderBytecode)
{
	nativePointer->AddRef();
	shaderBytecode->AddRef();
	this->nativePointer = nativePointer;
	this->shaderBytecode = shaderBytecode;
}

// �f�X�g���N�^�[
VertexShader::~VertexShader()
{
	SAFE_RELEASE(shaderBytecode);
	SAFE_RELEASE(nativePointer);
}

// �l�C�e�B�u�����̃|�C���^�[���擾���܂��B
ID3D11VertexShader* VertexShader::GetNativePointer()
{
	return nativePointer;
}

// ���̃V�F�[�_�[�̃o�C�g�R�[�h���擾���܂��B
ID3DBlob* VertexShader::GetShaderBytecode()
{
	return shaderBytecode;
}
