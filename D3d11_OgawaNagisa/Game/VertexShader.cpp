//=============================================================================
// VertexShader.cpp
// 
//=============================================================================
#include <d3dcompiler.h>
#include "VertexShader.h"
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
VertexShader* VertexShader::Create(const void* shaderBytecode, SIZE_T bytecodeLength)
{
	VertexShader* retVal = nullptr;
	ID3D11VertexShader* nativePointer = nullptr;
	ID3DBlob* blob = nullptr;
	try {
		// シェーダーを作成
		DX::ThrowIfFailed(
			Graphics::GetGraphicsDevice()->CreateVertexShader(
				shaderBytecode,		// コンパイルされたシェーダーのポインター
				bytecodeLength,		// コンパイルされた頂点シェーダ―のサイズ
				NULL,	// クラスリンケージインターフェイスへのポインター
				&nativePointer));
		// 引数で指定されたバイトコードのコピーを作成
		DX::ThrowIfFailed(D3DCreateBlob(bytecodeLength, &blob));
		CopyMemory(blob->GetBufferPointer(), shaderBytecode, bytecodeLength);
		// このクラスのインスタンスを初期化
		retVal = new VertexShader(nativePointer, blob);
		SAFE_RELEASE(nativePointer);
		SAFE_RELEASE(blob);

		return retVal;
	}
	catch (...) {
		SAFE_RELEASE(nativePointer);
		SAFE_RELEASE(blob);
		SAFE_DELETE(retVal);
		throw;
	}
	return nullptr;
}

// このクラスのインスタンスを作成します。
VertexShader* VertexShader::Create(LPCWSTR fileName, LPCSTR entryPoint, LPCSTR target)
{
	VertexShader* retVal = nullptr;
	ID3D11VertexShader* nativePointer = nullptr;
	ID3DBlob* shaderBytecode = nullptr;
	try {
		// シェーダーをコンパイル
		shaderBytecode = CompileShaderFromFile(fileName, entryPoint, target);
		// シェーダーを作成
		DX::ThrowIfFailed(
			Graphics::GetGraphicsDevice()->CreateVertexShader(
				shaderBytecode->GetBufferPointer(),
				shaderBytecode->GetBufferSize(),
				NULL,
				&nativePointer));
		// このクラスのインスタンスを初期化
		retVal = new VertexShader(nativePointer, shaderBytecode);
		SAFE_RELEASE(nativePointer);
		SAFE_RELEASE(shaderBytecode);

		return retVal;
	}
	catch (...) {
		SAFE_RELEASE(shaderBytecode);
		SAFE_RELEASE(nativePointer);
		SAFE_DELETE(retVal);
		throw;
	}
	return nullptr;
}

// このクラスのインスタンスを初期化します。
VertexShader::VertexShader(
	ID3D11VertexShader* nativePointer, ID3DBlob* shaderBytecode)
{
	nativePointer->AddRef();
	shaderBytecode->AddRef();
	this->nativePointer = nativePointer;
	this->shaderBytecode = shaderBytecode;
}

// デストラクター
VertexShader::~VertexShader()
{
	SAFE_RELEASE(shaderBytecode);
	SAFE_RELEASE(nativePointer);
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
