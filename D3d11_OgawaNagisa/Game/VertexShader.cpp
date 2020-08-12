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

// このクラスのインスタンスを作成します。
std::shared_ptr<VertexShader> VertexShader::Create(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	const void* shaderBytecode, SIZE_T bytecodeLength)
{
	// シェーダーを作成
	ComPtr<ID3D11VertexShader> nativePointer;
	ThrowIfFailed(
		graphicsDevice->GetDevice()->CreateVertexShader(
			shaderBytecode, bytecodeLength,
			NULL,
			&nativePointer));
	// 引数で指定されたバイトコードのコピーを作成
	ComPtr<ID3DBlob> blob;
	ThrowIfFailed(D3DCreateBlob(bytecodeLength, &blob));
	CopyMemory(blob->GetBufferPointer(), shaderBytecode, bytecodeLength);
	// このクラスのインスタンスを初期化
	shared_ptr<VertexShader> retVal(
		new VertexShader(graphicsDevice, nativePointer, blob));

	return retVal;
}

// このクラスのインスタンスを作成します。
std::shared_ptr<VertexShader> VertexShader::Create(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	LPCWSTR fileName, LPCSTR entryPoint, LPCSTR target)
{
	// シェーダーをコンパイル
	ComPtr<ID3DBlob> shaderBytecode;
	shaderBytecode = CompileShaderFromFile(fileName, entryPoint, target);
	// シェーダーを作成
	ComPtr<ID3D11VertexShader> nativePointer;
	ThrowIfFailed(
		graphicsDevice->GetDevice()->CreateVertexShader(
			shaderBytecode->GetBufferPointer(),
			shaderBytecode->GetBufferSize(),
			NULL,
			&nativePointer));
	// このクラスのインスタンスを初期化
	return shared_ptr<VertexShader>(
		new VertexShader(graphicsDevice, nativePointer, shaderBytecode));
}

// このクラスのインスタンスを初期化します。
VertexShader::VertexShader(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	Microsoft::WRL::ComPtr<ID3D11VertexShader> nativePointer,
	Microsoft::WRL::ComPtr<ID3DBlob> shaderBytecode)
	: GraphicsResource(graphicsDevice)
{
	this->nativePointer = nativePointer;
	this->shaderBytecode = shaderBytecode;
}

// ネイティブ実装のポインターを取得します。
Microsoft::WRL::ComPtr<ID3D11VertexShader> VertexShader::GetNativePointer()
{
	return nativePointer;
}

// このシェーダーのバイトコードを取得します。
Microsoft::WRL::ComPtr<ID3DBlob> VertexShader::GetShaderBytecode()
{
	return shaderBytecode;
}
