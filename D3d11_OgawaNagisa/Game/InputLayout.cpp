//=============================================================================
// InputLayout.cpp
// 
//=============================================================================
#include "Game.h"

// このクラスの新しいインスタンスを作成します。
InputLayout* InputLayout::Create(
	ID3D11Device* graphicsDevice,
	const D3D11_INPUT_ELEMENT_DESC* inputElementDescs, UINT numElements,
	const void* shaderBytecodeWithInputSignature, SIZE_T bytecodeLength)
{
	// このクラスのメモリーを確保
	auto result = new InputLayout();
	if (result == nullptr) {
		OutputDebugString(L"メモリーを確保できませんでした。");
		return nullptr;
	}

	// 入力レイアウトを作成
	auto hr = graphicsDevice->CreateInputLayout(
		inputElementDescs, numElements,
		shaderBytecodeWithInputSignature, bytecodeLength,
		&result->inputLayout
	);
	if (FAILED(hr)) {
		OutputDebugString(L"入力レイアウトを作成できませんでした。");
		return nullptr;
	}
	
	return result;
}

// リソースを開放します。
void InputLayout::Release()
{
	SAFE_RELEASE(inputLayout);
	delete this;
}

// D3D11のネイティブポインターを取得します。
ID3D11InputLayout* InputLayout::GetNativePointer()
{
	return inputLayout;
}