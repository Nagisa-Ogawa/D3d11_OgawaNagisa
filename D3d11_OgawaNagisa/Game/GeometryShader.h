//=============================================================================
// GeometryShader.h
// 
//=============================================================================
#pragma once

#include <d3d11.h>

// ジオメトリー シェーダーを表します。
class GeometryShader
{
public:
	// このクラスのインスタンスを作成します。
	template <SIZE_T _Size>
	static GeometryShader* Create(const BYTE(&shaderBytecode)[_Size]);
	// このクラスのインスタンスを作成します。
	static GeometryShader* Create(const void* shaderBytecode, SIZE_T bytecodeLength);
	// このクラスのインスタンスを作成します。
	static GeometryShader* Create(LPCWSTR fileName, LPCSTR entryPoint, LPCSTR target);

	// デストラクター
	~GeometryShader();
	// ネイティブ実装のポインターを取得します。
	ID3D11GeometryShader* GetNativePointer();

private:
	ID3D11GeometryShader* nativePointer = nullptr;
	// このクラスのインスタンスを初期化します。
	GeometryShader(ID3D11GeometryShader* nativePointer);
	// コピー コンストラクター
	GeometryShader(const GeometryShader&) {}
};

// このクラスのインスタンスを作成します。
template <SIZE_T _Size>
static GeometryShader* GeometryShader::Create(const BYTE(&shaderBytecode)[_Size])
{
	return 	Create(shaderBytecode, _Size);
}


