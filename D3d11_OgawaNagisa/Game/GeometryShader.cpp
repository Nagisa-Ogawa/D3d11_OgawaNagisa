//=============================================================================
// GeometryShader.cpp
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
std::shared_ptr<GeometryShader> GeometryShader::Create(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	const void* shaderBytecode, SIZE_T bytecodeLength)
{
	// �V�F�[�_�[���쐬
	ComPtr<ID3D11GeometryShader> nativePointer;
	ThrowIfFailed(
		graphicsDevice->GetDevice()->CreateGeometryShader(
			shaderBytecode, bytecodeLength,
			NULL,
			&nativePointer));
	// ���̃N���X�̃C���X�^���X��������
	return  shared_ptr<GeometryShader>(
		new GeometryShader(graphicsDevice, nativePointer));
}

// ���̃N���X�̃C���X�^���X���쐬���܂��B
std::shared_ptr<GeometryShader> GeometryShader::Create(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	LPCWSTR fileName, LPCSTR entryPoint, LPCSTR target)
{
	// �V�F�[�_�[���R���p�C��
	ComPtr<ID3DBlob> shaderBytecode(
		CompileShaderFromFile(fileName, entryPoint, target));
	// ���̃N���X�̃C���X�^���X���쐬
	return Create(
		graphicsDevice,
		shaderBytecode->GetBufferPointer(), shaderBytecode->GetBufferSize());
}

// ���̃N���X�̃C���X�^���X�����������܂��B
GeometryShader::GeometryShader(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> nativePointer)
	: GraphicsResource(graphicsDevice)
{
	this->nativePointer = nativePointer;
}

// �l�C�e�B�u�����̃|�C���^�[���擾���܂��B
Microsoft::WRL::ComPtr<ID3D11GeometryShader> GeometryShader::GetNativePointer()
{
	return nativePointer;
}
