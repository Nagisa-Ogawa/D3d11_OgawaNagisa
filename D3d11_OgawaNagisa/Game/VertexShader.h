//=============================================================================
// VertexShader.h
// 
//=============================================================================
#pragma once

#include "Graphics.h"

// 頂点シェーダーを表します。
class VertexShader : public GraphicsResource
{
public:
	// このクラスのインスタンスを作成します。
	template <SIZE_T _Size>
	static VertexShader* Create(
		GraphicsDevice* graphicsDevice, const BYTE(&shaderBytecode)[_Size]);
	// このクラスのインスタンスを作成します。
	static VertexShader* Create(
		GraphicsDevice* graphicsDevice, const void* shaderBytecode, SIZE_T bytecodeLength);
	// このクラスのインスタンスを作成します。
	static VertexShader* Create(
		GraphicsDevice* graphicsDevice, LPCWSTR fileName, LPCSTR entryPoint, LPCSTR target);

	// デストラクター
	~VertexShader();
	// ネイティブ実装のポインターを取得します。
	ID3D11VertexShader* GetNativePointer();
	// このシェーダーのバイトコードを取得します。
	ID3DBlob* GetShaderBytecode();

private:
	ID3D11VertexShader* nativePointer = nullptr;
	ID3DBlob* shaderBytecode = nullptr;
	// このクラスのインスタンスを初期化します。
	VertexShader(GraphicsDevice* graphicsDevice,
		ID3D11VertexShader* nativePointer, ID3DBlob* shaderBytecode);
	// コピー コンストラクター
	VertexShader(const VertexShader&);
};

// このクラスのインスタンスを作成します。
template <SIZE_T _Size>
static VertexShader* VertexShader::Create(GraphicsDevice* graphicsDevice, const BYTE(&shaderBytecode)[_Size])
{
	return 	Create(graphicsDevice, shaderBytecode, _Size);
}
