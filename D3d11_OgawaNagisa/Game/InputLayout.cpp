//=============================================================================
// InputLayout.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "Graphics.h"
#include "DirectXHelper.h"

using namespace DX;

// このクラスのインスタンスを初期化します。
InputLayout::InputLayout(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	const D3D11_INPUT_ELEMENT_DESC* inputElementDescs, UINT numElements,
	const Microsoft::WRL::ComPtr<ID3DBlob> shaderBytecodeWithInputSignature)
	: GraphicsResource(graphicsDevice)
{
	OnInitialize(
		graphicsDevice,
		inputElementDescs, numElements,
		shaderBytecodeWithInputSignature);
}

// このクラスのインスタンスを作成します。
void InputLayout::OnInitialize(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	const D3D11_INPUT_ELEMENT_DESC* inputElementDescs, UINT numElements,
	const Microsoft::WRL::ComPtr<ID3DBlob> shaderBytecodeWithInputSignature)
{
	ThrowIfFailed(
		graphicsDevice->GetDevice()->CreateInputLayout(
			inputElementDescs, numElements,
			shaderBytecodeWithInputSignature->GetBufferPointer(),
			shaderBytecodeWithInputSignature->GetBufferSize(),
			&nativePointer));
}

// ネイティブ実装のポインターを取得します。
Microsoft::WRL::ComPtr<ID3D11InputLayout> InputLayout::GetNativePointer() const
{
	return nativePointer;
}
