//=============================================================================
// PixelShader.cpp
// 
//=============================================================================
#include <d3dcompiler.h>
#include "PixelShader.h"
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
PixelShader* PixelShader::Create(const void* shaderBytecode, SIZE_T bytecodeLength)
{
	PixelShader* retVal = nullptr;
	ID3D11PixelShader* nativePointer = nullptr;
	try {
		// �V�F�[�_�[���쐬
		DX::ThrowIfFailed(
			Graphics::GetGraphicsDevice()->CreatePixelShader(
				shaderBytecode, bytecodeLength,
				NULL,
				&nativePointer));
		// ���̃N���X�̃C���X�^���X��������
		retVal = new PixelShader(nativePointer);
		SAFE_RELEASE(nativePointer);

		return retVal;
	}
	catch (...) {
		SAFE_RELEASE(nativePointer);
		delete retVal;
		throw;
	}
	return nullptr;
}

// ���̃N���X�̃C���X�^���X���쐬���܂��B
PixelShader* PixelShader::Create(LPCWSTR fileName, LPCSTR entryPoint, LPCSTR target)
{
	PixelShader* retVal = nullptr;
	ID3DBlob* shaderBytecode = nullptr;
	try {
		// �V�F�[�_�[���R���p�C��
		shaderBytecode = CompileShaderFromFile(fileName, entryPoint, target);
		// ���̃N���X�̃C���X�^���X���쐬
		retVal = Create(shaderBytecode->GetBufferPointer(), shaderBytecode->GetBufferSize());
		// �R���p�C�������o�C�g�R�[�h�͕s�v�Ȃ̂ō폜
		SAFE_RELEASE(shaderBytecode);

		return retVal;
	}
	catch (...) {
		SAFE_RELEASE(shaderBytecode);
		delete retVal;
		throw;
	}
	return nullptr;
}

// ���̃N���X�̃C���X�^���X�����������܂��B
PixelShader::PixelShader(ID3D11PixelShader* nativePointer)
{
	nativePointer->AddRef();
	this->nativePointer = nativePointer;
}

// �f�X�g���N�^�[
PixelShader::~PixelShader()
{
	SAFE_RELEASE(nativePointer);
}

// �l�C�e�B�u�����̃|�C���^�[���擾���܂��B
ID3D11PixelShader* PixelShader::GetNativePointer()
{
	return nativePointer;
}
