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
PixelShader* PixelShader::Create(const void* shaderBytecode, SIZE_T bytecodeLength)
{
	PixelShader* retVal = nullptr;
	ID3D11PixelShader* nativePointer = nullptr;
	try {
		// シェーダーを作成
		DX::ThrowIfFailed(
			Graphics::GetGraphicsDevice()->CreatePixelShader(
				shaderBytecode, bytecodeLength,
				NULL,
				&nativePointer));
		// このクラスのインスタンスを初期化
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

// このクラスのインスタンスを作成します。
PixelShader* PixelShader::Create(LPCWSTR fileName, LPCSTR entryPoint, LPCSTR target)
{
	PixelShader* retVal = nullptr;
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
PixelShader::PixelShader(ID3D11PixelShader* nativePointer)
{
	nativePointer->AddRef();
	this->nativePointer = nativePointer;
}

// デストラクター
PixelShader::~PixelShader()
{
	SAFE_RELEASE(nativePointer);
}

// ネイティブ実装のポインターを取得します。
ID3D11PixelShader* PixelShader::GetNativePointer()
{
	return nativePointer;
}
