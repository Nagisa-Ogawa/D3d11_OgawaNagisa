//=============================================================================
// VertexShader.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "Game.h"
#include <d3dcompiler.h>
#include "VertexShader.h"
#include "DirectXHelper.h"

using namespace DX;

namespace
{
	// �t�@�C������V�F�[�_�[���R���p�C�����܂��B
	ID3DBlob* CompileShaderFromFile(
		LPCWSTR fileName, LPCSTR entryPoint, LPCSTR target)
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
			SafeRelease(retVal);
			if (errorMsgs != nullptr) {
				auto message = static_cast<LPCSTR>(errorMsgs->GetBufferPointer());
				OutputDebugStringA(message);
				SafeRelease(errorMsgs);
			}
			throw;
		}
		return nullptr;
	}
}

// ���̃N���X�̃C���X�^���X���쐬���܂��B
VertexShader* VertexShader::Create(
	GraphicsDevice* graphicsDevice, const void* shaderBytecode, SIZE_T bytecodeLength)
{
	VertexShader* retVal = nullptr;
	ID3D11VertexShader* nativePointer = nullptr;
	ID3DBlob* blob = nullptr;
	try {
		// �V�F�[�_�[���쐬
		DX::ThrowIfFailed(
			graphicsDevice->GetDevice()->CreateVertexShader(
				shaderBytecode, bytecodeLength,
				NULL,
				&nativePointer));
		// �����Ŏw�肳�ꂽ�o�C�g�R�[�h�̃R�s�[���쐬
		DX::ThrowIfFailed(D3DCreateBlob(bytecodeLength, &blob));
		CopyMemory(blob->GetBufferPointer(), shaderBytecode, bytecodeLength);
		// ���̃N���X�̃C���X�^���X��������
		retVal = new VertexShader(graphicsDevice, nativePointer, blob);
		SafeRelease(nativePointer);
		SafeRelease(blob);

		return retVal;
	}
	catch (...) {
		SafeRelease(nativePointer);
		SafeRelease(blob);
		SafeDelete(retVal);
		throw;
	}
	return nullptr;
}

// ���̃N���X�̃C���X�^���X���쐬���܂��B
VertexShader* VertexShader::Create(
	GraphicsDevice* graphicsDevice, LPCWSTR fileName, LPCSTR entryPoint, LPCSTR target)
{
	VertexShader* retVal = nullptr;
	ID3D11VertexShader* nativePointer = nullptr;
	ID3DBlob* shaderBytecode = nullptr;
	try {
		// �V�F�[�_�[���R���p�C��
		shaderBytecode = CompileShaderFromFile(fileName, entryPoint, target);
		// �V�F�[�_�[���쐬
		DX::ThrowIfFailed(
			graphicsDevice->GetDevice()->CreateVertexShader(
				shaderBytecode->GetBufferPointer(),
				shaderBytecode->GetBufferSize(),
				NULL,
				&nativePointer));
		// ���̃N���X�̃C���X�^���X��������
		retVal = new VertexShader(graphicsDevice, nativePointer, shaderBytecode);
		SafeRelease(nativePointer);
		SafeRelease(shaderBytecode);

		return retVal;
	}
	catch (...) {
		SafeRelease(shaderBytecode);
		SafeRelease(nativePointer);
		SafeDelete(retVal);
		throw;
	}
	return nullptr;
}

// ���̃N���X�̃C���X�^���X�����������܂��B
VertexShader::VertexShader(
	GraphicsDevice* graphicsDevice,
	ID3D11VertexShader* nativePointer, ID3DBlob* shaderBytecode)
	: GraphicsResource(graphicsDevice)
{
	nativePointer->AddRef();
	shaderBytecode->AddRef();
	this->nativePointer = nativePointer;
	this->shaderBytecode = shaderBytecode;
}

// �f�X�g���N�^�[
VertexShader::~VertexShader()
{
	SafeRelease(shaderBytecode);
	SafeRelease(nativePointer);
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
