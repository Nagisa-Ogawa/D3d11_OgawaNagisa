//=============================================================================
// VertexShader.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "Graphics.h"
#include <d3dcompiler.h>
#include "DirectXHelper.h"

using namespace std;
using namespace Microsoft::WRL;
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
			ThrowIfFailed(
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
std::shared_ptr<VertexShader> VertexShader::Create(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	const void* shaderBytecode, SIZE_T bytecodeLength)
{
	// �V�F�[�_�[���쐬
	ComPtr<ID3D11VertexShader> nativePointer;
	ThrowIfFailed(
		graphicsDevice->GetDevice()->CreateVertexShader(
			shaderBytecode, bytecodeLength,
			NULL,
			&nativePointer));
	// �����Ŏw�肳�ꂽ�o�C�g�R�[�h�̃R�s�[���쐬
	ComPtr<ID3DBlob> blob;
	ThrowIfFailed(D3DCreateBlob(bytecodeLength, &blob));
	CopyMemory(blob->GetBufferPointer(), shaderBytecode, bytecodeLength);
	// ���̃N���X�̃C���X�^���X��������
	shared_ptr<VertexShader> retVal(
		new VertexShader(graphicsDevice, nativePointer, blob));

	return retVal;
}

// ���̃N���X�̃C���X�^���X���쐬���܂��B
std::shared_ptr<VertexShader> VertexShader::Create(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	LPCWSTR fileName, LPCSTR entryPoint, LPCSTR target)
{
	// �V�F�[�_�[���R���p�C��
	ComPtr<ID3DBlob> shaderBytecode;
	shaderBytecode = CompileShaderFromFile(fileName, entryPoint, target);
	// �V�F�[�_�[���쐬
	ComPtr<ID3D11VertexShader> nativePointer;
	ThrowIfFailed(
		graphicsDevice->GetDevice()->CreateVertexShader(
			shaderBytecode->GetBufferPointer(),
			shaderBytecode->GetBufferSize(),
			NULL,
			&nativePointer));
	// ���̃N���X�̃C���X�^���X��������
	return shared_ptr<VertexShader>(
		new VertexShader(graphicsDevice, nativePointer, shaderBytecode));
}

// ���̃N���X�̃C���X�^���X�����������܂��B
VertexShader::VertexShader(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	Microsoft::WRL::ComPtr<ID3D11VertexShader> nativePointer,
	Microsoft::WRL::ComPtr<ID3DBlob> shaderBytecode)
	: GraphicsResource(graphicsDevice)
{
	this->nativePointer = nativePointer;
	this->shaderBytecode = shaderBytecode;
}

// �l�C�e�B�u�����̃|�C���^�[���擾���܂��B
Microsoft::WRL::ComPtr<ID3D11VertexShader> VertexShader::GetNativePointer()
{
	return nativePointer;
}

// ���̃V�F�[�_�[�̃o�C�g�R�[�h���擾���܂��B
Microsoft::WRL::ComPtr<ID3DBlob> VertexShader::GetShaderBytecode()
{
	return shaderBytecode;
}
