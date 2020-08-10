//=============================================================================
// InputLayout.cpp
// 
//=============================================================================
#include "InputLayout.h"
#include "Game.h"
#include "DirectXHelper.h"

// このクラスのインスタンスを作成します。
InputLayout* InputLayout::Create(
	const D3D11_INPUT_ELEMENT_DESC* inputElementDescs, UINT numElements,
	ID3DBlob* shaderBytecodeWithInputSignature)
{
	InputLayout* retVal = nullptr;
	ID3D11InputLayout* nativePointer = nullptr;
	try {
		DX::ThrowIfFailed(
			Graphics::GetGraphicsDevice()->CreateInputLayout(
				inputElementDescs, numElements,
				shaderBytecodeWithInputSignature->GetBufferPointer(),
				shaderBytecodeWithInputSignature->GetBufferSize(),
				&nativePointer));
		// このクラスのインスタンスを初期化
		retVal = new InputLayout(nativePointer);
		SAFE_RELEASE(nativePointer);

		return retVal;
	}
	catch (...) {
		SAFE_RELEASE(nativePointer);
		SAFE_DELETE(retVal);
		throw;
	}
	return nullptr;
}

// このクラスのインスタンスを初期化します。
InputLayout::InputLayout(ID3D11InputLayout* nativePointer)
{
	nativePointer->AddRef();
	this->nativePointer = nativePointer;
}

// デストラクター
InputLayout::~InputLayout()
{
	SAFE_RELEASE(nativePointer);
}

// ネイティブ実装のポインターを取得します。
ID3D11InputLayout* InputLayout::GetNativePointer()
{
	return nativePointer;
}
