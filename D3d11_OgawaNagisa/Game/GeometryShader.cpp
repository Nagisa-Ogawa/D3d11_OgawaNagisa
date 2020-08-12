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
	// ファイルからシェーダーをコンパイルします。
	ID3DBlob* CompileShaderFromFile(LPCWSTR fileName, LPCSTR entryPoint, LPCSTR target)
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
std::shared_ptr<GeometryShader> GeometryShader::Create(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	const void* shaderBytecode, SIZE_T bytecodeLength)
{
	// シェーダーを作成
	ComPtr<ID3D11GeometryShader> nativePointer;
	ThrowIfFailed(
		graphicsDevice->GetDevice()->CreateGeometryShader(
			shaderBytecode, bytecodeLength,
			NULL,
			&nativePointer));
	// このクラスのインスタンスを初期化
	return  shared_ptr<GeometryShader>(
		new GeometryShader(graphicsDevice, nativePointer));
}

// このクラスのインスタンスを作成します。
std::shared_ptr<GeometryShader> GeometryShader::Create(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	LPCWSTR fileName, LPCSTR entryPoint, LPCSTR target)
{
	// シェーダーをコンパイル
	ComPtr<ID3DBlob> shaderBytecode(
		CompileShaderFromFile(fileName, entryPoint, target));
	// このクラスのインスタンスを作成
	return Create(
		graphicsDevice,
		shaderBytecode->GetBufferPointer(), shaderBytecode->GetBufferSize());
}

// このクラスのインスタンスを初期化します。
GeometryShader::GeometryShader(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> nativePointer)
	: GraphicsResource(graphicsDevice)
{
	this->nativePointer = nativePointer;
}

// ネイティブ実装のポインターを取得します。
Microsoft::WRL::ComPtr<ID3D11GeometryShader> GeometryShader::GetNativePointer()
{
	return nativePointer;
}
