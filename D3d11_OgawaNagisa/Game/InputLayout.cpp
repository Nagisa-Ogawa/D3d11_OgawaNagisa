//=============================================================================
// InputLayout.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "Graphics.h"
#include "DirectXHelper.h"

using namespace std;
using namespace Microsoft::WRL;
using namespace DX;

// このクラスのインスタンスを作成します。
std::shared_ptr<InputLayout> InputLayout::Create(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	const D3D11_INPUT_ELEMENT_DESC* inputElementDescs, UINT numElements,
	Microsoft::WRL::ComPtr<ID3DBlob> shaderBytecodeWithInputSignature)
{
	ComPtr<ID3D11InputLayout> nativePointer;
	ThrowIfFailed(
		graphicsDevice->GetDevice()->CreateInputLayout(
			inputElementDescs, numElements,
			shaderBytecodeWithInputSignature->GetBufferPointer(),
			shaderBytecodeWithInputSignature->GetBufferSize(),
			&nativePointer));
	// このクラスのインスタンスを初期化
	return shared_ptr<InputLayout>(
		new InputLayout(graphicsDevice, nativePointer));
}

// このクラスのインスタンスを初期化します。
InputLayout::InputLayout(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	Microsoft::WRL::ComPtr<ID3D11InputLayout> nativePointer)
	: GraphicsResource(graphicsDevice)
{
	this->nativePointer = nativePointer;
}

// ネイティブ実装のポインターを取得します。
Microsoft::WRL::ComPtr<ID3D11InputLayout> InputLayout::GetNativePointer()
{
	return nativePointer;
}
