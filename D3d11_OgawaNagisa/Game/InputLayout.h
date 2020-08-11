//=============================================================================
// InputLayout.h
// 
//=============================================================================
#pragma once

#include <d3d11.h>
#include "Graphics.h"

// 入力レイアウトを表します。
class InputLayout : public GraphicsResource
{
public:
	// このクラスのインスタンスを作成します。
	template <UINT _Size>
	static InputLayout* Create(
		GraphicsDevice* graphicsDevice,
		const D3D11_INPUT_ELEMENT_DESC(&shaderBytecode)[_Size],
		ID3DBlob* shaderBytecodeWithInputSignature);
	// このクラスのインスタンスを作成します。
	static InputLayout* Create(
		GraphicsDevice* graphicsDevice,
		const D3D11_INPUT_ELEMENT_DESC* inputElementDescs, UINT numElements,
		ID3DBlob* shaderBytecodeWithInputSignature);

	// デストラクター
	~InputLayout();
	// ネイティブ実装のポインターを取得します。
	ID3D11InputLayout* GetNativePointer();

private:
	ID3D11InputLayout* nativePointer = nullptr;
	// このクラスのインスタンスを初期化します。
	InputLayout(GraphicsDevice* graphicsDevice, ID3D11InputLayout* nativePointer);
	// コピー コンストラクター
	InputLayout(const InputLayout&);
};

// このクラスのインスタンスを作成します。
template <UINT _Size>
static InputLayout* InputLayout::Create(
	GraphicsDevice* graphicsDevice,
	const D3D11_INPUT_ELEMENT_DESC(&shaderBytecode)[_Size],
	ID3DBlob* shaderBytecodeWithInputSignature)
{
	return Create(graphicsDevice, shaderBytecode, _Size, shaderBytecodeWithInputSignature);
}
