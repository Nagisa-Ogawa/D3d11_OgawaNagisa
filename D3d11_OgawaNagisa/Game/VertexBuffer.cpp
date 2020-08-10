//=============================================================================
// VertexBuffer.cpp
// 
//=============================================================================
#include "VertexBuffer.h"
#include "Game.h"
#include "DirectXHelper.h"

// このクラスのインスタンスを作成します。
VertexBuffer* VertexBuffer::Create(const void* initialData, UINT byteWidth)
{
	VertexBuffer* retVal = nullptr;
	ID3D11Buffer* nativePointer = nullptr;
	try {
		// 頂点バッファーについての記述
	// 作成する頂点バッファについての記述
		D3D11_BUFFER_DESC vertexBufferDesc = {};
		vertexBufferDesc.ByteWidth = byteWidth;	// 作成するバッファのサイズ(bytes)
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;	// バッファの使用方法(とりあえずDEFAULT)
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// Vertex Bufferとして利用する
		vertexBufferDesc.CPUAccessFlags = 0;			// CPUからの読み書きに使わない場合は0
		vertexBufferDesc.MiscFlags = 0;				// オプションのフラグ
		vertexBufferDesc.StructureByteStride = 0;		// 頂点バッファとして使うなら0
			// 初期化データについての記述
		D3D11_SUBRESOURCE_DATA subresourceData = {};
		subresourceData.pSysMem = initialData;
		subresourceData.SysMemPitch = 0;
		subresourceData.SysMemSlicePitch = 0;
		// 頂点バッファーを作成
		DX::ThrowIfFailed(
			Graphics::GetGraphicsDevice()->CreateBuffer(
				&vertexBufferDesc,
				&subresourceData,	// バッファーの初期データ
				&nativePointer));
		// このクラスのインスタンスを初期化
		retVal = new VertexBuffer(nativePointer);
		SAFE_RELEASE(nativePointer);

		return retVal;
	}
	catch (...) {
		SAFE_RELEASE(nativePointer);
		SAFE_DELETE(retVal);
		throw;
	}
	return nullptr;
}

// このクラスのインスタンスを初期化します。
VertexBuffer::VertexBuffer(ID3D11Buffer* nativePointer)
{
	nativePointer->AddRef();
	this->nativePointer = nativePointer;
}

// デストラクター
VertexBuffer::~VertexBuffer()
{
	SAFE_RELEASE(nativePointer);
}

// ネイティブ実装のポインターを取得します。
ID3D11Buffer* VertexBuffer::GetNativePointer()
{
	return nativePointer;
}
