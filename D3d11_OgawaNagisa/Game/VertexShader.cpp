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
	// ファイルからシェーダーをコンパイルします。
	ID3DBlob* CompileShaderFromFile(
		LPCWSTR fileName, LPCSTR entryPoint, LPCSTR target)
	{
		ID3DBlob* retVal = nullptr;
		ID3DBlob* errorMsgs = nullptr;
		try {
			// コンパイル フラグ
			UINT creationFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
			creationFlags |= D3DCOMPILE_DEBUG;
#endif
			// シェーダーをコンパイル
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

// このクラスのインスタンスを作成します。
VertexShader* VertexShader::Create(
	GraphicsDevice* graphicsDevice, const void* shaderBytecode, SIZE_T bytecodeLength)
{
	VertexShader* retVal = nullptr;
	ID3D11VertexShader* nativePointer = nullptr;
	ID3DBlob* blob = nullptr;
	try {
		// シェーダーを作成
		DX::ThrowIfFailed(
			graphicsDevice->GetDevice()->CreateVertexShader(
				shaderBytecode, bytecodeLength,
				NULL,
				&nativePointer));
		// 引数で指定されたバイトコードのコピーを作成
		DX::ThrowIfFailed(D3DCreateBlob(bytecodeLength, &blob));
		CopyMemory(blob->GetBufferPointer(), shaderBytecode, bytecodeLength);
		// このクラスのインスタンスを初期化
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

// このクラスのインスタンスを作成します。
VertexShader* VertexShader::Create(
	GraphicsDevice* graphicsDevice, LPCWSTR fileName, LPCSTR entryPoint, LPCSTR target)
{
	VertexShader* retVal = nullptr;
	ID3D11VertexShader* nativePointer = nullptr;
	ID3DBlob* shaderBytecode = nullptr;
	try {
		// シェーダーをコンパイル
		shaderBytecode = CompileShaderFromFile(fileName, entryPoint, target);
		// シェーダーを作成
		DX::ThrowIfFailed(
			graphicsDevice->GetDevice()->CreateVertexShader(
				shaderBytecode->GetBufferPointer(),
				shaderBytecode->GetBufferSize(),
				NULL,
				&nativePointer));
		// このクラスのインスタンスを初期化
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

// このクラスのインスタンスを初期化します。
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

// デストラクター
VertexShader::~VertexShader()
{
	SafeRelease(shaderBytecode);
	SafeRelease(nativePointer);
}

// ネイティブ実装のポインターを取得します。
ID3D11VertexShader* VertexShader::GetNativePointer()
{
	return nativePointer;
}

// このシェーダーのバイトコードを取得します。
ID3DBlob* VertexShader::GetShaderBytecode()
{
	return shaderBytecode;
}
