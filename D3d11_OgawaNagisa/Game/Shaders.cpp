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
	// ファイルからシェーダーをコンパイルします。
	ID3DBlob* CompileShaderFromFile(
		LPCWSTR fileName, LPCSTR entryPoint, LPCSTR target)
	{
		// コンパイル フラグ
		UINT creationFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
		creationFlags |= D3DCOMPILE_DEBUG;
#endif
		// シェーダーをコンパイル
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

// このクラスのインスタンスを初期化します。
VertexShader::VertexShader(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	const void* shaderBytecode, SIZE_T bytecodeLength)
	: GraphicsResource(graphicsDevice)
{
	OnInitialize(graphicsDevice, shaderBytecode, bytecodeLength);
}

// このクラスのインスタンスを初期化します。
VertexShader::VertexShader(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	LPCWSTR fileName, LPCSTR entryPoint, LPCSTR target)
	: GraphicsResource(graphicsDevice)
{
	OnInitialize(graphicsDevice, fileName, entryPoint, target);
}

// このクラスのインスタンスを作成します。
void VertexShader::OnInitialize(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	const void* shaderBytecode, SIZE_T bytecodeLength)
{
	// シェーダーを作成
	ThrowIfFailed(
		graphicsDevice->GetDevice()->CreateVertexShader(
			shaderBytecode, bytecodeLength,
			NULL,
			&nativePointer));
	// 引数で指定されたバイトコードのコピーを作成
	ComPtr<ID3DBlob> blob;
	ThrowIfFailed(D3DCreateBlob(bytecodeLength, &blob));
	CopyMemory(blob->GetBufferPointer(), shaderBytecode, bytecodeLength);
	this->shaderBytecode = blob;
}

// このクラスのインスタンスを作成します。
void VertexShader::OnInitialize(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	LPCWSTR fileName, LPCSTR entryPoint, LPCSTR target)
{
	// シェーダーをコンパイル
	shaderBytecode = CompileShaderFromFile(fileName, entryPoint, target);
	// シェーダーを作成
	ThrowIfFailed(
		graphicsDevice->GetDevice()->CreateVertexShader(
			shaderBytecode->GetBufferPointer(),
			shaderBytecode->GetBufferSize(),
			NULL,
			&nativePointer));
}

// ネイティブ実装のポインターを取得します。
Microsoft::WRL::ComPtr<ID3D11VertexShader> VertexShader::GetNativePointer() const
{
	return nativePointer;
}

// このシェーダーのバイトコードを取得します。
Microsoft::WRL::ComPtr<ID3DBlob> VertexShader::GetShaderBytecode() const
{
	return shaderBytecode;
}

// このクラスのインスタンスを初期化します。
GeometryShader::GeometryShader(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	const void* shaderBytecode, SIZE_T bytecodeLength)
	: GraphicsResource(graphicsDevice)
{
	OnInitialize(graphicsDevice, shaderBytecode, bytecodeLength);
}

// このクラスのインスタンスを初期化します。
GeometryShader::GeometryShader(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	LPCWSTR fileName, LPCSTR entryPoint, LPCSTR target)
	: GraphicsResource(graphicsDevice)
{
	OnInitialize(graphicsDevice, fileName, entryPoint, target);
}

// 初期化の際に呼び出されます。
void GeometryShader::OnInitialize(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	const void* shaderBytecode, SIZE_T bytecodeLength)
{
	// シェーダーを作成
	ThrowIfFailed(
		graphicsDevice->GetDevice()->CreateGeometryShader(
			shaderBytecode, bytecodeLength,
			NULL,
			&nativePointer));
}

// 初期化の際に呼び出されます。
void GeometryShader::OnInitialize(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	LPCWSTR fileName, LPCSTR entryPoint, LPCSTR target)
{
	// シェーダーをコンパイル
	ComPtr<ID3DBlob> shaderBytecode(
		CompileShaderFromFile(fileName, entryPoint, target));
	// シェーダーを作成
	ThrowIfFailed(
		graphicsDevice->GetDevice()->CreateGeometryShader(
			shaderBytecode->GetBufferPointer(),
			shaderBytecode->GetBufferSize(),
			NULL,
			&nativePointer));
}

// ネイティブ実装のポインターを取得します。
Microsoft::WRL::ComPtr<ID3D11GeometryShader> GeometryShader::GetNativePointer() const
{
	return nativePointer;
}

// このクラスのインスタンスを初期化します。
PixelShader::PixelShader(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	const void* shaderBytecode, SIZE_T bytecodeLength)
	: GraphicsResource(graphicsDevice)
{
	OnInitialize(graphicsDevice, shaderBytecode, bytecodeLength);
}

// このクラスのインスタンスを初期化します。
PixelShader::PixelShader(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	LPCWSTR fileName, LPCSTR entryPoint, LPCSTR target)
	: GraphicsResource(graphicsDevice)
{
	OnInitialize(graphicsDevice, fileName, entryPoint, target);
}

// 初期化の際に呼び出されます。
void PixelShader::OnInitialize(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	const void* shaderBytecode, SIZE_T bytecodeLength)
{
	// シェーダーを作成
	ThrowIfFailed(
		graphicsDevice->GetDevice()->CreatePixelShader(
			shaderBytecode, bytecodeLength,
			NULL,
			&nativePointer));
}

// 初期化の際に呼び出されます。
void PixelShader::OnInitialize(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	LPCWSTR fileName, LPCSTR entryPoint, LPCSTR target)
{
	// シェーダーをコンパイル
	ComPtr<ID3DBlob> shaderBytecode(
		CompileShaderFromFile(fileName, entryPoint, target));
	// シェーダーを作成
	ThrowIfFailed(
		graphicsDevice->GetDevice()->CreatePixelShader(
			shaderBytecode->GetBufferPointer(),
			shaderBytecode->GetBufferSize(),
			NULL,
			&nativePointer));
}

// ネイティブ実装のポインターを取得します。
Microsoft::WRL::ComPtr<ID3D11PixelShader> PixelShader::GetNativePointer() const
{
	return nativePointer;
}
