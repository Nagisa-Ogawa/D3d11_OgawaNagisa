//=============================================================================
// VertexBuffer.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "Graphics.h"
#include "DirectXHelper.h"

using namespace std;
using namespace Microsoft::WRL;
using namespace DX;

// このクラスのインスタンスを作成します。
std::shared_ptr<VertexBuffer> VertexBuffer::Create(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	const void* initialData, UINT byteWidth)
{
	// 頂点バッファーについての記述
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = byteWidth;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;
	// 初期化データについての記述
	D3D11_SUBRESOURCE_DATA subresourceData = {};
	subresourceData.pSysMem = initialData;
	subresourceData.SysMemPitch = 0;
	subresourceData.SysMemSlicePitch = 0;
	// 頂点バッファーを作成
	ComPtr<ID3D11Buffer> nativePointer;
	ThrowIfFailed(
		graphicsDevice->GetDevice()->CreateBuffer(
			&bufferDesc, &subresourceData, &nativePointer));

	return shared_ptr<VertexBuffer>(
		new VertexBuffer(graphicsDevice, nativePointer));
}

// このクラスのインスタンスを初期化します。
VertexBuffer::VertexBuffer(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	Microsoft::WRL::ComPtr<ID3D11Buffer> nativePointer)
	: GraphicsResource(graphicsDevice)
{
	this->nativePointer = nativePointer;
}

// ネイティブ実装のポインターを取得します。
Microsoft::WRL::ComPtr<ID3D11Buffer> VertexBuffer::GetNativePointer() const
{
	return nativePointer;
}

// このクラスのインスタンスを作成します。
std::shared_ptr<IndexBuffer> IndexBuffer::Create(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	const void* initialData, UINT byteWidth, DXGI_FORMAT format)
{
	// 頂点バッファーについての記述
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = byteWidth;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;
	// 初期化データについての記述
	D3D11_SUBRESOURCE_DATA subresourceData = {};
	subresourceData.pSysMem = initialData;
	subresourceData.SysMemPitch = 0;
	subresourceData.SysMemSlicePitch = 0;
	// 頂点バッファーを作成
	ComPtr<ID3D11Buffer> nativePointer;
	ThrowIfFailed(
		graphicsDevice->GetDevice()->CreateBuffer(
			&bufferDesc, &subresourceData, &nativePointer));

	UINT count = 0;
	switch (format) {
	case DXGI_FORMAT_R32_UINT:
		count = byteWidth / sizeof(UINT);
		break;
	case DXGI_FORMAT_R16_UINT:
		count = byteWidth / sizeof(USHORT);
		break;
	default:
		throw DX::com_exception(E_INVALIDARG);
		return nullptr;
	}
	return shared_ptr<IndexBuffer>(
		new IndexBuffer(graphicsDevice, nativePointer, format, count));
}

// このクラスのインスタンスを初期化します。
IndexBuffer::IndexBuffer(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	Microsoft::WRL::ComPtr<ID3D11Buffer> nativePointer,
	DXGI_FORMAT format, UINT count)
	: GraphicsResource(graphicsDevice)
{
	this->nativePointer = nativePointer;
	this->format = format;
	this->count = count;
}

// ネイティブ実装のポインターを取得します。
Microsoft::WRL::ComPtr<ID3D11Buffer> IndexBuffer::GetNativePointer() const
{
	return nativePointer;
}

// インデックス バッファー内の要素のフォーマットを取得します。
DXGI_FORMAT IndexBuffer::GetFormat() const
{
	return format;
}

// インデックス バッファー先頭からのオフセットを取得します。
UINT IndexBuffer::GetOffset() const
{
	return offset;
}

// インデックス バッファー内の要素の数を取得します。
UINT IndexBuffer::GetCount() const
{
	return count;
}
