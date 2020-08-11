//=============================================================================
// InputLayout.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "InputLayout.h"
#include "Game.h"
#include "DirectXHelper.h"

// このクラスのインスタンスを作成します。
InputLayout* InputLayout::Create(
	GraphicsDevice* graphicsDevice,
	const D3D11_INPUT_ELEMENT_DESC* inputElementDescs, UINT numElements,
	ID3DBlob* shaderBytecodeWithInputSignature)
{
	InputLayout* retVal = nullptr;
	ID3D11InputLayout* nativePointer = nullptr;
	try {
		DX::ThrowIfFailed(
			graphicsDevice->GetDevice()->CreateInputLayout(
				inputElementDescs, numElements,
				shaderBytecodeWithInputSignature->GetBufferPointer(),
				shaderBytecodeWithInputSignature->GetBufferSize(),
				&nativePointer));
		// このクラスのインスタンスを初期化
		retVal = new InputLayout(graphicsDevice, nativePointer);
		SafeRelease(nativePointer);

		return retVal;
	}
	catch (...) {
		SafeRelease(nativePointer);
		SafeDelete(retVal);
		throw;
	}
	return nullptr;
}

// このクラスのインスタンスを初期化します。
InputLayout::InputLayout(
	GraphicsDevice* graphicsDevice, ID3D11InputLayout* nativePointer)
	: GraphicsResource(graphicsDevice)
{
	nativePointer->AddRef();
	this->nativePointer = nativePointer;
}

// デストラクター
InputLayout::~InputLayout()
{
	SafeRelease(nativePointer);
}

// ネイティブ実装のポインターを取得します。
ID3D11InputLayout* InputLayout::GetNativePointer()
{
	return nativePointer;
}
