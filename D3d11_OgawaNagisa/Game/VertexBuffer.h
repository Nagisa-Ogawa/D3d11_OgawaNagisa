//=============================================================================
// VertexBuffer.h
// 
//=============================================================================
#pragma once

#include "Graphics.h"

// 頂点バッファーを表します。
class VertexBuffer : public GraphicsResource
{
public:
	// このクラスのインスタンスを作成します。
	template <class T, UINT _Size>
	static VertexBuffer* Create(GraphicsDevice* graphicsDevice, const T(&shaderBytecode)[_Size]);
	// このクラスのインスタンスを作成します。
	static VertexBuffer* Create(GraphicsDevice* graphicsDevice, const void* initialData, UINT byteWidth);

	// デストラクター
	~VertexBuffer();
	// ネイティブ実装のポインターを取得します。
	ID3D11Buffer* GetNativePointer();

private:
	ID3D11Buffer* nativePointer = nullptr;
	// このクラスのインスタンスを初期化します。
	VertexBuffer(GraphicsDevice* graphicsDevice, ID3D11Buffer* nativePointer);
	// コピー コンストラクター
	VertexBuffer(const VertexBuffer&);
};

// このクラスのインスタンスを作成します。
template <class T, UINT _Size>
static VertexBuffer* VertexBuffer::Create(
	GraphicsDevice* graphicsDevice, const T(&shaderBytecode)[_Size])
{
	return Create(graphicsDevice, shaderBytecode, sizeof(T) * _Size);
}
