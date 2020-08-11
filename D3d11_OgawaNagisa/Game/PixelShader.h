//=============================================================================
// PixelShader.h
// 
//=============================================================================
#pragma once

#include "Graphics.h"

// ピクセル シェーダーを表します。
class PixelShader : public GraphicsResource
{
public:
	// このクラスのインスタンスを作成します。
	template <SIZE_T _Size>
	static PixelShader* Create(
		GraphicsDevice* graphicsDevice, const BYTE(&shaderBytecode)[_Size]);
	// このクラスのインスタンスを作成します。
	static PixelShader* Create(
		GraphicsDevice* graphicsDevice, const void* shaderBytecode, SIZE_T bytecodeLength);
	// このクラスのインスタンスを作成します。
	static PixelShader* Create(
		GraphicsDevice* graphicsDevice, LPCWSTR fileName, LPCSTR entryPoint, LPCSTR target);

	// デストラクター
	~PixelShader();
	// ネイティブ実装のポインターを取得します。
	ID3D11PixelShader* GetNativePointer();

private:
	ID3D11PixelShader* nativePointer = nullptr;
	// このクラスのインスタンスを初期化します。
	PixelShader(GraphicsDevice* graphicsDevice, ID3D11PixelShader* nativePointer);
	// コピー コンストラクター
	PixelShader(const PixelShader&);
};

// このクラスのインスタンスを作成します。
template <SIZE_T _Size>
static PixelShader* PixelShader::Create(GraphicsDevice* graphicsDevice, const BYTE(&shaderBytecode)[_Size])
{
	return 	Create(graphicsDevice, shaderBytecode, _Size);
}
