//=============================================================================
// Shaders.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "Graphics.h"
#include <d3dcompiler.h>
#include "DirectXHelper.h"

using namespace Microsoft::WRL;
using namespace DX;

namespace
{
	// �t�@�C������V�F�[�_�[���R���p�C�����܂��B
	ID3DBlob* CompileShaderFromFile(
		LPCWSTR fileName, LPCSTR entryPoint, LPCSTR target)
	{
		// �R���p�C�� �t���O
		UINT creationFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
		creationFlags |= D3DCOMPILE_DEBUG;
#endif
		// �V�F�[�_�[���R���p�C��
		HRESULT hr = S_OK;
		ComPtr<ID3DBlob> retVal;
		ComPtr<ID3DBlob> errorMsgs;
		hr = D3DCompileFromFile(
			fileName,
			NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE,
			entryPoint, target, creationFlags, 0,
			&retVal, &errorMsgs);
		if (FAILED(hr)) {
			if (errorMsgs != nullptr) {
				auto message = static_cast<LPCSTR>(errorMsgs->GetBufferPointer());
				OutputDebugStringA(message);
			}
		}

		return retVal.Get();
	}
}

// ���̃N���X�̃C���X�^���X�����������܂��B
VertexShader::VertexShader(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	const void* shaderBytecode, SIZE_T bytecodeLength)
	: GraphicsResource(graphicsDevice)
{
	OnInitialize(graphicsDevice, shaderBytecode, bytecodeLength);
}

// ���̃N���X�̃C���X�^���X�����������܂��B
VertexShader::VertexShader(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	LPCWSTR fileName, LPCSTR entryPoint, LPCSTR target)
	: GraphicsResource(graphicsDevice)
{
	OnInitialize(graphicsDevice, fileName, entryPoint, target);
}

// ���̃N���X�̃C���X�^���X���쐬���܂��B
void VertexShader::OnInitialize(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	const void* shaderBytecode, SIZE_T bytecodeLength)
{
	// �V�F�[�_�[���쐬
	ThrowIfFailed(
		graphicsDevice->GetDevice()->CreateVertexShader(
			shaderBytecode, bytecodeLength,
			NULL,
			&nativePointer));
	// �����Ŏw�肳�ꂽ�o�C�g�R�[�h�̃R�s�[���쐬
	ComPtr<ID3DBlob> blob;
	ThrowIfFailed(D3DCreateBlob(bytecodeLength, &blob));
	CopyMemory(blob->GetBufferPointer(), shaderBytecode, bytecodeLength);
	this->shaderBytecode = blob;
}

// ���̃N���X�̃C���X�^���X���쐬���܂��B
void VertexShader::OnInitialize(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	LPCWSTR fileName, LPCSTR entryPoint, LPCSTR target)
{
	// �V�F�[�_�[���R���p�C��
	shaderBytecode = CompileShaderFromFile(fileName, entryPoint, target);
	// �V�F�[�_�[���쐬
	ThrowIfFailed(
		graphicsDevice->GetDevice()->CreateVertexShader(
			shaderBytecode->GetBufferPointer(),
			shaderBytecode->GetBufferSize(),
			NULL,
			&nativePointer));
}

// �l�C�e�B�u�����̃|�C���^�[���擾���܂��B
Microsoft::WRL::ComPtr<ID3D11VertexShader> VertexShader::GetNativePointer() const
{
	return nativePointer;
}

// ���̃V�F�[�_�[�̃o�C�g�R�[�h���擾���܂��B
Microsoft::WRL::ComPtr<ID3DBlob> VertexShader::GetShaderBytecode() const
{
	return shaderBytecode;
}

// ���̃N���X�̃C���X�^���X�����������܂��B
GeometryShader::GeometryShader(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	const void* shaderBytecode, SIZE_T bytecodeLength)
	: GraphicsResource(graphicsDevice)
{
	OnInitialize(graphicsDevice, shaderBytecode, bytecodeLength);
}

// ���̃N���X�̃C���X�^���X�����������܂��B
GeometryShader::GeometryShader(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	LPCWSTR fileName, LPCSTR entryPoint, LPCSTR target)
	: GraphicsResource(graphicsDevice)
{
	OnInitialize(graphicsDevice, fileName, entryPoint, target);
}

// �������̍ۂɌĂяo����܂��B
void GeometryShader::OnInitialize(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	const void* shaderBytecode, SIZE_T bytecodeLength)
{
	// �V�F�[�_�[���쐬
	ThrowIfFailed(
		graphicsDevice->GetDevice()->CreateGeometryShader(
			shaderBytecode, bytecodeLength,
			NULL,
			&nativePointer));
}

// �������̍ۂɌĂяo����܂��B
void GeometryShader::OnInitialize(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	LPCWSTR fileName, LPCSTR entryPoint, LPCSTR target)
{
	// �V�F�[�_�[���R���p�C��
	ComPtr<ID3DBlob> shaderBytecode(
		CompileShaderFromFile(fileName, entryPoint, target));
	// �V�F�[�_�[���쐬
	ThrowIfFailed(
		graphicsDevice->GetDevice()->CreateGeometryShader(
			shaderBytecode->GetBufferPointer(),
			shaderBytecode->GetBufferSize(),
			NULL,
			&nativePointer));
}

// �l�C�e�B�u�����̃|�C���^�[���擾���܂��B
Microsoft::WRL::ComPtr<ID3D11GeometryShader> GeometryShader::GetNativePointer() const
{
	return nativePointer;
}

// ���̃N���X�̃C���X�^���X�����������܂��B
PixelShader::PixelShader(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	const void* shaderBytecode, SIZE_T bytecodeLength)
	: GraphicsResource(graphicsDevice)
{
	OnInitialize(graphicsDevice, shaderBytecode, bytecodeLength);
}

// ���̃N���X�̃C���X�^���X�����������܂��B
PixelShader::PixelShader(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	LPCWSTR fileName, LPCSTR entryPoint, LPCSTR target)
	: GraphicsResource(graphicsDevice)
{
	OnInitialize(graphicsDevice, fileName, entryPoint, target);
}

// �������̍ۂɌĂяo����܂��B
void PixelShader::OnInitialize(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	const void* shaderBytecode, SIZE_T bytecodeLength)
{
	// �V�F�[�_�[���쐬
	ThrowIfFailed(
		graphicsDevice->GetDevice()->CreatePixelShader(
			shaderBytecode, bytecodeLength,
			NULL,
			&nativePointer));
}

// �������̍ۂɌĂяo����܂��B
void PixelShader::OnInitialize(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	LPCWSTR fileName, LPCSTR entryPoint, LPCSTR target)
{
	// �V�F�[�_�[���R���p�C��
	ComPtr<ID3DBlob> shaderBytecode(
		CompileShaderFromFile(fileName, entryPoint, target));
	// �V�F�[�_�[���쐬
	ThrowIfFailed(
		graphicsDevice->GetDevice()->CreatePixelShader(
			shaderBytecode->GetBufferPointer(),
			shaderBytecode->GetBufferSize(),
			NULL,
			&nativePointer));
}

// �l�C�e�B�u�����̃|�C���^�[���擾���܂��B
Microsoft::WRL::ComPtr<ID3D11PixelShader> PixelShader::GetNativePointer() const
{
	return nativePointer;
}
