//=============================================================================
// VertexBuffer.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "Game.h"
#include "VertexBuffer.h"
#include "DirectXHelper.h"

using namespace DX;

// このクラスのインスタンスを作成します。
VertexBuffer* VertexBuffer::Create(
	GraphicsDevice* graphicsDevice, const void* initialData, UINT byteWidth)
{
	VertexBuffer* retVal = nullptr;
	ID3D11Buffer* nativePointer = nullptr;
	try {
		// 頂点バッファーについての記述
		D3D11_BUFFER_DESC vertexBufferDesc = {};
		vertexBufferDesc.ByteWidth = byteWidth;
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;
		// 初期化データについての記述
		D3D11_SUBRESOURCE_DATA subresourceData = {};
		subresourceData.pSysMem = initialData;
		subresourceData.SysMemPitch = 0;
		subresourceData.SysMemSlicePitch = 0;
		// 頂点バッファーを作成
		DX::ThrowIfFailed(
			graphicsDevice->GetDevice()->CreateBuffer(
				&vertexBufferDesc, &subresourceData, &nativePointer));
		// このクラスのインスタンスを初期化
		retVal = new VertexBuffer(graphicsDevice, nativePointer);
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
VertexBuffer::VertexBuffer(
	GraphicsDevice* graphicsDevice, ID3D11Buffer* nativePointer)
	: GraphicsResource(graphicsDevice)
{
	nativePointer->AddRef();
	this->nativePointer = nativePointer;
}

// デストラクター
VertexBuffer::~VertexBuffer()
{
	SafeRelease(nativePointer);
}

// ネイティブ実装のポインターを取得します。
ID3D11Buffer* VertexBuffer::GetNativePointer()
{
	return nativePointer;
}
