//=============================================================================
// GeometryShader.h
// 
//=============================================================================
#pragma once

#include "Graphics.h"

// ジオメトリー シェーダーを表します。
class GeometryShader : public GraphicsResource
{
public:
	// このクラスのインスタンスを作成します。
	template <SIZE_T _Size>
	static GeometryShader* Create(
		GraphicsDevice* graphicsDevice, const BYTE(&shaderBytecode)[_Size]);
	// このクラスのインスタンスを作成します。
	static GeometryShader* Create(
		GraphicsDevice* graphicsDevice, const void* shaderBytecode, SIZE_T bytecodeLength);
	// このクラスのインスタンスを作成します。
	static GeometryShader* Create(
		GraphicsDevice* graphicsDevice, LPCWSTR fileName, LPCSTR entryPoint, LPCSTR target);

	// デストラクター
	~GeometryShader();
	// ネイティブ実装のポインターを取得します。
	ID3D11GeometryShader* GetNativePointer();

private:
	ID3D11GeometryShader* nativePointer = nullptr;
	// このクラスのインスタンスを初期化します。
	GeometryShader(GraphicsDevice* graphicsDevice,
		ID3D11GeometryShader* nativePointer);
	// コピー コンストラクター
	GeometryShader(const GeometryShader&);
};

// このクラスのインスタンスを作成します。
template <SIZE_T _Size>
static GeometryShader* GeometryShader::Create(
	GraphicsDevice* graphicsDevice, const BYTE(&shaderBytecode)[_Size])
{
	return 	Create(graphicsDevice, shaderBytecode, _Size);
}
