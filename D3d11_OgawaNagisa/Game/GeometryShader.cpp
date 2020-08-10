//=============================================================================
// GeometryShader.cpp
// 
//=============================================================================
#include <d3dcompiler.h>
#include "GeometryShader.h"
#include "Game.h"
#include "DirectXHelper.h"

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

// このクラスのインスタンスを作成します。
GeometryShader* GeometryShader::Create(const void* shaderBytecode, SIZE_T bytecodeLength)
{
	GeometryShader* retVal = nullptr;
	ID3D11GeometryShader* nativePointer = nullptr;
	try {
		// シェーダーを作成
		DX::ThrowIfFailed(
			Graphics::GetGraphicsDevice()->CreateGeometryShader(
				shaderBytecode, bytecodeLength,
				NULL,
				&nativePointer));
		// このクラスのインスタンスを初期化
		retVal = new GeometryShader(nativePointer);
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

// このクラスのインスタンスを作成します。
GeometryShader* GeometryShader::Create(LPCWSTR fileName, LPCSTR entryPoint, LPCSTR target)
{
	GeometryShader* retVal = nullptr;
	ID3DBlob* shaderBytecode = nullptr;
	try {
		// シェーダーをコンパイル
		shaderBytecode = CompileShaderFromFile(fileName, entryPoint, target);
		// このクラスのインスタンスを作成
		retVal = Create(shaderBytecode->GetBufferPointer(), shaderBytecode->GetBufferSize());
		// コンパイルしたバイトコードは不要なので削除
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

// このクラスのインスタンスを初期化します。
GeometryShader::GeometryShader(ID3D11GeometryShader* nativePointer)
{
	nativePointer->AddRef();
	this->nativePointer = nativePointer;
}

// デストラクター
GeometryShader::~GeometryShader()
{
	SAFE_RELEASE(nativePointer);
}

// ネイティブ実装のポインターを取得します。
ID3D11GeometryShader* GeometryShader::GetNativePointer()
{
	return nativePointer;
}
